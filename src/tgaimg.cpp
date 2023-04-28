#include "tgaimg.h"

//loads image from TGA file
bool TGAimg::loadIMG(const string& file)
{
    //read in file
    ifstream fileToOpen(file, ios_base::binary);

    if(!fileToOpen.is_open())
    {
        cout << "Sorry, your file could not be opened" << endl;
        return false;
    }

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
            nextRow.emplace_back(blue, green, red);
        }

        this->image.push_back(nextRow);
        nextRow = {};
    }

    //close image file
    fileToOpen.close();
    return true;

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

//convolution method for applying filters to current image
vector<double> TGAimg::convolve(vector<vector<float>> convolutionKernel)
{

    vector<double> newImage = {};

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
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                else
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i +1][j + 1].red * convolutionKernel[2][2];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }

            }
            else if(j == imageHeader.width - 1)
            {
                if(i == 0)
                {
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                     newImage.push_back(newPixelValue);
                }
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                else
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i + 1][j - 1].red * convolutionKernel[2][0];
                    newPixelValue += this->image[i + 1][j].red * convolutionKernel[2][1];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
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
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
                else if(i == imageHeader.height - 1)
                {
                    newPixelValue += this->image[i - 1][j].red * convolutionKernel[0][1];
                    newPixelValue += this->image[i][j - 1].red * convolutionKernel[1][0];
                    newPixelValue += this->image[i - 1][j - 1].red * convolutionKernel[0][0];
                    newPixelValue += this->image[i - 1][j + 1].red * convolutionKernel[0][2];
                    newPixelValue += this->image[i][j + 1].red * convolutionKernel[1][2];
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
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
                    newPixelValue += this->image[i][j].red * convolutionKernel[1][1];
                    newImage.push_back(newPixelValue);
                }
            }
        }
    }

    return newImage;

}

//iterates over image and converts pixels to their grayscale value
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

//gaussian blur is applied to your image to reduce noise
void TGAimg::gaussianBlur()
{
    vector<double> blurredImage;
    vector<vector<float>> gaussianKernel = {{0.075f, 0.124f, 0.075f},{0.124f, .204f, 0.124f},{0.075f, 0.0124f, 0.075f}};
    blurredImage = this->convolve(gaussianKernel);

    int count = 0;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = blurredImage[count];
            this->image[i][j].blue = blurredImage[count];
            this->image[i][j].green = blurredImage[count];
            count += 1;
        }

    }
}

//prewitt kernel for general edge detection
void TGAimg::prewittEdgeDetection()
{
    vector<double> Gx = {};
    vector<double> Gy = {};
    vector<int> G = {};

    vector<vector<float>> horizontalOperator = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    vector<vector<float>> verticalOperator = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
    Gx = this->convolve(horizontalOperator);
    Gy = this->convolve(verticalOperator);

    //Find magnitude values 
    for(int i = 0; i < Gx.size(); ++i)
    {
        int magnitude = 0;
        magnitude = sqrt(pow(Gx[i], 2) + pow(Gy[i], 2));
        if(magnitude > 255)
        {
            magnitude = 255;
        }
        G.push_back(magnitude);
    }

    //Find the angle of the pixel value
    for(int i = 0; i < Gx.size(); ++i)
    {
        double theta = 0.0f;
        theta = atan(Gy[i] * 1.0f/Gx[i]);
        if(theta == -0 || isnan(theta))
        {
            theta = 0;
        }
        theta = theta * 180/M_PI;
        if(theta < 0)
        {
            theta += 180;
        }
        this->edgeAngles.push_back(theta);
    }

    int count = 0;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = G[count];
            this->image[i][j].blue = G[count];
            this->image[i][j].green = G[count];
            count += 1;
        }
    }

}

