#include <Des.h>
#include <memory.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <vector>
#include <bitset>
using namespace std;


// --------- -----------------------------------------

DesData::DesData() {
    memset(x, 0, sizeof(x));
}

DesData::DesData(char * s) {
    memcpy(x, s, sizeof(x));
}

BitData<32> DesData::get_L() {
    BitData<32> res;
    for(int i = 0; i < 32; i++) {
        res.set(i, (*this)[i]);
    }
    return res;
}
BitData<32> DesData::get_R() {
    BitData<32> res;
    for(int i = 32; i < 64; i++) {
        res.set(i-32, (*this)[i]);
    }
    return res;
}

void DesData::set_LR(BitData<32> l, BitData<32> r) {
    for(int i = 0; i < 32; i++) {
        this->set(i, l[i]);
        this->set(i+32, r[i]);
    }
}

// void DesData::swapLR() {

// }

KeyData::KeyData() {
    memset(x, 0, sizeof(x));
}

// uchar KeyData::operator [] (int idx) {
//     int r = idx & MASK;
//     idx >>= 3;
//     return (x[idx] >> r) & 1;
// }

// void KeyData::set(int idx, uchar val) {
//     int r = idx & MASK;
//     idx >>= 3;
//     if(val > 0) {
//         x[idx] |= ( 1<< r);
//     }
//     else {
//         x[idx] &= ~(1 << r);
//     }
// }

KeyData KeyData::shift_left(int t) {
    KeyData res ;
    for(int i = 0; i < 28; i++) {
        res.set(i, (*this)[(i+t)%28]);
    }
    for(int i = 28; i < 56; i++) {
        int idx = i - 28;
        idx = (idx + t) % 28 + 28;
        res.set(i, (*this)[idx]);
    }
    return res;
}

KeyData48::KeyData48() {
    memset(x, 0, sizeof(x));
}

// -----------------------------------------------------------

int Des::P64[64] = {
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8,  0, 
    48, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4, 
    62, 52, 46, 40, 30, 22, 16, 6
}; 
int Des::P64_inv[64];

int Des::PC_1[56] = {
    56, 48, 40, 32, 24, 16, 8 , 0 , 57, 49, 41, 33, 25, 17,
    9,  1,  58, 50, 42, 34, 26, 18, 10, 2 , 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14, 6 , 61, 53, 45, 37, 29, 21, 
    13, 5,  60, 52, 44, 36, 28, 20, 12, 4,  27, 19, 11, 3 
};

int Des::PC_2[48] = {
    13, 16, 10, 23, 0 , 4,  2,  27, 14, 5,  20, 9, 
    22, 18, 11, 3,  25, 7 , 15, 6,  26, 19, 12, 1,
    40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31
};


bool Des::isInit = Des::init();


bool Des::init() {
    for(int i = 0; i <64; i++) {
        P64_inv[P64[i]] = i;
    }
    return 1;
}

string Des::process(string text, string key, int type) {

    // ---------------------------
    // Debug 
    #ifdef DEBUG
        long long x = 0x0123456789ABCDEF;
        long long y = 0x133457799BBCDFF1;
    #endif
    // ---------------------------


    string res = "";
    DesData k;
    assert(key.size() >= 8);
    for(int i = 0; i < 8; i++) {
        k.set_int8(i, key[i]);
    }

    // ------------------
    #ifdef DEBUG
        for(int i = 0; i < 64; i++) {
            k.set(63-i, y & 1);
            y >>= 1;
        }
    #endif
    // -------------------

    vector<KeyData48> k_vec = Des::gen_K(k);
    if(type == -1) {
        reverse(k_vec.begin(), k_vec.end());
    }
    int times = (text.size() + 7) / 8;
    for(int i = 0, j = 0; i < times; i++) {
        DesData t;
        for(int k = 0; k < 8; j++, k++) {
            t.set_int8(k, (j < text.size() ? text[j] : 0));
        }
        // -----------------------
        #ifdef DEBUG
            for(int k = 0; k < 64; k++) {
                t.set(63-k, x & 1);
                x >>= 1;
            }
        #endif
        // -----------------------
        if(type == -1) {
            auto cur =  process(t, k_vec);
            for(int i = 0; i < 8; i++) {
                res += (char)cur.get_int8(i);
            }
        }
        else{ 
            res += process(t, k_vec).to_str();
        }
    }
    return res;
} 

