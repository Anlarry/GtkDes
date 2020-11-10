//
// Created by wind on 2020/11/10.
//

#ifndef MYPROJECT_BUTTON_H
#define MYPROJECT_BUTTON_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QList>
#include <QQmlProperty>
#include <QQuickItem>
#include <Algorithm.h>

class Button : public QObject {
    Q_OBJECT
public slots:
    void GenKey(const QVariant  &window);
    void Encrypt(const QVariant &window);
    void Decrypt(const QVariant &window);

protected:
    BigNum e;
    BigNum d;
    BigNum N;
};


#endif //MYPROJECT_BUTTON_H
