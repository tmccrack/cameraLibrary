#include <iostream>
#include "socketclient.h"


/*
 * Constructor for socket client class
 */
SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;
    //data = new QByteArray;
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
        data->append(QByteArray::number(x, 'f'));
        data->append(";");
        data->append(QByteArray::number(y, 'f'));
        socket->write(QByteArray::number(data->size()), 2);
        socket->flush();
        socket->waitForBytesWritten(1);
        socket->write(*data, data->size());
        socket->flush();
        data->clear();
        //printf("data sent\n");
        //
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
