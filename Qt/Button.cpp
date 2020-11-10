//
// Created by wind on 2020/11/10.
//

#include "Button.h"
#include <iostream>
#include <QQuickItem>
#include <stdio.h>
#include <Algorithm.h>
#include <assert.h>
using namespace std;

void Button::GenKey(const QVariant &window) {
    qDebug() << window << "\n";
    QObject* win = window.value<QObject*>();
    qDebug() << win << "\n";
    qDebug() << win->property("width").toInt() <<"\n";
    int gen_bit = win->findChild<QObject*>("KeyBits")->property("value").toInt();
    cout << "Bits : " << gen_bit  << "\n";
    Prime prime_cls;
    auto p = prime_cls.genPrime(gen_bit / 8, Prime::Option::MR_Certain);
    auto q = prime_cls.genPrime(gen_bit / 8, Prime::Option::MR_Certain);
    N = p * q;
    cout << "Gen Key: " << "\n";
    cout << p << "\n";
    cout << q << "\n";

    win->findChild<QObject*>("PField")->setProperty("text",
                                                    QString::fromStdString(p.DecimalStr()));
    win->findChild<QObject*>("QField")->setProperty(
        "text", QString::fromStdString(q.DecimalStr())
    );
    win->findChild<QObject*>("NField")->setProperty("text", QString::fromStdString(N.DecimalStr()));
    auto phi = (p - 1) * (q - 1);
    e = prime_cls.Rand(phi.size(), phi);
    while(!(gcd(e, phi) == 1)) {
        e = prime_cls.Rand(phi.size(), phi);
    }
    cout << "Gen 公钥E: " << e << "\n";
    d = e.ModInv(phi);
    cout << "Gen 私钥D: " << d << "\n";
    win->findChild<QObject*> ("EField")->setProperty("text", QString::fromStdString(e.DecimalStr()));
    win->findChild<QObject*>("DField")->setProperty("text", QString::fromStdString(d.DecimalStr()));
    return ;
}

void Button::Encrypt(const QVariant &window) {
    QObject* win = window.value<QObject*>();
    string text = win->findChild<QObject*>("TextField")->property("text").toString().toStdString();
    BigNum t(vector<int>(text.begin(), text.end()));
    if(N < t) {
        printf("\033[1;032mError: Text greator than N\033[0m\n");
        return ;
//        assert(false);
    }
    cout << "Text: " <<  t << "\n";
    BigNum res = t.powerMod(e, N);
    win->findChild<QObject*>("CipherField")->setProperty("text", QString::fromStdString(res.DecimalStr()));
    cout << "Cipher: " << res << "\n";
    return ;
}

void Button::Decrypt(const QVariant &window) {
    QObject* win = window.value<QObject*>();
    string cipher_s = win->findChild<QObject*>("CipherField")->property("text").toString().toStdString();
    BigNum cipher(cipher_s);
    if(N < cipher) {
        printf("\033[1;032mError: cipher greator than N\033[0m\n");
//        assert(false);
        return ;
    }
//    cout << cipher_s  << "\n";
    cout << "Cipher: " << cipher << "\n";
    BigNum text = cipher.powerMod(d, N);
    cout << "Text: " << text << "\n";
    if(text.size() > 0) {
        win->findChild<QObject*>("InvTextField")->setProperty(
                "text",QString::fromStdString(string(&text[0], &text[0]+text.size())));
    }
    // win->findChild<QObject*>("InvTextField")->setProperty("text", QString::fromStdString(text.DecimalStr()));
    return ;
}

