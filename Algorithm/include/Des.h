#ifndef __DES_H
#define __DES_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <stdlib.h>
using namespace std;

typedef  unsigned char uchar ;
typedef  unsigned int  uint  ;
#define MASK 0x7

template<int bits>
class BitData
{
public:
    BitData () {
        memset(x, 0, sizeof(x));
    }
    void set(int idx, uchar val) {
        uchar r = idx & MASK;
        idx >>= 3;
        if(val) {
            x[idx] |= (1 << r);
        }
        else {
            x[idx] &= ~(1 << r);
        }
    }
    uchar operator [](int idx) const {
        uchar r = idx& MASK;
        idx >>= 3;
        return (x[idx] >> r) & 1;
    }
    BitData<bits> operator ^ ( BitData <bits> other)  {
        BitData<bits> res;
        for(int i = 0; i < bits; i++) {
            res.set(i, (*this)[i] ^ other[i]);
        }
        return res;
    }
    void set_int8(int idx, uchar val){
        x[idx] = val;
    }
    uchar get_int8(int idx)  {
        return x[idx];
    }
    string to_str(){
        string res = "";
        char buffer[5];
        for(int i = 0; i < bits; ) {
            char x = 0;
            for(int j = 0; j < 4; j++) {
                x = (x << 1) | (*this)[i++];
            }
            sprintf(buffer, "%x", x);
            res += buffer[0];
        }
        return res;
    }
    friend ostream & operator<< (ostream & out, const BitData<bits> & data) {
        for(int i = 0; i < bits; i++) {
            printf("%d", data[i]);
        }
        return out;
    }
protected:
    uchar x[bits/8];
};

// -------------------------------------------------------------

class DesData  : public BitData<64> 
{
public:
    DesData() ;
    DesData(char* s) ;
    // unsigned char operator [] (int idx)   ;
    // void set_int8(int idx, uchar val);
    // uchar get_int8(int idx);
    // void set(int idx, uchar val) ;
    // string to_str();
    BitData<32> get_L();
    BitData<32> get_R();
    void  set_LR(BitData<32> l, BitData<32> r);
    // void swapLR() ;
protected:
    // unsigned char x[8];    
} ;

class KeyData :public BitData<56>
{
public:
    KeyData();
    // void set(int idx, uchar val) ;
    // uchar operator [] (int idx) ;
    KeyData shift_left(int t) ;
protected:
    // unsigned char x[7];
};

class KeyData48 : public BitData<48>
{
public:
    KeyData48();
};

class Des 
{
public :
    static bool isInit;
    static bool init();
    static string process(string text, string key, int type) ;
    static int P64[64] ;
    static int P64_inv[64];
    static int PC_1[56];
    static int PC_2[48];
protected:
    static DesData process(DesData t, const vector<KeyData48>& k) ;
    static vector<KeyData48> gen_K(DesData k) ;
    static BitData<32> f(BitData<32> R, const KeyData48 &k) ;
};

#endif