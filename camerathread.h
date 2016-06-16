#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QtCore>
#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QWinEventNotifier>
#include <windows.h>
#include "ATMCD32D.h"

class CameraThread : public QThread
{
    Q_OBJECT
public:
    CameraThread(QObject *parent = 0);
    ~CameraThread();
    void startCameraThread(int imageSize);
    void abortCameraThread();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool checkError(unsigned int _ui_error, const char* _cp_func);
    bool b_gblerrorFlag;
    unsigned int and_error; // Andor error
    DWORD win_error;  // Windows event error
    static long *imageBuffer;
    QMutex mutex;
    QWaitCondition condition;
    HANDLE camEvent;
    QWinEventNotifier *notifier;
    bool abort;
    int imageSize;

private slots:
    void camEventSignaled(HANDLE h);
};

static long *camData2;

#endif // CAMERATHREAD_H
