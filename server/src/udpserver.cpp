#include "udpserver.h"
#include <QDebug>

UdpServer::UdpServer(QObject*parent): QObject(parent){}

void UdpServer::start(quint16 port){
    socket.bind(QHostAddress::AnyIPv4, port);
    connect(&socket, &QUdpSocket::readyRead, this, &UdpServer::readyRead);
    qInfo() << "Server listening on port" << port;
}

void UdpServer::readyRead(){
    while(socket.hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize((int)socket.pendingDatagramSize());
        QHostAddress sender; quint16 senderPort;
        socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(datagram, &err);
        if(err.error!=QJsonParseError::NoError){
            qWarning() << "Failed parse JSON:" << err.errorString();
            continue;
        }
        if(!doc.isObject()) continue;
        QJsonObject obj = doc.object();
        processMessage(obj, sender, senderPort);
    }
}

void UdpServer::processMessage(const QJsonObject &msg, QHostAddress sender, quint16 senderPort){
    QString op = msg.value("op").toString();
    QJsonObject resp;
    if(op=="add" || op=="mul" || op=="derivative" || op=="evaluate"){
        Polynom p = jsonToPolynom(msg.value("polynom").toObject());
        if(op=="add"){
            Polynom other = jsonToPolynom(msg.value("other").toObject());
            Polynom res = Polynom::add(p, other);
            resp = polynomToJson(res); resp["status"]="ok"; resp["type"]="polynom";
        } else if(op=="mul"){
            Polynom other = jsonToPolynom(msg.value("other").toObject());
            Polynom res = Polynom::mul(p, other);
            resp = polynomToJson(res); resp["status"]="ok"; resp["type"]="polynom";
        } else if(op=="derivative"){
            Polynom res = p.derivative();
            resp = polynomToJson(res); resp["status"]="ok"; resp["type"]="polynom";
        } else if(op=="evaluate"){
            QJsonObject xobj = msg.value("x").toObject();
            Complex x{ xobj.value("re").toDouble(), xobj.value("im").toDouble() };
            Complex val = p.evaluate(x);
            resp["status"]="ok"; resp["type"]="value";
            QJsonObject v; v["re"]=val.re; v["im"]=val.im; resp["value"]=v;
        }
    } else {
        resp["status"]="error"; resp["message"]="unknown op";
    }
    QJsonDocument doc(resp);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    socket.writeDatagram(data, sender, senderPort);
}

QJsonObject UdpServer::polynomToJson(const Polynom &p){
    QJsonObject out; QJsonArray arr;
    for(const Complex &c : p.coeff){ QJsonObject o; o["re"]=c.re; o["im"]=c.im; arr.append(o); }
    out["coeff"]=arr; return out;
}

Polynom UdpServer::jsonToPolynom(const QJsonObject &o){
    QJsonArray arr = o.value("coeff").toArray();
    CArray v; v.reserve(arr.size());
    for(const QJsonValue &val : arr){
        QJsonObject c = val.toObject();
        v.push_back(Complex{c.value("re").toDouble(), c.value("im").toDouble()});
    }
    return Polynom(v);
}