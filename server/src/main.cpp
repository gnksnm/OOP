#include <QCoreApplication>
#include "udpserver.h"
int main(int argc,char**argv){ QCoreApplication a(argc,argv); UdpServer server; server.start(45454); return a.exec(); }
