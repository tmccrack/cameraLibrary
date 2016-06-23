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
    printf("Retrieve current image buffer - 3\n");
    printf("Abort acquistion - 4\n");
    printf("Check if running - 5\n");
    printf("Send data - 6\n");
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
     * Sub frame
     */
    else if(i_input == 3)
    {
        setFrameSizeLV(240, 272, 240, 273, 1, 1);
        acquireClosedLoopLV(0.001);
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

    else    return FALSE;
}
