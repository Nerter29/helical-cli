#include <chrono>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <vector>
#include <cmath>

#include "body.h"
#include "helical.h"

#define M_PI 3.14159265358979323846
#define DEPTH 100


void getWindowSize(int& screenWidth, int& screenHeight);
void dipslayScreen(std::vector<std::vector<std::string>>& screen, int screenWidth, int screenHeight);

int main(){
    int screenWidth = 80;
    int screenHeight = 44;

    //default screen height is the default limiting factor on the screen, used to have a base value for dynamic zoom
    float defaultScreenLimitingDim = screenHeight; 

    getWindowSize(screenWidth, screenHeight);
    
    std::string spaceMaterial = " ";
    std::vector<std::vector<std::string>> screen;

    //initialize space with spaces materials
    screen.resize(screenHeight, std::vector<std::string>(screenWidth, spaceMaterial));

    std::vector<Body> bodies;

    int frameDelayMS = 20;

    //z coordinate is common because it will be the same for every bodies, 
    //(we consider the solar system as a perfect plate, that all share a common dimension and I chose z)
    float commonZ = 0;

    //solar system speed
    float zSpeed = 1;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float cameraAngle = 0;
    float cameraSpeed = 0.01;
    
    //baseCameraZoom to have a base value for dynamic zoom
    float baseCameraZoom = 0.5;
    float cameraZoom = baseCameraZoom;

    int maxTrailLength = 200;

    bool hasToRollback = false;

    //all the bodies attributes, main settings in this project, have fun changing it !
    std::vector<BodyAttributes> bodiesAttributes = {
        //{startX, startY, angleSpeed, skinSize, headSkin, trailSkin, r, g, b}
        {centerX     , centerY     , 0   , 7 , "@", "#", 255, 255, 0  }, // sun
        {centerX - 2, centerY - 8 , 0.09 , 1 , "M", "*", 82, 152, 242},
        {centerX + 2, centerY + 8 , 0.09 , 1 , "W", "*", 221, 61, 242},
        {centerX +  20, centerY - 6 , 0.06 , 3 , "&", "*", 166, 53, 242},
        {centerX + 14, centerY - 34 , -0.04 , 2 , "x", "*", 59, 215, 180}

    };

    spawnBodies(bodies, bodiesAttributes, maxTrailLength);

    while(true){
        //clear terminal
        system("clear");

        getWindowSize(screenWidth, screenHeight);

        //the zoom is dynamic : if the window shrinks or grows, we adapt the zoom so we can always see the whole system
        float limitingScreenDimension = std::min(screenWidth, screenHeight);
        cameraZoom = baseCameraZoom * (limitingScreenDimension / defaultScreenLimitingDim);

        //move solar system
        commonZ += zSpeed;

        //if z becomes too big, we rollback the entire system, a part of this process is made in generateScreen()
        if(commonZ >= DEPTH * 2 ){
            commonZ -= DEPTH;
            hasToRollback = true;
        }
        else{
            hasToRollback = false;
        }

        //update bodies
        for(int i = 0; i < bodies.size(); i++){
            Body& body = bodies[i];
            body.move(centerX, centerY, commonZ);
        }

        generateScreen(screenWidth, screenHeight, spaceMaterial, bodies, commonZ, screen,
            centerX, centerY, cameraAngle, cameraSpeed, hasToRollback, cameraZoom);

        dipslayScreen(screen, screenWidth, screenHeight);

        //wait for the next frame
        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelayMS));
    }
    return 0;
}




void getWindowSize(int& screenWidth, int& screenHeight){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    screenWidth = w.ws_col / 2; // / 2 because I put spaces beetween every points
    screenHeight = w.ws_row - 1; // -1 because there is one extra row
}


void dipslayScreen(std::vector<std::vector<std::string>>& screen, int screenWidth, int screenHeight){
    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            std::cout << " " << screen[y][x]; // put spaces between every points
        }
        std::cout << "\n";
    }  
}
