#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#define RECT 0
#define CIRC 1



class Shapes {

    public:
    const int shapeType = 0;
    const std::string shapeName = "Unknown";
    sf::Vector2f positionVec = {0.0,0.0};
    std::vector<float>speedVec = {0.0,0.0};
    std::vector<int>colorVec = {0, 0, 0};
    float scaleFactor = 1.0;

    Shapes( int type, std::string name, sf::Vector2f initPosition,  std::vector<float>speed, 
        std::vector<int> color, float scale)
        :   shapeType(type),
            shapeName(name),                      
            positionVec(initPosition),
            speedVec(speed),
            colorVec(color)
        {}

};


class Circles : Shapes {
    
    public:

    int circleSegments = 32;
    float circleRadius = 0.0;
    sf::CircleShape circ2Draw;

    Circles( int type, std::string name, sf::Vector2f initPosition,  std::vector<float>speed, 
        std::vector<int> color, float scale, float circRad)
                    : Shapes {type, name, initPosition, speed, color, scale},                    
                      circleRadius(circRad)
                    {}
            void makeCircle2Draw()
             {        
                sf::CircleShape* circ2Draw = new sf::CircleShape();
            }
    ~Circles() {} 
    }; 

class Rectangles  : Shapes {
    
        public:

        std::vector<float>sizeVec = {0.0,0.0};
        sf::RectangleShape rect2Draw;
    
    Rectangles( int type, std::string name, sf::Vector2f initPosition,  std::vector<float>speed, 
        std::vector<int> color, float scale, std::vector<float>size)
                    :  Shapes {type, name, initPosition, speed, color, scale}, 
                       sizeVec(size)
                    {}
            void makeRect2Draw()
             {        
                sf::RectangleShape* rect2Draw = new sf::RectangleShape();
            }
    ~Rectangles() {} 
    }; 


