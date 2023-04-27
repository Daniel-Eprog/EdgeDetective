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

    static TGAimg imageFile;
    static string imageName;
    static vector<Button> buttonsList;
    static vector<Text> textboxList;
    static vector<RectangleShape> dividersList;
    static Texture preProcessTexture;
    static Texture postProcessTexture;

    static void openMenu()
    {
        //Setup of variables
        RenderWindow window = RenderWindow{ { 1280u, 720u }, "Edge Detective" };
        window.setFramerateLimit(60);

        Font font;
        if(!font.loadFromFile("../src/fonts/arial.ttf"))
            cout << "Error: font could not be loaded" << endl;

        ///TODO: RESERVE APPROPRIATE VECTOR SIZES
        buttonsList.emplace_back(Text("Load File ", font), Vector2f(120,40), Vector2f(200,55), &loadFile);
        buttonsList.emplace_back(Text("Save File ", font), Vector2f(120,105), Vector2f(200,55), &saveFile);
        buttonsList.emplace_back(Text("Prewitt Edge Detection  ", font), Vector2f(1000,40), Vector2f(450,55), &applyPrewittEdgeDetection);
        buttonsList.emplace_back(Text("Canny Edge Detection  ", font), Vector2f(1000,105), Vector2f(450,55), &applyCannyEdgeDetection);
        buttonsList[1].updateStatus(Button::Status::Disabled); //Disable Save File until an algorithm is run
        buttonsList[2].updateStatus(Button::Status::Disabled); //Disable Prewitt until a file is loaded
        buttonsList[3].updateStatus(Button::Status::Disabled); //Disable Canny until a file is loaded

        //Rectangle that holds the pre-process image.
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
                    ///TODO: GET USER KEYBOARD INPUT TO PUT INTO THE STRING "imageName"
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
            for(Text& currentText: textboxList){
                window.draw(currentText);
            }
            for(RectangleShape& currentRect: dividersList){
                window.draw(currentRect);
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
        imageFile.exportIMG("../src/temp.tga"); //Write file into temp file to then open in SFML
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
        imageFile.loadIMG("../input/" + imageName);
        ///TODO: ERROR CHECK FOR FILE MISSING
        displayPreProcessImage();
        dividersList[1].setFillColor(Color(255,255,255,0)); //Clear the post-processing box
        buttonsList[2].updateStatus(Button::Status::Default); //Enable the Prewitt button
        buttonsList[3].updateStatus(Button::Status::Default); //Enable the Canny button
    }

    static void saveFile()
    {
        cout << "saveFile" << endl;
        imageFile.exportIMG("../output/" + imageName);
    }

    static void applyPrewittEdgeDetection()
    {
        buttonsList[2].updateStatus(Button::Status::Disabled); //Disable to prevent processing again
        buttonsList[1].updateStatus(Button::Status::Default); //Enable Save
        buttonsList[3].updateStatus(Button::Status::Default); //Enable Canny
        ///TODO: CALL ALL THE FUNCTIONS
        imageFile.convertGrayScale();
        displayPostProcessImage();
    }

    static void applyCannyEdgeDetection()
    {
        buttonsList[3].updateStatus(Button::Status::Disabled); //Lock to prevent processing again
        buttonsList[1].updateStatus(Button::Status::Default); //Enable Save
        buttonsList[2].updateStatus(Button::Status::Default); //Enable Canny
        ///TODO: CALL ALL THE FUNCTIONS
        imageFile.convertGrayScale();
        displayPostProcessImage();
    }

};

TGAimg Menu::imageFile = TGAimg();
string Menu::imageName = "car.tga";
vector<Button> Menu::buttonsList = vector<Button>();
vector<Text> Menu::textboxList = vector<Text>();
vector<RectangleShape> Menu::dividersList = vector<RectangleShape>();
Texture Menu::preProcessTexture = Texture();
Texture Menu::postProcessTexture = Texture();


#endif