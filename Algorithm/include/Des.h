#ifndef __DES_H
#define __DES_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <datatype.h>
using namespace std;


#define MASK 0x7


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