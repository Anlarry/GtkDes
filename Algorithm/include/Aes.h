#ifndef  __AES_H
#define  __AES_H

typedef unsigned char uchar;
typedef unsigned int  uint;

#include <vector>
#include <string>
using namespace std;

class AesData 
{
public:
    AesData() ;
    AesData(const std::vector<uchar>& hex_arr);
    uchar* operator[] (int idx) ;
    vector<uchar> column(int idx);
    void setColumn(int idx, const vector<uchar>& w);
    AesData operator ^ (const AesData &other ) const ;
    void rawShift(int idx, int k);
    friend ostream& operator << (ostream &out, const AesData &data);
protected:
    uchar x[4][4];
};

class Aes
{
public:
    static int S[16][16] ;
    static int invS[16][16];
    Aes() ;
    string process(const string &text, const string &key) ;
    string processInv(const string &text, const string &key);
protected:
    AesData process(const AesData& text, const AesData& key);
    AesData processInv(const AesData  &text, const AesData  &key);
    vector<AesData> genKey(const AesData & key) ;
    uchar SubByte(uchar in);
    uchar SubByteInv(uchar in);
    uchar GFmul(uchar a, uchar b);
    AesData MixColumn( AesData data);
    AesData MinColumnInv(AesData data);
};

#endif