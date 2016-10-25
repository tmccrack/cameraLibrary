#include <QCoreApplication>
#include "cameralibrary.h"
#include <iostream>

bool GetInput(long *buffer, float *contBuffer);
void copyArray(float *buff);
void Init();
bool Inited = false;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    long *buffer = new long[262144];
    float *contBuffer = new float[6];

    Init();
    while(GetInput(buffer, contBuffer))
    {

    }

    printf("Shutting down\n\n");

    if (buffer) delete[] buffer;
    if (contBuffer) delete[] contBuffer;
    return a.exec();
}

void Init()
{
    printf("Initializing...\n");
    initializeCameraLV();
    //setTemperatureLV(0);
    Inited = true;
}

bool GetInput(long *buffer, float *contBuffer)
{
    int i_input;
    printf("\n");
    printf("Acquire full images - 1\n");
    printf("Acquire sub frame image - 2\n");
    printf("Acquire closed loop - 3\n");
    printf("Retrieve current image buffer - 4\n");
    printf("Abort acquistion - 5\n");
    printf("Move mirror - 6\n");
    printf("Get control values - 7\n");
    printf("Shutdown camera - Any other key\n");
    char buf[BUFSIZ];
    std::cin >> i_input;
    bool bad = std::cin.fail();
    if (bad) i_input = 100;

    /*
     * Full frame
     */
    if(i_input == 1)
    {
        acquireFullFrameLV(1.0);
        return true;
     }

    /*
     * Sub frame
     */
    else if(i_input == 2)
    {
        setFrameSizeLV(240, 273, 240, 273, 1, 1);
        acquireSubFrameLV(0.10);
        return true;
    }

    /*
     * Closed loop
     */
    else if(i_input == 3)
    {
        setFrameSizeLV(185, 217, 216, 248, 1, 1);
        setTargetValueLV(16, 16);
        acquireClosedLoopLV(0.010);
        return true;
    }

    /*
     * Get image buffer
     */
    else if (i_input == 4)
    {
        if (!Inited)    Init();

        getCameraDataLV(buffer);
        for (int i = 0; i < 200; i++)
        {
            printf("%i ", buffer[i]);
        }
        printf("\n");
        return true;
    }

    /*
     * Abort
     */
    else if (i_input == 5)
    {
        abortAcquisitionLV();
        return true;
    }

    /*
     * Socket tests, toggle a bits
     */
    else if (i_input == 6)
    {
        float xV, yV;
        printf("Enter x & y voltages: \n");
        std::cin >> xV;
        std::cin >> yV;
        setMirrorValueLV(xV, yV);
        return true;


//        int i = 0;
//        bool a = false;
//        float x = 0.000;
//        float y = 5.000;
//        SocketClient *sock = new SocketClient;
//        sock->openConnection();
//        if (sock->isConnected())
//        {
//            while (i<10000)
//            {
//                if (a)
//                {
//                    sock->sendData(y,y);
//                    a = false;
//                }
//                else
//                {
//                    sock->sendData(x, x);
//                    a = true;


//                }
//                i++;
//            }
//            sock->sendData(0.0000, 0.0000);

//            sock->closeConnection();
//            delete sock;
//            return true;
//        }
//        else printf("Issue connecting");
//        return true;

    }

    else if (i_input==7)
    {
        getControlValuesLV(contBuffer);
        for (int i = 0; i < 6; i++)
        {
            printf("%f ", contBuffer[i]);
        }
        return true;
    }

    else
    {
        shutdownCameraLV();
        return false;

    }
    return false;
}
