#include <QCoreApplication>
#include "cameralibrary.h"
#include <iostream>

bool GetInput(long *buffer);
void Init();
bool Inited = FALSE;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    long *buffer = new long[262144];

    while(GetInput(buffer))
    {

    }
    shutdownCameraLV();


    printf("Shutting down\n\n");
    return a.exec();
}

void Init()
{
    printf("Initializing...\n");
    initializeCameraLV();
    setTemperatureLV(0);
    Inited = TRUE;
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
    printf("Get temperature - 7\n");
    printf("Shutdown camera - Any other key\n");
    std::cin >> i_input;
    printf("\n\n");

    /*
     * Full frame
     */
    if(i_input == 1)
    {
        if (Inited)
        {
            acquireFullFrameLV(1.0);
            return TRUE;
        }
        else
        {
            Init();
            acquireFullFrameLV(1.0);
            return TRUE;
        }
    }

    /*
     * Sub frame
     */
    else if(i_input == 2)
    {
        if (Inited)
        {
            setFrameSizeLV(240, 272, 240, 273, 1, 1);
            acquireSubFrameLV(0.10);
            return TRUE;
        }
        else
        {
            Init();
            setFrameSizeLV(240, 272, 240, 273, 1, 1);
            acquireSubFrameLV(0.10);
            return TRUE;
        }
    }

    /*
     * Closed loop
     */
    else if(i_input == 3)
    {
        if (Inited)
        {
            setFrameSizeLV(240, 272, 240, 273, 1, 1);
            acquireClosedLoopLV(0.000);
            return TRUE;
        }
        else
        {
            Init();
            setFrameSizeLV(240, 272, 240, 273, 1, 1);
            acquireClosedLoopLV(0.000);
            return TRUE;
        }
    }

    /*
     * Get image buffer
     */
    else if (i_input == 4)
    {
        if (Inited)
        {
            getCameraDataLV(buffer);
            for (int i = 0; i < 200; i++)
            {
                printf("%i ", buffer[i]);
            }
            printf("\n");
            return TRUE;
        }
        else
        {
            Init();
            getCameraDataLV(buffer);
            for (int i = 0; i < 200; i++)
            {
                printf("%i ", buffer[i]);
            }
            printf("\n");
            return TRUE;
        }

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
        float x = 0.000;
        float y = 5.000;
        SocketClient *sock = new SocketClient;
        sock->openConnection();
        if (sock->isConnected())
        {
            while (i<10000)
            {
                if (a)
                {
                    sock->sendData(y,y);
                    a = false;
                }
                else
                {
                    sock->sendData(x, x);
                    a = true;
                }
                i++;
            }
            sock->sendData(0.0000, 0.0000);
            sock->closeConnection();
            delete sock;
            return true;
        }
        else printf("Issue connecting");

    }

    else if (i_input == 7)
    {
        int *temp;
        unsigned int *status;
        getTemperatureLV(temp, status);
        printf("Current temperature: %d\n", *temp);
        printf("Temp status flag: %d\n", *status);
        return true;
    }



    else    return false;
}
