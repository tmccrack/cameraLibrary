#include <iostream>
#include "socketclient.h"


/*
 * Constructor for socket client class
 */
SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    host = "172.29.46.109";
    port = 6666;
    connected = false;
}


/*
 * Destructor for the socket client class
 */
SocketClient::~SocketClient()
{

}


/*
 * Function to connect to specified client
 */
void SocketClient::openConnection()
{
    //connect
    //socket = new QTcpSocket(this);

    socket->connectToHost(host, port);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected";
        connected = true;
    }
    else
    {
        qDebug() << "Cannot connect";
    }
}


/*
 * Function to send data to specified client
 */
void SocketClient::sendData(float x, float y)
{
    if(connected)
    {
        QByteArray data = QByteArray::number(x) + QByteArray::QByteArray(";") + QByteArray::number(y);
        socket->write(data, 15);
        socket->flush();
        //printf("data sent\n");
        //socket->waitForBytesWritten(1);
        //socket->waitForReadyRead(3000);
        //qDebug() << socket->readAll();
    }
    else
    {
        qDebug() << "Not connected";
    }

}


/*
 * Close connection with client
 */
void SocketClient::closeConnection()
{
    if(connected)
    {
        // Close frees ports on other side, disconnectFrom waits for
        //socket->disconnectFromHost();
        socket->close();
        connected = false;
        qDebug() << "Connection closed";
    }

}

/*
 * Return connection state
 */
bool SocketClient::isConnected()
{
    // Multiple states possible, only care about if connected
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        return true;
    }
    else
    {
        connected = false;
        return false;
    }
}
