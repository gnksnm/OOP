#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>

class UdpClient : public QObject {
    Q_OBJECT
public:
    explicit UdpClient(QObject*parent=nullptr);
    void start(const QHostAddress &host, quint16 port);
    void sendJson(const QJsonObject &obj);
signals:
    void received(const QJsonObject &obj);
private slots:
    void readyRead();
private:
    QUdpSocket socket;
    QHostAddress hostAddr; quint16 hostPort=0;
};