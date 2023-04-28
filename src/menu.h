#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "button.h"
#include "tgaimg.cpp"
using namespace sf;

class Menu
{
public:

    static const unsigned int IMAGE_SIZE = 520;

    static vector<Button> buttonsList;
    static vector<Text> textboxList;
    static vector<RectangleShape> dividersList;
    static string imageName;
    static TGAimg preProcessImage;      //Copy not to be modified
    static TGAimg processingImage;      //Copy to be modified by edge detection
    static Texture preProcessTexture;   //Original image (shown on the left of the GUI)
    static Texture postProcessTexture;  //Post-processed (shown on the right of the GUI)

    static void openMenu()
    {
        //Setup of variables
        RenderWindow window = RenderWindow{ { 1280u, 720u }, "Edge Detective" };
        window.setFramerateLimit(60);

        Font font;
        if(!font.loadFromFile("../src/fonts/arial.ttf"))
            cout << "Error: font could not be loaded" << endl;

        ///TODO: RESERVE APPROPRIATE VECTOR SIZES
        buttonsList.reserve(4);
        buttonsList.emplace_back(Text("Load File ", font), Vector2f(209,120), Vector2f(240,55), &loadFile);
        buttonsList.emplace_back(Text("Save File ", font), Vector2f(491,120), Vector2f(240,55), &saveFile);
        buttonsList.emplace_back(Text("Prewitt Edge Detection  ", font), Vector2f(965,45), Vector2f(450,55), &applyPrewittEdgeDetection);
        buttonsList.emplace_back(Text("Canny Edge Detection  ", font), Vector2f(965,120), Vector2f(450,55), &applyCannyEdgeDetection);
        buttonsList[1].updateStatus(Button::Status::Disabled); //Disable Save File until an algorithm is run
        buttonsList[2].updateStatus(Button::Status::Disabled); //Disable Prewitt until a file is loaded
        buttonsList[3].updateStatus(Button::Status::Disabled); //Disable Canny until a file is loaded

        //Rectangle that holds the pre-process image.
        dividersList.reserve(2);
        dividersList.emplace_back(Vector2f(IMAGE_SIZE,IMAGE_SIZE));
        dividersList[0].setPosition(Vector2f(90,175));
        dividersList[0].setFillColor(Color(255,255,255,0));
        dividersList[0].setOutlineThickness(5);
        dividersList[0].setOutlineColor(Color::Black);

        //Rectangle that holds the post-process image.
        dividersList.emplace_back(Vector2f(IMAGE_SIZE,IMAGE_SIZE));
        dividersList[1].setPosition(Vector2f(670,175));
        dividersList[1].setFillColor(Color(255,255,255,0));
        dividersList[1].setOutlineThickness(5);
        dividersList[1].setOutlineColor(Color::Black);

        //Rectangle that holds the text box
        dividersList.emplace_back(Vector2f(520,55));
        dividersList[2].setPosition(Vector2f(90,17));
        dividersList[2].setFillColor(Color(60,60,80,255));
        dividersList[2].setOutlineThickness(3);
        dividersList[2].setOutlineColor(Color(0,0,0,255));

        //Input text
        textboxList.emplace_back(imageName, font);
        textboxList[0].setPosition(Vector2f(90,20));
        textboxList[0].setCharacterSize(40);

        //Error text
        textboxList.emplace_back("File not found", font);
        textboxList[1].setPosition(Vector2f(120,400));
        textboxList[1].setFillColor(sf::Color::Transparent);
        textboxList[1].setStyle(sf::Text::Bold);
        textboxList[1].setCharacterSize(72);

        Vector2i mousePositionInt;
        Vector2f mousePositionFloat;

        bool mousePressedThisFrame = false;
        bool mouseReleasedThisFrame = false;

        //SFML main loop
        while (window.isOpen())
        {
            window.clear(Color(60, 60, 60, 255));

            //Handle Events (mouse clicks)
            mousePressedThisFrame = false;
            mouseReleasedThisFrame = false;
            for (auto event = Event{}; window.pollEvent(event);)
            {
                switch(event.type)
                {
                    case Event::Closed:
                        window.close();
                        break;
                    case Event::MouseButtonPressed:
                        mousePressedThisFrame = true;
                        break;
                    case Event::MouseButtonReleased:
                        mouseReleasedThisFrame = true;
                        break;
                    case Event::TextEntered: {
                        int fileNameLength = imageName.size();
                        unsigned int unicode = event.text.unicode;
                        // 8 in unicode is the Backspace. If this is pressed the last letter is erased
                        if (unicode == 8) {
                            // Before erasing we must check if the string is empty
                            if (fileNameLength > 0) {
                                imageName.erase(fileNameLength - 1, 1);
                            }
                        }
                        //Unicode between 32 and 126 represents Letters, Numbers, and special Characters
                        else if (unicode >= 32 && unicode <= 126) {
                            imageName += (char) unicode;
                        }
                        textboxList[0].setString(imageName);
                        break;
                    }
                    default:
                        break;
                }
            }

            //Change status of each button depending on the mouse, then draw each button
            mousePositionInt = Mouse::getPosition(window);
            mousePositionFloat = Vector2f(static_cast<float>(mousePositionInt.x), static_cast<float>(mousePositionInt.y));
            for(Button& currentButton: buttonsList)
            {
                switch (currentButton.getStatus())
                {
                    case Button::Status::Disabled:
                        break;
                    case Button::Status::Default:
                        if (currentButton.mouseIsOver(mousePositionFloat))      //If mouse is over button
                            currentButton.updateStatus(Button::Status::Hover);
                        break;
                    case Button::Status::Hover:
                        if (!currentButton.mouseIsOver(mousePositionFloat))     //If mouse is NOT over button
                            currentButton.updateStatus(Button::Status::Default);
                        else if(mousePressedThisFrame) {                                     //If mouse is over button and clicked
                            currentButton.updateStatus(Button::Status::Clicked);
                            currentButton.callFunction();
                        }
                        break;
                    case Button::Status::Clicked:
                        if (!currentButton.mouseIsOver(mousePositionFloat))     //If mouse is NOT over button
                            currentButton.updateStatus(Button::Status::Default);
                        else if(mouseReleasedThisFrame)                                     //If mouse is over button and released
                            currentButton.updateStatus(Button::Status::Hover);
                        break;
                }
                window.draw(currentButton);
            }

            //Draw other UI elements
            for(RectangleShape& currentRect: dividersList){
                window.draw(currentRect);
            }
            for(Text& currentText: textboxList){
                window.draw(currentText);
            }
            window.display();
        }
    }

