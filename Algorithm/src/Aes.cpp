#include <Aes.h>
#include <iostream>
#include <string.h>
#include <datatype.h>
#include <assert.h>
#include <StringProcess.h>
using namespace std;


AesData::AesData() {
    memset(x, 0, sizeof(x));    
}
AesData::AesData(const vector<uchar> &hex_arr) {
    if(hex_arr.size() > 16) {
        printf("\033[1;33mWarning: AesData hex_arr.size > 16\n\033[0m");
    }
    for(int i = 0, k = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            x[j][i] = hex_arr[k++];
        }
    }
}

AesData AesData::operator^(const AesData &other) const  {
    AesData res;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            res[i][j] = x[i][j] ^ other.x[i][j];
        }
    }
    return res;
} 

uchar* AesData::operator[] (int idx) {
    return x[idx];
}

vector<uchar> AesData::column(int idx) {
    vector<uchar> res ;
    for(int i = 0; i < 4; i++) {
        res.push_back(x[i][idx]);
    }
    return res;
}

void AesData::setColumn(int idx, const vector<uchar> &w) {
    for(int i = 0; i < 4; i++) {
        x[i][idx] = w[i];
    }
}

void AesData::rawShift(int idx, int k) {
    uchar buf[4];
    memcpy(buf, x[idx], sizeof(buf));
    for(int i = 0; i < 4; i++) {
        x[idx][i] = buf[(i+k+4)%4];
    }
}

ostream& operator << (ostream &out, const AesData &data) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            printf("%2x ", data.x[i][j]);
        }
        puts("");
    }
    return out;
}

int Aes::S[16][16] = { 
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};

int Aes::invS[16][16] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

Aes::Aes() {

}
 
string Aes::process(const string &text, const string &key){
#ifdef  DEBUG
    AesData _t(vector<uchar>({0x32, 0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,
                            0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34}));
    AesData  _k(vector<uchar>({0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
                                0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c}));
//    AesData _k(vector<uchar>({0x3C, 0xA1, 0x0B, 0x21, 0x57, 0xF0, 0x19, 0x16, 0x90, 0x2E, 0x13, 0x80, 0xAC, 0xC1, 0x07, 0xBD}));
    auto _chiper = process(_t, _k);
    vector<uchar> vec ;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            vec.push_back(_chiper[j][i]);
        }
    }
    return cvtVec_uchar(vec);
#else
    assert(key.size() >= 16);
    vector<uchar> byte_text;
    for(int i = 0; i < text.size(); i++) {
        byte_text.push_back((uchar)text[i]);
    }
    int r = byte_text.size() - byte_text.size() / 16 * 16;
    r = (16 - r) % 16;
    for(int i = 0; i < r; i++) byte_text.push_back(0);
    AesData aes_key(vector<uchar>(key.begin(), key.end()));
    // vector<AesData> cipher;
    string res;
    for(int i = 0; i < byte_text.size(); i+= 16) {
        AesData aes_text(vector<uchar>(&byte_text[i], &byte_text[i+16]));
        // cipher.push_back(process(aes_text, aes_key));
        auto cipher = process(aes_text, aes_key);
        vector<uchar> vec;
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) 
                vec.push_back(cipher[k][j]);
        }
        res += cvtVec_uchar(vec);
    }
    return res;
#endif
}

string Aes::processInv(const string &text, const string &key){
    assert(key.size() >= 16) ;
    vector<uchar> byte_text = cvtHexStr(text);
    assert(key.size() % 16 == 0);
    AesData  aes_key(vector<uchar>(key.begin(), key.end()));
#ifdef DEBUG
    AesData  _k(vector<uchar>({0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
                               0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c}));
    aes_key = _k;
#endif
    string  res;
    for(int i = 0; i < byte_text.size(); i+=16) {
        AesData  aes_text(vector<uchar>(&byte_text[i], &byte_text[i+16]));
        AesData _t = processInv(aes_text, aes_key);
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                res += (char)_t[k][j];
            }
        }
    }
    return res;
}

AesData Aes::process(const  AesData& text, const AesData& key) {
    vector<AesData> key_arr = genKey(key);
#ifdef  DEBUG
    cout << "Key Arr: \n";
    for(auto _k : key_arr) {
        cout << _k  << "\n";
    }
#endif
    AesData data = key_arr[0] ^ text;
    for(int i = 1; i <= 10; i++) {
        // subByte
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                data[k][j] = SubByte(data[k][j]);
            }
        }
#ifdef DEBUG
        cout << "subByte: " << i << "\n";
        cout << data << "\n";
