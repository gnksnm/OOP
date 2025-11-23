#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "complex.h"
#include "polynom.h"

class UdpServer : public QObject {
    Q_OBJECT
public:
    explicit UdpServer(QObject*parent=nullptr);
    void start(quint16 port);
private slots:
    void readyRead();
private:
    QUdpSocket socket;
    void processMessage(const QJsonObject &msg, QHostAddress sender, quint16 senderPort);
    QJsonObject polynomToJson(const Polynom &p);
    Polynom jsonToPolynom(const QJsonObject &o);
};