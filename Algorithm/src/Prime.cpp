#include <Prime.h>
#include <time.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
using namespace std;

#define SIZE 17

BigNum Prime::Rand(int len, const BigNum &mod) {    
    BigNum res;
    res.resize(len);
    for(int i = 0; i < len; i++) {
        res[i] = rand() % 256;
    }
    res.removeLeadZero();
    res = res % mod;
    return res;
}

BigNum Prime::RandOdd(int len) {
    BigNum res ;
    res.resize(len);
    for(int i = 0; i < len-1; i++) {
        res[i] = rand() % 256;
    }
    res[len-1] = 128 + rand() % 128;
    if((res[0] & 1 )== 0) {
        res[0] += 1;
    }
    return res;
}

bool Prime::MillerRabin(const BigNum &n) {
    BigNum b, m, v, tmp;
    BigNum j = 0;
    m = n - 1;
    while((m[0] & 0x1 )== 0) {
        j = j + 1;
        m = m / 2;
    }
    b = Rand(SIZE, n-3) + 2;
    v = b.powerMod(m, n);
    if(v == 1) return true;
    BigNum i = 1;
    tmp = n - 1;
    while(v < tmp) {
        if(i == j) return false;
        v = v * v % n;
        i = i + 1;
    }
    return true;
}

bool Prime::MillerRabin(const BigNum &num, int loop) {
    for(int i = 0;  i < loop; i++) {
        if(!MillerRabin(num)) return false;
    }
    return true;
    
}
    // -------------------------

bool Prime::MillerRabinCertainTest(BigNum p)
{
    vector<BigNum> test({2, 3, 5, 233, 331});
    if(p < 2) return false;
    if(!(p == 2) and (p[0] & 1) == 0) {
        return false;
    }
    BigNum s = p - 1;
    while(!(s[0] & 1)) {
        s = s / 2;
    }
    for(int i = 0; i < 5; i++) {
        if(p == test[i]) {
            return 1;
        }
        BigNum t = s, m = test[i].powerMod(s, p);
        while(!(t == p-1) and !(m == 1) and !(m == p-1)) {
            m = m * m % p;
            t = t * 2;
        }
        if(!(m == p-1) and !(t[0] & 1)) {
            return false;
        }
    }
    return true;
}

void Prime::multiThreadMR(mutex &mu,  bool &ok, BigNum &result, int byte, Prime::Option opt) {
    BigNum res = RandOdd(byte);
    int loop = 20;
    while(1) {
        mu.lock() ;
        if(ok) {
            cout << "down by other" << "\n";
            mu.unlock() ;
            return ;
        }
        mu.unlock();
        res = RandOdd(byte);
        bool cur;
        if(opt == Prime::Option::MR_Certain) {
            cur = MillerRabinCertainTest(res);
        }
        else if (opt == Prime::Option::MR_Random){
            cur = MillerRabin(res, loop);
        }
        else {
            printf("\033[1;32mErr:genPrime Option error\033[0m");
            assert(false);
            return ;
        }
        if(cur) {
            mu.lock();
            if(!ok) {
                ok = cur;
                result = res;
                cout << "done" << "\n";
            }
            else {
                cout << "down by other" << "\n";
//                cout << res << "\n";
            }
            mu.unlock();
            return ;
        }
    }
}


BigNum Prime::genPrime(int byte, Prime::Option opt) {
    mutex mu;
    condition_variable cond;
    bool ok = false;
    BigNum res;
    int thread_num = 7;
    thread thread_arr[thread_num];
    for(int i = 0; i < thread_num; i++) {
        thread_arr[i] = thread(&Prime::multiThreadMR, this,
                               ref(mu), ref(ok), ref(res),
                               byte, opt);
    }
    for(int i = 0; i < thread_num; i++) {
        thread_arr[i].join();
    }
    return res;
}