    static void displayPreProcessImage()
    {
        preProcessTexture.loadFromFile("../input/" + imageName);
        Vector2u textureSize = preProcessTexture.getSize();
        Vector2f textureSizeFloat = Vector2f(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));
        const float maxWidth = IMAGE_SIZE;
        const float maxHeight = IMAGE_SIZE;
        const float scale = std::min(maxWidth/textureSizeFloat.x, maxHeight/textureSizeFloat.y);
        dividersList[0].setTexture(&preProcessTexture);
        dividersList[0].setSize(textureSizeFloat*scale);
        dividersList[0].setFillColor(Color(255,255,255,255));
    }

    static void displayPostProcessImage()
    {
        processingImage.exportIMG("../src/temp.tga"); //Write file into temp file to then open in SFML
        postProcessTexture.loadFromFile("../src/temp.tga");
        Vector2u textureSize = postProcessTexture.getSize();
        Vector2f textureSizeFloat = Vector2f(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));
        const float maxWidth = IMAGE_SIZE;
        const float maxHeight = IMAGE_SIZE;
        const float scale = std::min(maxWidth/textureSizeFloat.x, maxHeight/textureSizeFloat.y);
        dividersList[1].setTexture(&postProcessTexture);
        dividersList[1].setSize(textureSizeFloat*scale);
        dividersList[1].setFillColor(Color(255,255,255,255));
    }

    static void loadFile()
    {
        cout << "loadFile" << endl;
        dividersList[0].setFillColor(Color(255,255,255,0));  //Clear the pre-processing box
        dividersList[1].setFillColor(Color(255,255,255,0));  //Clear the post-processing box
        buttonsList[1].updateStatus(Button::Status::Disabled);                 //Disable Save
        if(preProcessImage.loadIMG("../input/" + imageName)){
            buttonsList[2].updateStatus(Button::Status::Default);   //Enable Prewitt
            buttonsList[3].updateStatus(Button::Status::Default);   //Enable Canny
            textboxList[1].setFillColor(sf::Color::Transparent);
            displayPreProcessImage();
        } else {
            buttonsList[2].updateStatus(Button::Status::Disabled);  //Disable Prewitt
            buttonsList[3].updateStatus(Button::Status::Disabled);  //Disable Canny
            textboxList[1].setFillColor(sf::Color::Red);
        }
    }

    static void saveFile()
    {
        cout << "saveFile" << endl;
        string fileDir = (buttonsList[2].getStatus() == Button::Status::Default ? "../output/canny_" + imageName: "../output/prewitt_" + imageName);
        processingImage.exportIMG(fileDir);
    }

    static void applyPrewittEdgeDetection()
    {
        processingImage = TGAimg(preProcessImage);
        buttonsList[2].updateStatus(Button::Status::Disabled); //Disable to prevent processing again
        buttonsList[1].updateStatus(Button::Status::Default); //Enable Save
        buttonsList[3].updateStatus(Button::Status::Default); //Enable Canny
        processingImage.convertGrayScale();
        processingImage.gaussianBlur();
        processingImage.prewittEdgeDetection();
        displayPostProcessImage();
    }

    static void applyCannyEdgeDetection()
    {
        processingImage = TGAimg(preProcessImage);
        buttonsList[3].updateStatus(Button::Status::Disabled); //Lock to prevent processing again
        buttonsList[1].updateStatus(Button::Status::Default); //Enable Save
        buttonsList[2].updateStatus(Button::Status::Default); //Enable Canny
        processingImage.convertGrayScale();
        processingImage.gaussianBlur();
        processingImage.sobelEdgeDetection();
        processingImage.nonMaxSuppression();
        processingImage.doubleThreshold();
        processingImage.hysteresis();
        displayPostProcessImage();
    }

};

//Define all the static fields. I don't know why C++ needs me to do that all the way out here...
TGAimg Menu::processingImage = TGAimg();
TGAimg Menu::preProcessImage = TGAimg();
string Menu::imageName = "car.tga";
vector<Button> Menu::buttonsList = vector<Button>();
vector<Text> Menu::textboxList = vector<Text>();
vector<RectangleShape> Menu::dividersList = vector<RectangleShape>();
Texture Menu::preProcessTexture = Texture();
Texture Menu::postProcessTexture = Texture();


#endif