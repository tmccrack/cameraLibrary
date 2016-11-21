#include <QCoreApplication>
#include <iostream>
#include "camera.h"

using namespace std;

bool GetInput(long *buffer, float *control_buffer, Camera *cam);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    long *buffer = new long[262144];
    float *control_buffer = new float[6];
    bool sim_cam;

    cout << "Enter camera name: ";
    cin.clear();
    string cam_name, temp;
    getline(cin, cam_name);

    cout << "Fake camera? [y/n] ";
    cin.clear();
    getline(cin, temp);
    if (temp == "y")
    {
        cout << "Setting up fake camera " << cam_name << endl;
        sim_cam = FALSE;
    }
    else
    {
        cout << "Setting up real camera " << cam_name << endl;
        sim_cam = TRUE;
    }
    static Camera camera(0, QString::fromStdString(cam_name), sim_cam);

    while(GetInput(buffer, control_buffer, &camera))
    {

    }

    camera.shutdownCamera();

    if (buffer) delete[] buffer;
    if (control_buffer) delete[] control_buffer;
    return a.exec();
}


bool GetInput(long *buffer, float *control_buffer, Camera *cam)
{
    int i_input;
    ImageDimension img;
    ExposureProperties exp;
    TimingProperties tim;
    ReadProperties read;
    ShutterProperties shut;

    string temp;
    printf("\n");
    printf("Start camera - 1\n");
    printf("Stop camera - 2\n");
    printf("Retrieve image - 3\n");
    printf("Image dimension - 4\n");
    printf("Exposure time - 5\n");
    printf("Array timing - 6\n");
    printf("Shutter parameters - 7\n");
    printf("Shutdown camera - Any other key\n");

    getline(cin, temp);
    if (temp.empty()) return true;
    else i_input = atoi(temp.c_str());
    cin.clear();

    /*
     * Start
     */
    if(i_input == 1)
    {
        cam->startCamera();
        return true;
     }

    /*
     * Stop
     */
    else if(i_input == 2)
    {
        cam->stopCamera();
        return true;
    }

    /*
     * Get image
     */
    else if(i_input == 3)
    {
        cam->getCameraData(buffer);
        for (int i = 0; i < 200; i++)
        {
            printf("%i ", buffer[i]);
        }
        return true;
    }

    /*
     * Image dimesion
     */
    else if (i_input == 4)
    {
        img = cam->getImageDims();
        cout << "Enter image dimensions." << endl;

        cout << "X dim: ";
        cin.clear();
        getline(cin, temp);
        if (temp.empty()) img.h_end = 512;
        else img.h_end = atoi(temp.c_str());
        cout << "X dim set to " << img.h_end << endl;


        cout << "Y dim: ";
        cin.clear();
        getline(cin, temp);
        if (temp.empty()) img.v_end = 512;
        else img.v_end = atoi(temp.c_str());
        cout << "Y dim set to " << img.v_end << endl << endl;

        cam->setImageDims(img);
        img = cam->getImageDims();
        cout <<"X: " <<img.h_dim << "\tY: " <<
                img.v_dim << "\tTotal: " << img.size << endl << endl;
        return true;
    }

    /*
     * Exposure parameters
     */
    else if (i_input == 5)
    {
        cout << "Enter exposure time." << endl;
        cin.clear();
        getline(cin, temp);
        if (temp.empty()) exp.exp_time = 0.001;
        else exp.exp_time = strtof(temp.c_str(), NULL);

        cout << "Enter EM gain." << endl;
        cin.clear();
        getline(cin, temp);
        if (temp.empty()) exp.em_gain = 1;
        else exp.em_gain = atoi(temp.c_str());

        cam->setExposureParams(exp);
        exp = cam->getExposureParams();
        cout << "Exposure time: " << exp.exp_time << "\tEM gain: " << exp.em_gain << endl << endl;
        return true;
    }

    /*
     * Timing parameters
     */
    else if (i_input == 6)
    {
        cout << "Press enter for default timing params." << endl
             << "Press any other key for custom timing params." << endl;
        cin.clear();
        getline(cin, temp);
        if (!temp.empty())
        {
            cout << "Enter horizontal then vertical shift speeds." << endl;
            cin.clear();
            cin >> tim.h_shift;
            cin >> tim.v_shift;

            cout << "Enter number of images in DMA buffer." << endl;
            cin.clear();
            cin >> tim.dma_images;

            cout << "Enter time to accumulate DMA buffer [ms]." << endl;
            cin >> tim.dma_accum_time;

        }
        else
        {
            tim.h_shift = 0;
            tim.v_shift = 0;
            tim.dma_images = 1;
            tim.dma_accum_time = 0.001;
        }

        cam->setTimingParams(tim);
        tim = cam->getTimingParams();
        cout << "H shift: " << tim.h_shift << "\tV shift: " << tim.v_shift << endl
             << "# DMA images: " << tim.dma_images << "\tDMA accum: " << tim.dma_accum_time;
        return true;
    }

    /*
     * Shutter parameters
     */
    else if (i_input==7)
    {
        cout << "Press enter for default shutter params" << endl
             << "Enter 0 for automatic, 1 to open, 2 to close." << endl;
        cin.clear();
        getline(cin, temp);
        if (temp.empty()) shut.mode = 0;
        else shut.mode = atoi(temp.c_str());

        shut.type = 1;
        shut.open_time = 30;  // [ms]
        shut.close_time = 30;  // [ms]

        cam->setShutterParams(shut);
        shut = cam->getShutterParams();
        cout << "TTL level: " << shut.type << "\tMode: " << shut.mode << endl;

        return true;
    }

    /*
     * Nothing entered, quit
     */

    return false;
}
