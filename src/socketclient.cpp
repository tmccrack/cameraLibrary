#include <iostream>
#include "socketclient.h"
using namespace std;

/*
 * Constructor for socket client class
 */
SocketClient::SocketClient()
{
    host = "localhost";
    port = 6666;
    socket = new QTcpSocket;
    data = new QByteArray;
    connected = false;
    qDebug() << "Client created";
}

SocketClient::SocketClient(int pport, string hhost)
{
    host = QString::fromStdString(hhost);
    port = pport;
    socket = new QTcpSocket;
    data = new QByteArray;
    connected = false;
    qDebug() << "Client created";
}


/*
 * Destructor for the socket client class
 */
SocketClient::~SocketClient()
{
    if (socket) delete socket;
    if (data) delete data;
}


/*
 * Function to connect to specified client
 */
bool SocketClient::openConnection(string conn_type)
{
    //connect
    socket->connectToHost(host, port);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected";
        QByteArray temp;
        temp.append(QString::fromStdString(conn_type));
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
    return connected;
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

void SocketClient::getData(float *x, float *y)
{
    if(connected)
    {
        bool *ok;
        QByteArray data;
        if (socket->waitForReadyRead())
        {
            socket->readAll();
            if (socket->waitForReadyRead())
            {
                data = socket->readAll();
                QList<QByteArray> data_list = data.split(';');
                *x = data_list[0].toFloat(ok);
                if(!ok) qDebug() << "Value conversion error: " << data_list[0];

                *y = data_list[1].toFloat(ok);
                if(!ok) qDebug() << "Value conversion error: " << data_list[1];
            }
            else qDebug() << "Read error";
        }
        else qDebug() << "Handshake failed";
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