int main() 
{
    std::string datum;
    std::vector<std::string>data;
    std::vector<int>windowSize = {0,0};
    std::string path2Font = "";
    int fontSize;
    int circleSegments = 32;
    std::vector<int>fontColor = {0,0,0};
    
    std::vector<CircleShapes> circleShapeVec;
    std::vector<RectangleShapes> rectShapeVec;


 /***************************************************************/
 /* Open and parse the config file                              */
 /***************************************************************/   

    std::ifstream config("config.txt");
    if (config.is_open()) 
    {
        
        int line_count = 0;
        std::string line;
        while (std::getline(config, line)) {
            line_count++;
        }
        config.clear();
        config.seekg(0);

        while (std::getline(config, line)) 
        {
            std::stringstream stream(line);
            while (stream >> datum) 
            {
                data.push_back(datum);
            }
        }
        int j = 0;
        for (auto i : data)
        {
            if (i == "Window") 
            {
                 windowSize = {std::stoi(data[j+1]), std::stoi(data[j+2])};
            }
            else if (i == "Font") 
            {
                path2Font = data[j+1];
                fontSize = std::stoi(data[j+2]);
                fontColor = {std::stoi(data[j+3]),std::stoi(data[j+4]),std::stoi(data[j+5])};
            }

 // if a circle, save the entries into the CircleShapes class and push the object into circleShapeVec     
 // instantiate a sfml CircleShape with the radius entries and save the initial position from entries; save to vector 
 
            else if (i == "Circle") 
            {

                 //shapeType, shapeName, initialPositionVec, speedVec, colorVec, circleRadius
                 CircleShapes circle1(1, data[j+1], sf::Vector2f{std::stof(data[j+2]) , std::stof(data[j+3])}, {std::stof(data[j+4]), 
                     std::stof(data[j+5])},{std::stoi(data[j+6]), std::stoi(data[j+7]), std::stoi(data[j+8])}, 
                     std::stof(data[j+9]));  
                circleShapeVec.push_back(circle1);
                circle1.makeCircle2Draw();
            }
            else if  (i == "Rectangle")
            {
                 //shapeType, shapeName, initialPositionVec, speedVec, colorVec, rectSize
                 RectangleShapes rect1(1, data[j+1], sf::Vector2f{std::stof(data[j+2]) , std::stof(data[j+3])}, {std::stof(data[j+4]), 
                    std::stof(data[j+5])},{std::stoi(data[j+6]), std::stoi(data[j+7]), std::stoi(data[j+8])}, 
                    {std::stof(data[j+9]), std::stof(data[j+10])});  
                rectShapeVec.push_back(rect1);
                rect1.makeRect2Draw();
            }
            j++;
        }
        config.close();

        const int wWidth = 1280;
        const int wHeight = 720;
        sf::RenderWindow window(sf::VideoMode(wWidth, wHeight),"My Window");
        window.setFramerateLimit(60);
        window.clear(sf::Color::Black);

        ImGui::SFML::Init(window);
        sf::Clock deltaClock;
        ImGui::GetStyle().ScaleAllSizes(4.0f);
        float c[3] = {0.0f, 1.0f, 1.0f};

        sf::Font myFont;
        if (!myFont.loadFromFile(path2Font))
        {
            std::cerr << "Could not losd font! \n";
            exit(-1);        
        }
        //sf::text.setPosition(0, wHeight - (float)text.getCharacterSize());
        bool drawCirce = true;
        bool drawText = true;
        bool drawShape = true;
        float scaleFactor = 1.0;
        float velocityX = 2;
        float velocityY = 2;
        std::vector<const char*>names;
        for(auto& obj : circleShapeVec)
        {
            names.push_back(obj.shapeName.c_str());
        }
        for(auto& obj : rectShapeVec)
        {
            names.push_back(obj.shapeName.c_str());
        }
        for (auto a : names)
            std::cout << a << "\n"; 
        int shapeIndex = 0;
        int currentShapeIndex = 0;


       
        
        while (window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                ImGui::SFML::ProcessEvent(window, event);
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();

/*************************************************************************/
            ImGui::SFML::Update(window, deltaClock.restart());
            ImGui::Begin("Bouncing Shapes");
            ImGui::Checkbox("Draw Shape", &drawShape);
            ImGui::SameLine();
            ImGui::Checkbox("Draw Text",  &drawText);
            if (ImGui::SliderFloat("Scale", &scaleFactor, 0.0f, 4.0f))
                for (auto& a : circleShapeVec)
                    if (a.shapeName == names[shapeIndex])
                        a.scaleFactor = scaleFactor;
                for (auto& a : rectShapeVec)
                    if (a.shapeName == names[shapeIndex])
                        a.scaleFactor = scaleFactor;
            ImGui::SetNextItemWidth(100);
            if (ImGui::SliderFloat("X", &velocityX, -8.0f, 8.0f))
            {
                for (auto& a : circleShapeVec)
                    if (a.shapeName == names[shapeIndex])
                        a.speedVec[0] = velocityX;
                for (auto& a : rectShapeVec)
                    if (a.shapeName == names[shapeIndex])
                        a.speedVec[0] = velocityX;                   
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            if (ImGui::SliderFloat("Y  Velocity", &velocityY, -8.0f, 8.0f))
            {
                for (auto& a : circleShapeVec)
                    if (a.shapeName == names[shapeIndex])
                        a.speedVec[1] = velocityY;
                for (auto& a : rectShapeVec)
                    if (a.shapeName == names[shapeIndex])
                        a.speedVec[1] = velocityY;                   
            }
            if(ImGui::ColorEdit3("Color Circle", c))
            {
                for (auto& a : circleShapeVec)
                    if (a.shapeName == names[shapeIndex])
                        a.colorVec = {(int)c[0]*255, (int)c[1]*255, (int)c[2]*255};
                for (auto& a : rectShapeVec)
                    if (a.shapeName == names[shapeIndex])
                        a.colorVec = {(int)c[0]*255, (int)c[1]*255, (int)c[2]*255};
            }
            if (ImGui::Combo("Shape", &shapeIndex, names.data(), (int)names.size(), -1))
            {
                for (auto& a : circleShapeVec)
                    if (a.shapeName == names[shapeIndex]) 
                    { 
                        scaleFactor = a.scaleFactor;    
                        velocityX = a.speedVec[0];
                        velocityY = a.speedVec[1];
                        
                    }

            }
            

            ImGui::End();
            int i = 0;

            for (auto sfObj: circleShapeVec)
            {
                sfObj.circ2Draw.setRadius(circleShapeVec[i].circleRadius * circleShapeVec[i].scaleFactor);
                if ((circleShapeVec[i].positionVec.x + 2 * (circleShapeVec[i].circleRadius * circleShapeVec[i].scaleFactor) > 1280)
                    || circleShapeVec[i].positionVec.x < 0.0) 
                {
                    circleShapeVec[i].speedVec[0] *= -1.0f;
                }
                if ((circleShapeVec[i].positionVec.y + 2 * (circleShapeVec[i].circleRadius * circleShapeVec[i].scaleFactor) > 720)
                    || circleShapeVec[i].positionVec.y < 0.0)
                {
                    circleShapeVec[i].speedVec[1] *= -1.0f;
                }
                 
                sfObj.circ2Draw.setPosition(circleShapeVec[i].positionVec);

                // sf::FloatRect boundingBox = sfObj.circ2Draw.getLocalBounds();
                // std::cout << boundingBox.left << " " << boundingBox.top << "\n";
                // if ((boundingBox.left <= 0.0) || (boundingBox.width >= wWidth))
                //     circleShapeVec[i].speedVec[0] *= -1.0f;
                // if ((boundingBox.top <= 0.0) || (boundingBox.height >= wHeight))
                //     circleShapeVec[i].speedVec[1] *= -1.0f;


                circleShapeVec[i].positionVec.x += circleShapeVec[i].speedVec[0];
                circleShapeVec[i].positionVec.y += circleShapeVec[i].speedVec[1];

                sfObj.circ2Draw.setPosition(circleShapeVec[i].positionVec);

                int red = circleShapeVec[i].colorVec[0];
                int green = circleShapeVec[i].colorVec[1];
                int blue = circleShapeVec[i].colorVec[2];
                sfObj.circ2Draw.setFillColor(sf::Color(red, green, blue));
                window.draw(sfObj.circ2Draw);
                i++;
            }
            i=0;
            for (auto sfObj: rectShapeVec)
            {
                if (rectShapeVec[i].positionVec.x + rectShapeVec[i].sizeVec[0] > 1280
                    || rectShapeVec[i].positionVec.x < 0.0) 
                    {
                        rectShapeVec[i].speedVec[0] *= -1.0f;
                    }
                if (rectShapeVec[i].positionVec.y + rectShapeVec[i].sizeVec[1]> 720 
                    || rectShapeVec[i].positionVec.y < 0.0)
                    {
                    rectShapeVec[i].speedVec[1] *= -1.0f;
                    }

                rectShapeVec[i].positionVec.x += rectShapeVec[i].speedVec[0];
                rectShapeVec[i].positionVec.y += rectShapeVec[i].speedVec[1];

                sfObj.rect2Draw.setPosition(rectShapeVec[i].positionVec);
                sfObj.rect2Draw.setSize(sf::Vector2f (rectShapeVec[i].sizeVec[0] * rectShapeVec[i].scaleFactor, 
                        rectShapeVec[i].sizeVec[1] * rectShapeVec[i].scaleFactor));
                int red = rectShapeVec[i].colorVec[0];
                int green = rectShapeVec[i].colorVec[1];
                int blue = rectShapeVec[i].colorVec[2];
                sfObj.rect2Draw.setFillColor(sf::Color(red, green, blue));
                window.draw(sfObj.rect2Draw);
                i++;
            }
            ImGui::SFML::Render(window);
             window.display();
        }
    }
}