#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QtCore>
#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <QWaitCondition>
#include <windows.h>
#include "ATMCD32D.h"

class SocketClient
{
public:
    SocketClient();
    void Connect();
    void sendData(float x, float y);
    void Close();

private:
    QTcpSocket *socket;
    bool connected = false;

};

#endif // SOCKETCLIENT_H
