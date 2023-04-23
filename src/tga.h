#ifndef TGAIMG_H
#define TGAIMG_H

#include <iostream>
#include <algorithm>
#include <map>
#include <iomanip>
#include <fstream>

using namespace std;

class TGAimg
{

    public:

        //struct for storing pixel color data
        struct Pixel
        {
            //pixel color variables
            unsigned char blue;
            unsigned char green;
            unsigned char red;
            //pixel constructor
            Pixel(unsigned char bval, unsigned char gval, unsigned char rval)
            : blue{bval}, green{gval}, red{rval} { }

        };

        //struct for storing image header data
        struct IMGheader
        {

            char idLength;
            char colorMapType;
            char dataTypeCode;
            short colorMapOrigin;
            short colorMapLength;
            char colorMapDepth;
            short xOrigin;
            short yOrigin;
            short width;
            short height;
            char bitsPerPixel;
            char imageDescriptor;
            
        };

        void loadIMG(string file);//loads image from TGA file
        void exportIMG(string file) const;//exports an image as TGA file
        void getHeader() const; //prints header information
        void convertGrayScale(); //converts the desired image into grayscale

    private:
        IMGheader imageHeader; //stores header information in custom struct 
        map<int, Pixel> image; //stores pixel information in map

};

//loads image from TGA file
void TGAimg::loadIMG(string file)
{
    //read in file
    ifstream fileToOpen(file, ios_base::binary);

    if(!fileToOpen.is_open())
    {
        cout << "Sorry, your file could not be opened" << endl;
    }
    else
    {
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.idLength), sizeof(this->imageHeader.idLength));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.colorMapType), sizeof(this->imageHeader.colorMapType));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.dataTypeCode), sizeof(this->imageHeader.dataTypeCode));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.colorMapOrigin), sizeof(this->imageHeader.colorMapOrigin));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.colorMapLength), sizeof(this->imageHeader.colorMapLength));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.colorMapDepth), sizeof(this->imageHeader.colorMapDepth));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.xOrigin), sizeof(this->imageHeader.xOrigin));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.yOrigin), sizeof(this->imageHeader.yOrigin));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.width), sizeof(this->imageHeader.width));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.height), sizeof(this->imageHeader.height));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.bitsPerPixel), sizeof(this->imageHeader.bitsPerPixel));
        fileToOpen.read(reinterpret_cast<char*>(&this->imageHeader.imageDescriptor), sizeof(this->imageHeader.imageDescriptor));
    }

    for (int i = 0; i < (int)this->imageHeader.width * (int)this->imageHeader.height; i++)
    {
        unsigned char blue;
        unsigned char green;
        unsigned char red;
        fileToOpen.read(reinterpret_cast<char*>(&blue), sizeof(blue));
        fileToOpen.read(reinterpret_cast<char*>(&green), sizeof(green));
        fileToOpen.read(reinterpret_cast<char*>(&red), sizeof(red));
        this->image.emplace(i, Pixel(blue, green, red));
    }

    //close image file
    fileToOpen.close();

}

//prints out header information
void TGAimg::getHeader() const
{
    cout << "ID Length: " << (int)imageHeader.idLength << endl;
    cout << "Color Map Type: " << (int)imageHeader.colorMapType << endl;
    cout << "Data Type Code: " << (int)imageHeader.dataTypeCode << endl;
    cout << "Color Map Origin: " << imageHeader.colorMapOrigin << endl;
    cout << "Color Map Length: " << imageHeader.colorMapLength << endl;
    cout << "Color Map Depth: " << (int)imageHeader.colorMapDepth << endl;
    cout << "X Origin: " << imageHeader.xOrigin << endl;
    cout << "Y Origin: " << imageHeader.yOrigin << endl;
    cout << "Width: " << imageHeader.width << endl;
    cout << "Height: " << imageHeader.height << endl;
    cout << "Bits Per Pixel: " << (int)imageHeader.bitsPerPixel << endl;
    cout << "Image Descriptor: " << (int)imageHeader.imageDescriptor << endl;
}

//exports image as TGA file
void TGAimg::exportIMG(string file) const
{
    ofstream fileToWrite(file, ios_base::binary);
    fileToWrite.write((char*) &imageHeader.idLength, sizeof(imageHeader.idLength));
    fileToWrite.write((char*) &imageHeader.colorMapType, sizeof(imageHeader.colorMapType));
    fileToWrite.write((char*) &imageHeader.dataTypeCode, sizeof(imageHeader.dataTypeCode));
    fileToWrite.write((char*) &imageHeader.colorMapOrigin, sizeof(imageHeader.colorMapOrigin));
    fileToWrite.write((char*) &imageHeader.colorMapLength, sizeof(imageHeader.colorMapLength));
    fileToWrite.write((char*) &imageHeader.colorMapDepth, sizeof(imageHeader.colorMapDepth));
    fileToWrite.write((char*) &imageHeader.xOrigin, sizeof(imageHeader.xOrigin));
    fileToWrite.write((char*) &imageHeader.yOrigin, sizeof(imageHeader.yOrigin));
    fileToWrite.write((char*) &imageHeader.width, sizeof(imageHeader.width));
    fileToWrite.write((char*) &imageHeader.height, sizeof(imageHeader.height));
    fileToWrite.write((char*) &imageHeader.bitsPerPixel, sizeof(imageHeader.bitsPerPixel));
    fileToWrite.write((char*) &imageHeader.imageDescriptor, sizeof(imageHeader.imageDescriptor));
    map<int, Pixel>::iterator iter;
    for(auto iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        fileToWrite.write((char*) &iter->second.blue, sizeof(iter->second.blue));
        fileToWrite.write((char*) &iter->second.green, sizeof(iter->second.green));
        fileToWrite.write((char*) &iter->second.red, sizeof(iter->second.red));
    }
}

//converts TGAimg to grayscale using luminosity method 0.21 R + 0.72 G + 0.07 B
void TGAimg::convertGrayScale()
{
    map<int, Pixel>::iterator iter;

    float grayScaleValue = 0;

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        grayScaleValue = (0.21 * iter->second.red) + (0.72 * iter->second.green) + (0.07 * iter->second.blue); 
        iter->second.blue = grayScaleValue;
        iter->second.green = grayScaleValue;
        iter->second.red = grayScaleValue;

    }

}


#endif