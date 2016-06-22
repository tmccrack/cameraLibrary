#include <QCoreApplication>
#include "socketclient.h"
#include <iostream>


/*
 * Constructor for socket client class
 */
SocketClient::SocketClient(QString host, int port)
{
    this->socket = new QTcpSocket;
    this->port = port;
    this->host = host;
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
void SocketClient::Connect()
{
    //connect

    socket->connectToHost(host, port);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected";
        this->connected = true;
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
    if(this->connected)
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
void SocketClient::Close()
{
    if(this->connected)
    {
        socket->close();
        qDebug() << "Connection closed";
    }

}
