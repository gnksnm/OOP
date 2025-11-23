#include "udpclient.h"
#include <QDebug>
UdpClient::UdpClient(QObject*parent): QObject(parent){
    connect(&socket,&QUdpSocket::readyRead,this,&UdpClient::readyRead);
    socket.bind(QHostAddress::AnyIPv4, 0);
}
void UdpClient::start(const QHostAddress &host, quint16 port){ hostAddr=host; hostPort=port; }
void UdpClient::sendJson(const QJsonObject &obj){
    QJsonDocument doc(obj); QByteArray data = doc.toJson(QJsonDocument::Compact);
    socket.writeDatagram(data, hostAddr, hostPort);
}
void UdpClient::readyRead(){
    while(socket.hasPendingDatagrams()){
        QByteArray datagram; datagram.resize((int)socket.pendingDatagramSize());
        QHostAddress sender; quint16 senderPort;
        socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QJsonParseError err; QJsonDocument doc = QJsonDocument::fromJson(datagram, &err);
        if(err.error!=QJsonParseError::NoError) continue;
        if(!doc.isObject()) continue;
        emit received(doc.object());
    }
}