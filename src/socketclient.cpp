#include <iostream>
#include "socketclient.h"


/*
 * Constructor for socket client class
 */
SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;
    data = new QByteArray;
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
void SocketClient::openConnection(QString conn_type)
{
    //connect
    socket->connectToHost(host, port);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected";
        QByteArray temp;
        temp.append(conn_type);
        socket->write(QByteArray::number(temp.size()), 2);
        socket->flush();
        socket->waitForBytesWritten(100);
        socket->write(temp, temp.size());
        socket->flush();
        socket->waitForBytesWritten(1000);
        connected = true;
    }
    else
    {
        qDebug() << "Cannot connect";
    }
}


void SocketClient::reopenConnection()
{
    //connect
    socket->connectToHost(host, port);
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
        socket->write(QByteArray::number(data->size()));
        socket->flush();
        socket->waitForBytesWritten(1);
        socket->write(*data, data->size());
        socket->flush();
        data->clear();
        //qDebug() << "Data sent";
    }
    else
    {
        socket->close();  // Cleanup
        qDebug() << "Not connected";
        connected = false;
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