DesData Des::process(DesData t, const vector<KeyData48> &k) {
    DesData now;
    for(int i = 0; i < 64; i++) {
        now.set(i, t[P64[i]]);
    }
    auto L0 = now.get_L();
    auto R0 = now.get_R();
    auto L1 = L0, R1 = R0;
    for(int i = 0; i < 16; i++) {
        #ifdef DEBUG
            printf("L_%d: ", i);
            for(int i = 0; i < 32; i++) {
                printf("%d", L0[i]);
            }
            puts("");
            printf("R_%d: ", i);
            for(int i = 0; i < 32; i++) {
                printf("%d", R0[i]);
            }
            puts("");
        #endif
        R1 = L0 ^ f(R0, k[i]);
        L1 = R0;
        L0 = L1;
        R0 = R1;
        
    }
    now.set_LR(R1, L1);
    DesData res;
    for(int i = 0; i < 64; i++) {
        res.set(i, now[P64_inv[i]]);
    }
    return res;
}

vector<KeyData48> Des::gen_K(DesData k) {
    vector<KeyData48> res;
    KeyData kd;
    for(int i = 0; i < 56; i++) {
        kd.set(i, k[PC_1[i]]);
    }
    #ifdef DEBUG 
        for(int i = 0; i < 56; i++) {
            printf("%d", kd[i]);
        }
        puts("");
    #endif
    for(int i = 1; i <= 16; i++) {
        if(i == 1 || i == 2 || i == 9 || i == 16) {
            kd = kd.shift_left(1);
        }
        else {
            kd = kd.shift_left(2);
        }
        #ifdef DEBUG
            for(int i = 0; i < 28; i++) {
                printf("%d", kd[i]);
            } 
            printf(" ");
            for(int i = 0; i < 28; i++) {
                printf("%d", kd[i+28]);
            }
            puts("");
        #endif
        KeyData48 kd48;
        for(int j = 0; j < 48; j++) {
            kd48.set(j, kd[PC_2[j]]);
        }
        #ifdef DEBUG
            printf("%d: ", i);
            for(int j = 0; j < 48; j++) {
                printf("%d", kd48[j]);
            }
            puts("");
        #endif
        res.push_back(kd48);
    }
    return res;
}

BitData<32> Des::f(BitData<32> data, const KeyData48 & k) {
    const int E[] = {
        31, 0,  1,  2,  3,  4, 
        3,  4,  5,  6,  7,  8, 
        7,  8,  9,  10, 11, 12,
        11, 12, 13, 14, 15, 16,
        15, 16, 17, 18, 19, 20,
        19, 20, 21, 22, 23, 24,
        23, 24, 25, 26, 27, 28,
        27, 28, 29, 30, 31, 0
    };
    const int S[8][4][16] =
    {
        {
            {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
            {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
            {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
            {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
        },
        {
            {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
            {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
            {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
            {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
        },
        {
            {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
            {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
            {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
            {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
        },
        {
            {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
            {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
            {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
            {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
        },
        {
            {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
            {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
            {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
            {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
        },
        {
            {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
            {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
            {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
            {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
        },
        {
            {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
            {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
            {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
            {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
        },
        {
            {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
            {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
            {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
            {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11},
        }
    };
    BitData<48> extData;
    // BitData<32> data;
    // for(int i = 0; i < 32; i++) {
    //     data.set(i, R & 1);
    //     R >>= 1;
    // }
    for(int i = 0; i < 48; i++) {
        extData.set(i, data[E[i]]);
    }
    extData = extData ^ (BitData<48>)k;
    BitData<32> res;
    for(int i = 0, t = 0, k = 0; i < 48; t++) {
        int x = 0, y = 0;
        y = extData[i++];
        for(int j = 0; j < 4; j++) {
            x = (x << 1) | extData[i++];
        }
        y = (y << 1) | extData[i++];
        for(int j = 0; j < 4; j++, k++) {
            res.set(k+j, (S[t][y][x] >> j) & 1);
        }
    }
    // uint res_int = 0;
    // for(int i = 0; i < 32; i++) {
    //     res_int = (res_int << 1) | res[i];
    // }
    return res;
}