#endif
        // raw shift
        for(int j = 1; j < 4; j++) {
            data.rawShift(j, j);
        }
#ifdef DEBUG
        cout << "raw shift: " << i << "\n";
        cout << data <<"\n";
#endif
        //
        if(i != 10) {
            data = MixColumn(data);
        }
#ifdef  DEBUG
        cout << "mix column: " << i << "\n";
        cout << data << '\n';
#endif
        data = key_arr[i] ^ data;
#ifdef  DEBUG
        cout << "cipher: " << i << "\n";
        cout << data << "\n";
#endif
    }

    return data;
}

AesData Aes::processInv(const AesData &text, const AesData  &key) {
    vector<AesData> key_arr = genKey(key);
    reverse(key_arr.begin(), key_arr.end());
    AesData data = key_arr[0] ^ text;
    for(int i = 1; i <= 10; i++) {
        // row shift inv
        for(int j = 1; j < 4; j++) {
            data.rawShift(j, -j);
        }
        // sub byte inv
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                data[k][j] = SubByteInv(data[k][j]);
            }
        }
        // key
        data = data ^ key_arr[i];
        // mix column inv
        if(i != 10) {
            data = MinColumnInv(data);
        }
    }
    return data;
}

vector<AesData> Aes::genKey(const AesData & key) {
    const int Nr = 11;
    const uint Rcon[] = {
        0x01000000, 0x02000000,
        0x04000000, 0x08000000,
        0x10000000, 0x20000000,
        0x40000000, 0x80000000,
        0x1b000000, 0x36000000
    };
    vector<AesData> key_arr;
    key_arr.push_back(key);
    for(int i = 1; i < Nr; i++) {
        int last = key_arr.size()-1;
        AesData cur;
        auto k0 = key_arr[last].column(0);
        auto k3 = key_arr[last].column(3);
        vector<uchar> kk = apply(vecRot(k3, 1), this, &Aes::SubByte);
//        cout << cvtVec_uchar(kk) << "\n";
        auto rcon_vec = int2vec(Rcon[i-1]);
//        cout << cvtVec_uchar(rcon_vec) << "\n";
        cur.setColumn(0, vecXor(vecXor(k0, kk), rcon_vec));
//        cout << cvtVec_uchar(cur.column(0)) << "\n";
        for(int j = 1; j < 4; j++) {
            auto u = key_arr[last].column(j);
            auto v = cur.column(j-1);
            cur.setColumn(j, vecXor(u, v));
        }
        key_arr.push_back(cur);
    }
    return key_arr;
}

uchar Aes::SubByte(uchar in) {
    uchar x = in >> 4;
    uchar y = in & 0xf;
    return S[x][y];
}

uchar Aes::SubByteInv(uchar in) {
    uchar x = in >> 4;
    uchar y = in & 0xf;
    return invS[x][y];
}

uchar Aes::GFmul(uchar a, uchar b) {
    uchar powb[8];
    powb[0] = b;
    for(int i = 1; i < 8; i++) {
        powb[i] = powb[i-1] << 1;
        if(powb[i-1] & 0x80) {
            powb[i] ^= 0x1b;
        }
    }
    uchar res = 0;
    for(int i = 0; i < 8; i++) {
        if((a>>i) & 1) {
            res ^= powb[i];
        }
    }
    return res;
}

AesData Aes::MixColumn(AesData data) {
    AesData res;
    int mat[4][4] = {
        2, 3, 1, 1,
        1, 2, 3, 1,
        1, 1, 2, 3,
        3, 1, 1, 2
    };
    for(int c = 0; c < 4; c++) {
        for(int r = 0; r < 4; r++) {
            uchar cur = 0;
            for(int k = 0; k < 4; k++) {
                cur ^= GFmul(mat[r][k], data[k][c]);
            }
            res[r][c] = cur;
        }        
    }
    return res;
}

AesData  Aes::MinColumnInv(AesData data) {
    AesData res;
    int mat[4][4] = {
            0xe, 0xb, 0xd, 0x9,
            0x9, 0xe, 0xb, 0xd,
            0xd, 0x9, 0xe, 0xb,
            0xb, 0xd, 0x9, 0xe
    };
    for(int c = 0; c < 4; c++) {
        for(int r = 0; r < 4; r++) {
            uchar cur = 0;
            for(int k = 0; k < 4; k++) {
                cur ^= GFmul(mat[r][k], data[k][c]);
            }
            res[r][c] = cur;
        }
    }
    return res;
}