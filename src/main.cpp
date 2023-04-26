/**/



#include "tgaimg.h"
#include <string>

int main()
{
    //Welcome message
    cout << "***************************************************" << endl;
    cout << "*            Welcome to Edge Detective            *" << endl;
    cout << "*  Your one stop shop c++ edge detection library  *" << endl;
    cout << "*                 Please Stand By...              *" << endl;
    cout << "***************************************************" << endl;

    /*Part I*/
    TGAimg part1Img1;
    part1Img1.loadIMG("input/car.tga");
    part1Img1.convertGrayScale();
    part1Img1.exportIMG("output/postGrayScale4.tga");
    part1Img1.gaussianBlur();
    part1Img1.exportIMG("output/postGauss4.tga");
    part1Img1.sobelEdgeDetection();
    part1Img1.exportIMG("output/postSobel4.tga");
    part1Img1.nonMaxSuppression();
    part1Img1.exportIMG("output/postSuppression4.tga");
    part1Img1.doubleThreshold();
    part1Img1.exportIMG("output/postThreshold4.tga");
    part1Img1.hysteresis();
    part1Img1.exportIMG("output/postHysteresis4.tga");

    

    cout << "***************************************************" << endl;
    cout << "*                Images Processed :)              *" << endl;
    cout << "*            Testing Images for Fidelity          *" << endl;
    cout << "*                Please Stand By...               *" << endl;
    cout << "***************************************************" << endl;

    
     return 0;
}