//sobel kernel for general edge detection
void TGAimg::sobelEdgeDetection()
{
    vector<double> Gx = {};
    vector<double> Gy = {};
    vector<double> G = {};

    vector<vector<float>> horizontalOperator = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    vector<vector<float>> verticalOperator = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    Gx = this->convolve(horizontalOperator);
    Gy = this->convolve(verticalOperator);

    //Find magnitude values 
    for(int i = 0; i < Gx.size(); ++i)
    {
        int magnitude = 0;
        magnitude = sqrt(pow(Gx[i], 2) + pow(Gy[i], 2));
        if(magnitude > 255)
        {
            magnitude = 255;
        }
        if(magnitude < 50)
        {
            magnitude = 0;
        }
        G.push_back(magnitude);
    }

    // int Gmax = 0;
    // for(int i = 0; i < G.size(); ++i)
    // {
    //     if(G.at(i) > Gmax)
    //     {
    //         Gmax = G.at(i);
    //     }
    // }

    // for(int i = 0; i < G.size(); ++i)
    // {
    //     G.at(i) = G.at(i) / Gmax * 255; 
    //     if(G.at(i) > 40)
    //     {
    //         G.at(i) = 255;
    //     }
    // }

    // Find the angle of the pixel value
    for(int i = 0; i < Gx.size(); ++i)
    {
        double theta = 0.0f;
        theta = atan(Gy[i]/Gx[i]);
        if(theta == -0 || isnan(theta))
        {
            theta = 1.5708;
        }
        theta = theta * 180/M_PI;
        if(theta < 0)
        {
            theta += 180;
        }
        this->edgeAngles.push_back(theta);
    }

    int count = 0;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = G[count];
            this->image[i][j].blue = G[count];
            this->image[i][j].green = G[count];
            count += 1;
        }
    }

}

