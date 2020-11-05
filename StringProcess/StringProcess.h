#include <string>
#include <iostream>
#include <vector>
using namespace std;

typedef unsigned char uchar;
typedef unsigned int  uint;

uchar char2HexNum(char c) {
    if(c >= '0' and c <= '9') return c - '0';
    if(c >= 'a' and c <= 'z') return c - 'a' + 10;
    if(c >= 'A' and c <= 'Z') return c - 'A' + 10;
    return 0xff;
}

// hexStr -> vector<uchar>
vector<uchar> cvtHexStr(const string &str)  {
    vector<uchar> hex;
    if(str.size() % 2 != 0) {
        throw "cvtHexStr Format Error";
    }
    for(int i = 0; i < str.size();) {
        uchar x = 0;
        uchar t = 0;
        if((t = char2HexNum(str[i++])) == 0xff) {
            throw "cvtHexStr Format Error";
        }
        x = (x << 4) + t;
        if((t = char2HexNum(str[i++])) == 0xff) {
            throw "cvtHexStr Format Error";
        }        
        x = (x << 4) + t;
        hex.push_back(x);
    }
    return hex;
} 

string cvtVec_uchar(const vector<uchar> &vec) {
    string res;
    char buffer[5];
    for(auto num : vec) {
        uchar x = num >> 4;
        uchar y = num & 0xf;
        sprintf(buffer,"%x%x", x, y);
        res.push_back(buffer[0]);
        res.push_back(buffer[1]);
    }
    return res;
}