#ifndef __DATATYPE_H
#define __DATATYPE_H

typedef  unsigned char uchar ;
typedef  unsigned int  uint  ;

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

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

template<int bytes>
class ByteData
{
public:
    ByteData() {

    }
    ByteData(const vector<uchar> &hex_arr) {
        
    }
};


template<typename T>
vector<T> vecXor(const vector<T>& A, const vector<T> &B) {
    if(A.size() != B.size()) {
        printf("\033[1;31mError: vecXor A.size != B.size\n\033[0m");
        return vector<T>(0);
    }
    vector<T> res(A.size());
    for(int i = 0; i < res.size(); i++) {
        res[i] = A[i] ^ B[i];
    }
    return res;
}

template<typename T>
vector<T> vecRot(const vector<T> &A, int k) {
    vector<T> res(A.size());
    int len = A.size();
    for(int i = 0; i < res.size(); i++) {
        res[i] = A[(i+k)%len];
    }
    return res;
}

template<typename T> // T is integer
vector<uchar> int2vec(T num) {
    uchar *p = (uchar*)&num;
    vector<uchar> res(sizeof(num));
    for(int i = 0; i < res.size(); i++) {
        res[i] = *p;
        p++;
    }
    reverse(res.begin(), res.end());
    return res;
}

template<typename _cls, typename S,typename T>
vector<T> apply(const vector<S>& vec, _cls *obj,T (_cls::*fun)(S)) {
    vector<T> res(vec.size());
    for(int i = 0; i < res.size(); i++) {
        S x = vec[i];
        res[i] = (obj->*fun)(x);
    }
    return res;
}

class BigNum
{
public:
    BigNum(const string &str) {

    }
    BigNum(int len) {
        num.resize(len );
    }
    int size() const {
        return num.size();
    }
protected:
    vector<int> num;
    BigNum operator+(const BigNum &other ) const {
        BigNum res(max(other.size(), this->size()) + 1);
        int len = max(other.size(), this->size());
        int carry = 0;
        for(int i = 0; i < len; i++) {
            int cur = carry ;
            cur += i >= other.size() ? 0 : other.num[i];
            cur += i >= num.size() ? 0 : num[i];
            res.num[i] = cur & 0xff;
            carry = cur >> 8;
        }
        res.num[len] = carry;
        len = num.size() - 1;
        while(len >= 0 and res.num[len] == 0) len--;
        res.num.resize(len+1);
        return res;
    }
    BigNum operator-(const BigNum &other ) const {
        assert(this->size() > other.size() );
        int len = this->size() ;
        BigNum res(len);
        uint carry = 0;
        for(int i = 0; i < len; i++) {
            uint cur = num[i] - carry;
            cur -= i >= other.size() ? 0 : other.num[i];
            res.num[i] = (uchar)cur & 0xff;
            carry  = ( cur >> 8 ) & 0x1;
        }
        res.num[len] -= carry;
        len = res.size() ;
        while(len >= 0 and res.num[len] == 0) len--;
        res.resize(len+1);
        return  res;
    }
    BigNum operator* (const BigNum &other) const{
        BigNum res(other.size() + this->size());
        int carry = 0;
        for(int i = 0; i < num.size(); i++) {
            carry  = 0;
            int cur = 0;
            for(int j = 0; j < other.size(); j++) {
                cur = num[i] * other.num[j] + res.num[i+j] + carry;
                res.num[i+j] = carry  & 0xff;
                carry  >>= 8;
            }
            res.num[num.size()+other.size()-1] += carry;
        }
        int len = res.size() ;
        while(len >= 0 and res.num[len] == 0) len --;
        res.resize(len+1);
        return res;
    }
    BigNum operator / (const BigNum &other) const {

    }
    BigNum operator % (const BigNum &other) const {

    }

};

#endif