#include <QCoreApplication>
#include "cameralibrary.h"

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
    printf("Shutdown camera - Any other key\n");
    std::cin >> i_input;
    printf("\n\n");

    if(i_input == 1)
    {
       acquireFullFrameLV(1.0);
       return TRUE;
    }
    else if(i_input == 2)
    {
        setFrameSizeLV(240, 272, 240, 273, 1, 1);
        acquireSubFrameLV(0.10);
        return TRUE;
    }
    else if (i_input == 3)
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
    else if (i_input == 4)
    {
        abortAcquisitionLV();
        return TRUE;
    }
    else    return FALSE;
}

//void DisplayImage(WORD * _lp_data, int _i_xPx, int _i_yPx)
//{
//  //Using third party Package 'CIm'g

//  //Create a CImg Object the width and height of the sensor, 2 Dimensional, one color chanel per pixel, pixels defaulted to 0(black)
//  CImg<float> img(_i_xPx,_i_yPx,1,1,0);

//  //fill the img window in black
//  img.fill(0);

//  int i_counter = 0;
//  for(int y = 0; y < _i_yPx;++y)
//  {
//    for(int x = 0; x < _i_xPx; x++)
//    {
//      unsigned char _ucp_grayScale[] = {255,255,255};         //define the colour white
//      float f_intensity = _lp_data[i_counter]/65536;   //Calculate pixel intensity
//      img.draw_point(x,y,_ucp_grayScale,f_intensity);           //Draw Pixel
//      i_counter++;
//    }
//  }
//  img.display("Image");
//}
