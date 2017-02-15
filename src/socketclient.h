#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QtCore/QtCore>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QList>

class SocketClient
{
public:
    SocketClient();
    SocketClient(int pport, std::string hhost);
    ~SocketClient();
    bool openConnection(std::string conn_type);
    void reopenConnection();
    void sendData(float x, float y);
    void getData(float *x, float *y);
    void closeConnection();
    bool isConnected();

private:
    QTcpSocket *socket;
    QByteArray *data;
    QString host;
    int port;
    bool connected;

};

#endif // SOCKETCLIENT_H