//uses the angle of pixel intesities to calculate and thin out pixel fields
void TGAimg::nonMaxSuppression() {

    // This function will use the angle of each edge and evaluate the pixels on both sides in the direction it is facing
    // If a pixel has a higher magnitude than the surrounding pixels then it will be used in the image, if not then 0 will be used.
    int count = 0;
    vector<int> newImage;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            if (j == 0)
            {
                if (i == 0)
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            count += 1;
                            newImage.push_back(this->image[i][j].red);

                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else{

                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        newImage.push_back(this->image[i][j].red);
                        count += 1;
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                        // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                }
                else if(i == imageHeader.height - 1)
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) ){

                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }

                            // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {

                        if (this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {       
                        newImage.push_back(this->image[i][j].red);
                        count += 1;
                    }
                }
                else
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }

                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j].red && this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }

                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {

                        if (this->image[i][j].red >= this->image[i+1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }

            }
            else if(j == imageHeader.width - 1)
            {
                if (i == 0)
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) ){

                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }

                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {

                        if (this->image[i][j].red >= this->image[i+1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5){

                        if (this->image[i][j].red >= this->image[i+1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        newImage.push_back(this->image[i][j].red);
                        count += 1;
                    }

                }
                else if(i == imageHeader.height - 1)
                {

                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        newImage.push_back(this->image[i][j].red);
                        count += 1;

                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5){

                        if (this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5){

                        if (this->image[i][j].red >= this->image[i-1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }
                else
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j].red && this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else 
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }

            }
            else 
            {
                if (i == 0)
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red && this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }
                else if(i == imageHeader.height - 1){
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {
                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red && this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                        // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                        // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j-1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }
                else
                {
                    // If the edge angle is between +/-22.5 degrees we will round it to 0 degrees. The Pixel at [i][j] is compared to the pixels to the left and right.
                    if ( (0 <= this->edgeAngles[count] && this->edgeAngles[count] < 22.5) || (157.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 180) )
                    {

                        // If the value is greater than both neighboring pixels then it is a local max, and we do not change it
                        if (this->image[i][j].red >= this->image[i][j-1].red && this->image[i][j].red >= this->image[i][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;

                        }
                        // If either of the 2 values around it are greater than it then we suppress the pixel to 0
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 22.5 and 67.5 degrees we will round it to 45 degrees. The Pixel at [i][j] is compared to the pixels to the top right and bottom left.
                    else if (22.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 67.5)
                    {
                        if (this->image[i][j].red >= this->image[i+1][j-1].red && this->image[i][j].red >= this->image[i-1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else{
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 67.5 and 112.5 degrees we will round it to 90 degrees. The Pixel at [i][j] is compared to the pixels to the top and bottom.
                    else if (67.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 112.5){

                        if (this->image[i][j].red >= this->image[i+1][j].red && this->image[i][j].red >= this->image[i-1][j].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                    // If the edge angle is between 112.5 and 157.5 degrees we will round it to 135 degrees. The Pixel at [i][j] is compared to the pixels to the top left and bottom right.
                    else if (112.5 <= this->edgeAngles[count] && this->edgeAngles[count] < 157.5)
                    {
                        if (this->image[i][j].red >= this->image[i-1][j-1].red && this->image[i][j].red >= this->image[i+1][j+1].red)
                        {
                            newImage.push_back(this->image[i][j].red);
                            count += 1;
                        }
                        else
                        {
                            newImage.push_back(0);
                            count += 1;
                        }
                    }
                }

            }

        }
    }

    count = 0;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            this->image[i][j].red = newImage[count];
            this->image[i][j].blue = newImage[count];
            this->image[i][j].green = newImage[count];
            count += 1;
        }
    }
}

//reduces the image into two pixel values strong and weak to further differentiate edges
void TGAimg::doubleThreshold()
{
    float lowThresholdRatio = 0.09;
    float highThresholdRatio = 0.15;
    float highThreshold;
    float lowThreshold;
    int imageMax = 0;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            if(this->image[i][j].red > imageMax)
            {
                imageMax = this->image[i][j].red;
            }

        }
    }

    highThreshold = imageMax * highThresholdRatio;
    lowThreshold = highThreshold * lowThresholdRatio;

    int weak = 25;
    int strong = 255;

    for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            if(this->image[i][j].red >= highThreshold)
            {
                this->image[i][j].red = strong;
                this->image[i][j].blue = strong;
                this->image[i][j].green = strong;

            }
            else if(this->image[i][j].red < lowThreshold)
            {
                this->image[i][j].red = 0;
                this->image[i][j].blue = 0;
                this->image[i][j].green = 0;

            }
            else if(this->image[i][j].red <= highThreshold && this->image[i][j].red >= lowThreshold)
            {
                this->image[i][j].red = weak;
                this->image[i][j].blue = weak;
                this->image[i][j].green = weak;

            }
        }
    }
}

//condenses pixels together based on their surrounding pixels
void TGAimg::hysteresis()
{

     for(int i = 0; i < this->imageHeader.height; ++i)
    {
        for(int j = 0; j < this->imageHeader.width; ++j)
        {
            if(this->image[i][j].red == 25)
            {

            if(j == 0)
            {
            
                if(i == 0)
                {
                    if( this->image[i][j + 1].red == 255 || this->image[i + 1][j].red == 255 || this->image[i +1][j + 1].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                else if(i == imageHeader.height - 1)
                {
                    if(this->image[i - 1][j].red == 255 || this->image[i - 1][j + 1].red == 255 || this->image[i][j + 1].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                   
                }
                else
                {
                    if(this->image[i - 1][j].red == 255 || 
                    this->image[i - 1][j + 1].red == 255 || 
                    this->image[i][j + 1].red == 255 || 
                    this->image[i +1][j + 1].red == 255 || 
                    this->image[i + 1][j].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }

            }
            else if(j == imageHeader.width - 1)
            {
                if(i == 0)
                {
                    if( this->image[i][j - 1].red == 255 || this->image[i + 1][j - 1].red == 255 || this->image[i + 1][j].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                else if(i == imageHeader.height - 1)
                {
                    if(this->image[i - 1][j].red == 255 || this->image[i][j - 1].red == 255 || this->image[i - 1][j - 1].red == 255)
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                else
                {
                    if( this->image[i - 1][j].red == 255 ||
                        this->image[i][j - 1].red == 255 ||
                        this->image[i - 1][j - 1].red == 255 ||
                        this->image[i + 1][j - 1].red == 255 ||
                        this->image[i + 1][j].red == 255
                        )
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }

            }
            else
            {
                 if(i == 0)
                {
                    if (
                    this->image[i][j + 1].red == 255 ||
                    this->image[i +1][j + 1].red == 255 ||
                    this->image[i][j - 1].red == 255 ||
                    this->image[i + 1][j - 1].red == 255 ||
                    this->image[i + 1][j].red == 255
                    )
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                else if(i == imageHeader.height - 1)
                {
                    if(
                        this->image[i - 1][j].red == 255 ||
                        this->image[i][j - 1].red == 255 ||
                        this->image[i - 1][j - 1].red == 255 ||
                        this->image[i - 1][j + 1].red == 255 ||
                        this->image[i][j + 1].red == 255
                    )
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
                else
                {
                    if(
                        this->image[i - 1][j].red == 255 ||
                        this->image[i][j - 1].red == 255 ||
                        this->image[i - 1][j - 1].red == 255 ||
                        this->image[i - 1][j + 1].red == 255 ||
                        this->image[i][j + 1].red == 255 ||
                        this->image[i + 1][j - 1].red == 255 ||
                        this->image[i + 1][j].red == 255 ||
                        this->image[i + 1][j + 1].red == 255
                    )
                    {
                        this->image[i][j].red = 255;
                        this->image[i][j].blue = 255;
                        this->image[i][j].green = 255;
                    }
                    else
                    {
                        this->image[i][j].red = 0;
                        this->image[i][j].blue = 0;
                        this->image[i][j].green = 0;
                    }
                }
            }
        }
        }
    }


}