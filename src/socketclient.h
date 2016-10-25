#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QtCore>
#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class SocketClient : public QObject
{
    Q_OBJECT

public:
    SocketClient(QObject *parent = 0);
    ~SocketClient();
    void openConnection(QString conn_type);
    void reopenConnection();
    void sendData(float x, float y);
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
