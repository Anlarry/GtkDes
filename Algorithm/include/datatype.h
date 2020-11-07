#ifndef __DATATYPE_H
#define __DATATYPE_H

typedef  unsigned char uchar ;
typedef  unsigned int  uint  ;

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
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
    BigNum() : BigNum(0) {
    }
    BigNum(const vector<int> &num) {
        this->num = vector<int>(num);
        this->removeLeadZero();
    }
    BigNum(int x) {
//        num.resize(len );
        num.clear();
        while(x) {
            num.push_back(x & 0xff);
            x >>= 8;
        }
    }
    int size() const {
        return num.size();
    }
    friend ostream &operator << (ostream& os, const BigNum& num) {
        for(int i = 0;  i < num.num.size(); i++) {
            os << num.num[i] << " ";
        }
        return os;
    }
    BigNum operator+(const BigNum &other ) const {
        // BigNum res(max(other.size(), this->size()) + 1);
        BigNum res;
        res.resize(max(other.size(), this->size()) + 1);
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
        len = res.size() - 1;
        while(len >= 0 and res.num[len] == 0) len--;
        res.num.resize(len+1);
        return res;
    }
    BigNum operator-(const BigNum &other ) const {
        assert(this->size() >= other.size() );
        int len = this->size() ;
        // BigNum res(len);
        BigNum res;
        res.resize(len);
        int carry = 0;
        for(int i = 0; i < len; i++) {
            int cur = num[i] - carry;
            cur -= i >= other.size() ? 0 : other.num[i];
            res.num[i] = cur & 0xff;
            carry  = (cur >> 8) & 0x1;
        }
        res.num[len] -= carry;
        res.removeLeadZero();
        return  res;
    }
    BigNum operator* (const BigNum &other) const{
        // BigNum res(other.size() + this->size());
        BigNum res;
        res.resize(other.size() + this->size() );
        int carry = 0;
        for(int i = 0; i < num.size(); i++) {
            carry  = 0;
            int cur = 0;
            for(int j = 0; j < other.size(); j++) {
                cur = num[i] * other.num[j] + res.num[i+j] + carry;
                res.num[i+j] = cur  & 0xff;
                carry = cur >> 8;
            }
            res.num[i+other.size()] += carry;
        }
        int len = res.size()-1;
        while(len >= 0 and res.num[len] == 0) len --;
        res.num.resize(len+1);
        return res;
    }
    BigNum operator / ( BigNum other) const {
        if(this->size() < other.size()) return BigNum(0);
        // BigNum res(num.size() - other.size() + 1);
        BigNum res;
        res.resize(num.size() - other.size() + 1);
        BigNum sub;
        BigNum Num = *this;
        int len = num.size() - other.size();
        for(int i = len; i >= 0; i--) {
            sub = other.ShiftLeft(i);
            while(sub <= Num) {
                Num = Num - sub;
                res.num[i] ++;
            }
        }
        res.removeLeadZero();
        return res;
    }
    BigNum operator % ( BigNum other) const {
        if(this->size() < other.size()) return *this;
        // BigNum res(num.size() - other.size() + 1);
        BigNum res;
        res.resize(num.size() - other.size() + 1);
        BigNum sub(0);
        BigNum Num = *this;
        int len = num.size() - other.size();
        for(int i = len; i >= 0; i--) {
            sub = other.ShiftLeft(i);
            while(sub <= Num) {
                Num = Num - sub;
                res.num[i] ++;
            }
        }
        Num.removeLeadZero();
        return Num;
    }
    BigNum ShiftLeft(int k) const {
        // BigNum res (this->size() + k);
        BigNum res;
        res.resize(this->size() + k);
        for(int i = 0; i < this->size(); i++) {
            res.num[i+k] = num[i];
        }
        return res;
    }
    bool operator <= (const BigNum &other) const {
        if(this->size()  > other.size()) {
            return false;
        }
        if(this->size() < other.size()) {
            return true;
        }
        for(int i = other.size()-1; i >= 0; i--) {
            if(num[i] > other.num[i]) return false;
            else if(num[i] < other.num[i]) {
                return true;
            }
        }
        return true;
    }
    bool operator < (const BigNum &other) const {
        if(this->size()  > other.size()) {
            return false;
        }
        if(this->size() < other.size()) {
            return true;
        }
        for(int i = other.size()-1; i >= 0; i--) {
            if(num[i] > other.num[i]) return false;
            else if(num[i] < other.num[i]) {
                return true;
            }
        }
        return false;
    }
    bool isZero() const {
        return num.size() == 0;
    }
    bool operator == (const BigNum& other) const {
        if(this->size() != other.size()) {
            return false;
        }
        for(int i = 0; i < other.size(); i++) {
            if(num[i] != other.num[i]) return false;
        }
        return true;
    }
    friend void ex_gcd(BigNum a, BigNum b, BigNum &x, BigNum &y, const BigNum &mod) {
        if(b == 0) {
            x = BigNum(vector<int>({1}));
            y = BigNum(vector<int>({0}));
            return ;
        }
        ex_gcd(b, a % b, x, y, mod);
        auto tmp = x;
        x = y;
        auto t  = a / b;
        y = (tmp  + mod - ((a / b) * y) % mod) % mod;
    }
    friend BigNum gcd(BigNum a, BigNum b) {
        if(b == 0) {
            return a;
        }
        return gcd(b, a % b);
    }
    BigNum ModInv(const BigNum &mod) const {
        BigNum s, t;
        ex_gcd(*this, mod, s, t, mod);
        return s;        
    }
    BigNum powerMod(BigNum k, const BigNum &mod ) const {
        BigNum res = BigNum(vector<int>({1}));
        BigNum x = *this;
        while(1) {
            if(k[0] & 1)
                res = res * x % mod;
            x = x * x % mod;
            k = k / 2;
            if(k == 0) break;
        }
        return res;
    }
    void removeLeadZero() {
        int len = this->size()-1;
        while(len >= 0 and num[len] == 0) {
            len --;
        }
        num.resize(len+1);
    }

    int & operator[](int x) {
        return num[x];
    }
    void resize(int len) {
        num.resize(len);
    }
protected:
    vector<int> num;
};

#endif