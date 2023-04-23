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
        bool TGAimg::compareIMG(TGAimg& image2); //compares two images



        void blendMultiply(TGAimg& bottom); //blend multiply calling object is the top layer
        void blendSubtract(TGAimg& bottom); //blend subtract calling object is top layer
        void blendScreen(TGAimg& bottom); //blend screen 1 - (1 - A) * (1 - B)
        void blendOverlay(TGAimg& bottom); 
        void addGreen(int value);//adds specified green value to green channel
        void scaleRed(int value);//scales red channel by specified red value
        void scaleBlue(int value);//scales blue channel by specified blue value
        void separateRed();//removes red balanced grey value from image
        void separateBlue();//removes blue balanced grey value from image
        void separateGreen();//removes green balanced grey value from image
        void combineColors(TGAimg& gChannel, TGAimg& rChannel);//combines green channel and red channel to calling blue channel image

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

void TGAimg::blendMultiply(TGAimg& bottom)
{
    map<int, Pixel>::iterator iter;
    map<int, Pixel>::iterator iterBot = bottom.image.begin();

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        iter->second.blue = ((iter->second.blue * iterBot->second.blue) / 255.0f) + 0.5f;
        iter->second.green = ((iter->second.green * iterBot->second.green) / 255.0f) + 0.5f;
        iter->second.red = ((iter->second.red * iterBot->second.red) / 255.0f) + 0.5f;

        iterBot++;
    }

}

void TGAimg::blendSubtract(TGAimg& bottom)
{

    map<int, Pixel>::iterator iter;
    map<int, Pixel>::iterator iterBot = bottom.image.begin();

    int resultBlue;
    int resultGreen;
    int resultRed;

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        resultBlue  = (iterBot->second.blue - iter->second.blue);
        resultGreen = (iterBot->second.green - iter->second.green);
        resultRed = (iterBot->second.red - iter->second.red);
        if(resultBlue <= 0)
        {
            iter->second.blue = 0;
        }
        else
        {
            iter->second.blue = static_cast<unsigned char>(resultBlue);
        }
        if(resultGreen <= 0)
        {
            iter->second.green = 0;
        }
        else
        {
            iter->second.green = static_cast<unsigned char>(resultGreen);
        }
        if(resultRed <= 0)
        {
            iter->second.red = 0;
        }
        else
        {
            iter->second.red = static_cast<unsigned char>(resultRed);
        }
        
        iterBot++;
    }
    
}

void TGAimg::blendScreen(TGAimg& bottom)
{
   
    map<int, Pixel>::iterator iter;
    map<int, Pixel>::iterator iterBot = bottom.image.begin();

    float blueNormal;
    float greenNormal;
    float redNormal;

    for(iter = this->image.begin(); iter != this->image.end(); iter ++)
    {
    
        blueNormal = 1 - (1 - (iterBot->second.blue/255.0f)) * (1 - (iter->second.blue/255.0f)); //normalize blue and multiply
        greenNormal = 1 - (1 - (iterBot->second.green/255.0f)) * (1 - (iter->second.green/255.0f)); //normalize green and multiply
        redNormal = 1 - (1 - (iterBot->second.red/255.0f)) * (1 - (iter->second.red/255.0f)); //normalize red and multiply

        iter->second.blue = static_cast<unsigned char>((blueNormal * 255) + 0.5f);
        iter->second.green = static_cast<unsigned char>((greenNormal * 255) + 0.5f);
        iter->second.red = static_cast<unsigned char>((redNormal * 255) + 0.5f);
        
        iterBot++;
    }

}

void TGAimg::blendOverlay(TGAimg& bottom)
{
    map<int, Pixel>::iterator iterTop;
    map<int, Pixel>::iterator iterBot = bottom.image.begin();

    float greyVal;

    for(iterTop = this->image.begin(); iterTop != this->image.end(); iterTop++)
    {
        greyVal = (iterBot->second.blue + iterBot->second.red + iterBot->second.green) / 765.0f;
        if(greyVal < 0.5f)
        {
            iterTop->second.blue = ((2 * iterTop->second.blue * iterBot->second.blue) / 255.0f) + 0.5f;
            iterTop->second.green = ((2 * iterTop->second.green * iterBot->second.green) / 255.0f) + 0.5f;
            iterTop->second.red = ((2 * iterTop->second.red * iterBot->second.red) / 255.0f) + 0.5f;
        }
        else
        {
            iterTop->second.blue = 255.0f - (2 * (255.0f - iterTop->second.blue) * (255.0f - iterBot->second.blue) / 255.0f) + 0.5f;
            iterTop->second.green = 255.0f - (2 * (255.0f - iterTop->second.green) * (255.0f - iterBot->second.green) /255.0f) + 0.5f;
            iterTop->second.red = 255.0f - (2 * (255.0f - iterTop->second.red) * (255.0f - iterBot->second.red) / 255.0f) + 0.5f;
        }

        iterBot++;

    }
}

void TGAimg::addGreen(int value)
{
    map<int, Pixel>::iterator iter;
    int colorChange;

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        colorChange = iter->second.green + value;
        if(colorChange > 255)
        {
            iter->second.green = 255;
        }
        else
        {
            iter->second.green = colorChange;
        }

    } 
}

void TGAimg::scaleRed(int value)
{

    map<int, Pixel>::iterator iter;

    int colorChange;

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        colorChange = iter->second.red * value;
        if(colorChange > 255)
        {
            iter->second.red = 255;
        }
        else
        {
            iter->second.red = colorChange;
        }

    }

}

void TGAimg::scaleBlue(int value)
{

    map<int, Pixel>::iterator iter;
    int colorChange;

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {

        colorChange = iter->second.blue * value;
        if(colorChange > 255)
        {
            iter->second.blue = 255;
        }
        else
        {
            iter->second.blue = colorChange;
        }
        
    }

}

void TGAimg::separateBlue()
{

    map<int, Pixel>::iterator iter;

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        iter->second.green = iter->second.blue;
        iter->second.red = iter->second.blue;
    }

}

void TGAimg::separateGreen()
{

    map<int, Pixel>::iterator iter;

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        iter->second.blue = iter->second.green;
        iter->second.red = iter->second.green;
    }

}

void TGAimg::separateRed()
{

    map<int, Pixel>::iterator iter;

    for(iter = this->image.begin(); iter != this->image.end(); iter++)
    {
        iter->second.green = iter->second.red;
        iter->second.blue = iter->second.red;
    }

}

void TGAimg::combineColors(TGAimg& gChannel, TGAimg& rChannel)
{
    map<int, Pixel>::iterator iterB;
    map<int, Pixel>::iterator iterR = rChannel.image.begin();
    map<int, Pixel>::iterator iterG = gChannel.image.begin();

    for(iterB = this->image.begin(); iterB != this->image.end(); iterB++)
    {
        iterB->second.green = iterG->second.green;
        iterB->second.red = iterR->second.red;

        iterR++;
        iterG++;
    }

}


bool TGAimg::compareIMG(TGAimg& image2)
{
    map<int, Pixel>::iterator iter1;
    map<int, Pixel>::iterator iter2 = image2.image.begin();

    for(iter1 = this->image.begin(); iter1 != this->image.end(); iter1++)
    {
        if(iter1->second.red != iter2->second.red || iter1->second.green != iter2->second.green || iter1->second.blue != iter2->second.blue) 
        {
            return false;
        }

        iter2++;
        
    }

    return true;
}

#endif