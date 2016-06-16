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

class CameraThread : public QThread
{
    Q_OBJECT
public:
    CameraThread(QObject *parent = 0);
    ~CameraThread();

    void startCameraThread(void *eventHandle);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    QMutex mutex;
    QWaitCondition condition;
    HANDLE camEvent;
    bool abort;

private slots:
    void camEventSignaled(HANDLE h);



};

#endif // SOCKETCLIENT_H
