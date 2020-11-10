#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QList>
#include <QQmlProperty>
#include <iostream>
#include <Button.h>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


//    QQmlComponent component(&engine, QUrl::fromLocalFile("./main.qml"));
    QList<QObject*> rootObj = engine.rootObjects();
    QObject* root = rootObj[0];
    QObject* but = root->findChild<QObject*>("GenKey");

//    QObject* P = root->findChild<QObject*>("PField");
//    QQmlProperty::write(P, "text", "50");
//    QObject* ret = root->findChildren<QObject>("")
//    for(int i = 0; i < rootObj.at(0)->)

    Button butctl;
    QObject::connect(but, SIGNAL(qmlSignal(QVariant)), &butctl, SLOT(GenKey(QVariant)));
    but = root->findChild<QObject*>("Encypt");
    QObject::connect(but, SIGNAL(qmlSignal(QVariant)), &butctl, SLOT(Encrypt(QVariant)));
    but = root->findChild<QObject*>("Decypt");
    QObject::connect(but, SIGNAL(qmlSignal(QVariant)), &butctl, SLOT(Decrypt(QVariant)));
//    QObject::connect(but, &CInter)
    return app.exec();

}
