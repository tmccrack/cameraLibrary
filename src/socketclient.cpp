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

/*
 * Camera thread constructor
 */
CameraThread::CameraThread(QObject *parent) : QThread(parent)
{
    // Create win32 event handle
    this->camEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  // Create win32 event handle
    this->abort = false;
}
\

/*
 * Camera thread destructor
 */
CameraThread::~CameraThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();

    wait();  // Wait for run() to finish
    destroy(camEvent);  // Free event handle

}

/*
 * Main worker thread for camera acquisition.
 * Runs until abort is set.
 * Create event handle and wait for driver to set event.
 * Reset event and read camera data into mutex protected image buffer
 * Copy image into mutex protected data buffer for external logging and display
 * TODO: compute centroid and send signal out
 */
void CameraThread::run()
{
    // Create notifier and attach notifier to event handler
    notifier = new QWinEventNotifier(this->camEvent);
    connect(notifier, SIGNAL(activated(HANDLE)), SLOT(camEventSignaled(HANDLE)));

    // Pass Andor API event handle and start acquistion
    ui_error = SetDriverEvent(this->camEvent);
    // checkError(ui_error);
    ui_error = StartAcquisition();
    // checkError(ui_error);

    while (!this->abort)
    {

    }
}


/*
 * Andor event handler
 */
void CameraThread::camEventSignaled(HANDLE h)
{
    // Event received
    // Get image data
    mutex.lock();
    GetMostRecentImage(camData, imageDim.size);
    std::copy(camData, camData + imageDim.size, imageBuffer);

    // Centroid
    mutex.unlock();

    // Send update


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

