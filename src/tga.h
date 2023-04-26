#ifndef TGA_H
#define TGA_H

#include <iostream>
#include <algorithm>
#include <map>
#include <iomanip>
#include <fstream>
#include <vector>

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
        void gaussianBlur();
        void prewittEdgeDetection();
        void convolve(vector<vector<float>> convolutionKernel);

        private:
            IMGheader imageHeader; //stores header information in custom struct 
            vector<vector<Pixel>> image; //stores pixel information in map
};


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

    for (int i = 0; i < (int)this->imageHeader.height; i++)
    {
        vector<Pixel> nextRow = {};
        for(int j = 0; j < (int)this->imageHeader.width; j++)
        {
            unsigned char blue;
            unsigned char green;
            unsigned char red;
            fileToOpen.read(reinterpret_cast<char*>(&blue), sizeof(blue));
            fileToOpen.read(reinterpret_cast<char*>(&green), sizeof(green));
            fileToOpen.read(reinterpret_cast<char*>(&red), sizeof(red));
            nextRow.push_back(Pixel(blue, green, red));
        }

        this->image.push_back(nextRow);
        nextRow = {};
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
    for(int i = 0; i < (int)this->imageHeader.height; ++i)
    {
        for(int j = 0; j < (int)this->imageHeader.width; ++j)
        {
        fileToWrite.write((char*) &this->image[i][j].blue, sizeof(this->image[i][j].blue));
        fileToWrite.write((char*) &this->image[i][j].green, sizeof(this->image[i][j].green));
        fileToWrite.write((char*) &this->image[i][j].red, sizeof(this->image[i][j].red));
        }
    }
}



void TGAimg::convertGrayScale()
{
    float grayScaleValue = 0;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            grayScaleValue = (0.21 * this->image[i][j].red) + (0.72 * this->image[i][j].green) + (0.07 * this->image[i][j].blue); 
            this->image[i][j].blue = grayScaleValue;
            this->image[i][j].green = grayScaleValue;
            this->image[i][j].red = grayScaleValue;
        }

    }

}

void TGAimg::prewittEdgeDetection()
{
    vector<Pixel> Gx = {};
    vector<Pixel> Gy = {};
    vector<Pixel> G = {};

    vector<vector<float>> verticalOperator = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    vector<vector<float>> horizontalOperator = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
    this->convertGrayScale();
    this->gaussianBlur();
    Gx = this->convolve(horizontalOperator);
    Gy = this->convolve(verticalOperator);

    for(int i = 0; i < Gx.size(); ++i)
    {
        int magnitude = 0;
        magnitude = sqrt(pow(Gx[i].red, 2) + pow(Gy[i].red, 2));
        if(magnitude > 255)
        {
            magnitude = 255;
        }
        G.push_back(Pixel(magnitude, magnitude, magnitude));
    }

    int count = 0;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = G[count].red;
            this->image[i][j].blue = G[count].blue;
            this->image[i][j].green = G[count].green;
            count += 1;
        }
    }

}

vector<TGAimg::Pixel> TGAimg::convolve(vector<vector<float>> convolutionKernel)
{

    vector<Pixel> newImage = {};

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        vector<Pixel> newRow = {};

        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            int newPixelValue = 0;

            if(j == 0)
            {
            
                if(i == 0)
                {
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i +1][j + 1].red * convolutionKernel[2][2];
                    if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                    newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                     if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                   newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }
                else
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i +1][j + 1].red * convolutionKernel[2][2];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                     if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                    newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }

            }
            else if(j == imageHeader.width - 1)
            {
                if(i == 0)
                {
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                     if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                     newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                     if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                    newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }
                else
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                     if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                    newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }

            }
            else
            {
                 if(i == 0)
                {
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i +1][j + 1].red * convolutionKernel[2][2];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                     if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                    newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                     if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                    newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }
                else
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i + 1][j + 1].red * convolutionKernel[2][2];
                    if(newPixelValue > 255)
                    {
                        newPixelValue = 255;
                    }
                    else if(newPixelValue < 0)
                    {
                        newPixelValue = 0;
                    }
                    newImage.push_back(Pixel(newPixelValue, newPixelValue, newPixelValue));
                }
            }
        }
    }

    return newImage;

}

void TGAimg::gaussianBlur()
{
    vector<Pixel> blurredImage;
    vector<vector<float>> gaussianKernel = {{0.075f, 0.124f, 0.075f},{0.124f, .204f, 0.124f},{0.075f, 0.0124f, 0.075f}};
    blurredImage = this->convolve(gaussianKernel);

    int count = 0;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = blurredImage[count].red;
            this->image[i][j].blue = blurredImage[count].blue;
            this->image[i][j].green = blurredImage[count].green;
            count += 1;
        }

    }
}

#endif