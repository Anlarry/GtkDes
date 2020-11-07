//
// Created by wind on 2020/11/5.
//

#ifndef MYPROJECT_PRIME_H
#define MYPROJECT_PRIME_H

#include <datatype.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
using namespace std;

class Prime
{
public:
    enum Option {MR_Random, MR_Certain};
    bool MillerRabin(const  BigNum &num, int loop);
    bool MillerRabinCertainTest( BigNum p);
    BigNum Rand(int len, const BigNum &mod);
    BigNum genPrime(int byte, Prime::Option opt);
    BigNum RandOdd(int len);
protected:
    bool MillerRabin(const BigNum &num) ;
    void multiThreadMR(mutex &mu,  bool &ok, BigNum &res, int byte, Prime::Option opt);
};

#endif //MYPROJECT_PRIME_H
