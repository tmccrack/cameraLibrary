#include <QCoreApplication>
#include "socketclient.h"
#include <iostream>

SocketClient::SocketClient()
{

}

void SocketClient::Connect()
{

    QString cRIO = "172.29.46.109";
    QString tester = "localhost";

    //connecte
    socket = new QTcpSocket;
    socket->connectToHost(cRIO, 6667);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected";
        this->connected = true;
    }
    else
    {
        qDebug() << "Cannot connect";
    }

    // sent data

    // got data

    // closed connection

}

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

void SocketClient::Close()
{
    if(this->connected)
    {
        socket->close();
        qDebug() << "Connection closed";
    }

}


//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    SocketClient client;
//    client.Connect();
//    client.sendData(float(1.333333333), float(5.66346514565));
//    client.Close();

//    return a.exec();
//}

