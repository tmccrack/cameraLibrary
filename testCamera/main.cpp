#include <QCoreApplication>
#include "cameralibrary.h"
#include <iostream>

bool GetInput(long *buffer);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    long *buffer = new long[262144];
    initializeCameraLV();

    while(GetInput(buffer))
    {

    }
    shutdownCameraLV();


    printf("Shutting down\n\n");
    return a.exec();
}



bool GetInput(long *buffer)
{
    int i_input;

    printf("\n");
    printf("Acquire full images - 1\n");
    printf("Acquire sub frame image - 2\n");
    printf("Acquire closed loop - 3\n");
    printf("Retrieve current image buffer - 4\n");
    printf("Abort acquistion - 5\n");
    printf("Send data over socket - 6\n");
    printf("Shutdown camera - Any other key\n");
    std::cin >> i_input;
    printf("\n\n");

    /*
     * Full frame
     */
    if(i_input == 1)
    {
       acquireFullFrameLV(1.0);
       return TRUE;
    }

    /*
     * Sub frame
     */
    else if(i_input == 2)
    {
        setFrameSizeLV(240, 272, 240, 273, 1, 1);
        acquireSubFrameLV(0.10);
        return TRUE;
    }

    /*
     * Closed loop
     */
    else if(i_input == 3)
    {
        setFrameSizeLV(240, 272, 240, 273, 1, 1);
        acquireClosedLoopLV(0.000);
        return TRUE;
    }

    /*
     * Get image buffer
     */
    else if (i_input == 4)
    {
        getCameraDataLV(buffer);
        getCameraDataLV(buffer);
        for (int i = 0; i < 200; i++)
        {
            printf("%i ", buffer[i]);
        }
        printf("\n");
        return TRUE;
    }

    /*
     * Abort
     */
    else if (i_input == 5)
    {
        abortAcquisitionLV();
        return TRUE;
    }

    /*
     * Socket tests, toggle a bits
     */
    else if (i_input == 6)
    {
        int i = 0;
        bool a = false;
        SocketClient *sock = new SocketClient;
        sock->openConnection();
        while (i<10000)
        {
            if (a)
            {
                sock->sendData(5.0000, 5.0000);
                a = false;
            }
            else
            {
                sock->sendData(0.0000, 0.0000);
                a = true;
            }
            i++;
        }
        sock->sendData(0.0000, 0.0000);
        sock->closeConnection();
        delete sock;
        return true;
    }



    else    return FALSE;
}
