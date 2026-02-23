#include <chrono>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <vector>
#include <cmath>
#include <csignal>


#include "body.h"
#include "helical.h"

#define M_PI 3.14159265358979323846
#define DEPTH 100


void getWindowSize(int& screenWidth, int& screenHeight);
void dipslayScreen(std::vector<std::vector<std::string>>& screen, int screenWidth, int screenHeight);
void showCursor(int signal);

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
    float zSpeed = 1.5;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float cameraAngle = 0;
    float cameraSpeed = 0.01;
    
    //baseCameraZoom to have a base value for dynamic zoom
    float baseCameraZoom = 0.4;
    float cameraZoom = baseCameraZoom;

    int maxTrailLength = 200;

    bool hasToRollback = false;

    using clock = std::chrono::high_resolution_clock;
    clock::time_point lastTime = clock::now();

    //all the bodies attributes, main settings in this project, have fun changing it !
    std::vector<BodyAttributes> bodiesAttributes = {
      //{startX, startY, angleSpeed, skinSize, headSkin, trailSkin, r, g, b}
        {centerX     , centerY     , 0   , 7 , "@", "#", 255, 255, 0  }, // sun
        {centerX - 2, centerY - 8 , 0.09 , 1 , "M", "*", 82, 152, 242},
        {centerX + 2, centerY + 8 , 0.09 , 1 , "W", "*", 221, 61, 242},
        {centerX +  20, centerY - 6 , 0.02 , 3 , "&", "*", 166, 53, 242},
        {centerX + 14, centerY - 34 , -0.04 , 2 , "x", "*", 59, 215, 180}

    };
    /*
        std::vector<BodyAttributes> bodiesAttributes = {
      //{startX, startY, angleSpeed, skinSize, headSkin, trailSkin, r, g, b}
        {centerX     , centerY     , 0   , 12 , "@", "#", 255, 255, 0  }, // sun
        {centerX - 20, centerY - 8 , 0.09 , 5 , "M", "*", 82, 152, 242},
        {centerX + 20, centerY + 8 , 0.09 , 3 , "W", "*", 221, 61, 242},

    };*/

    spawnBodies(bodies, bodiesAttributes, maxTrailLength);

    std::cout << "\033[?25l"; // hide cursor
    while(true){
        std::signal(SIGINT, showCursor); //display cursor if the program ends
        //clear terminal
        std::cout << "\033[H\033[J";
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
            if(body.isShootingStar){
                body.moveStraight(commonZ);
                if(body.isOut){
                    bodies.erase(bodies.begin() + i);
                }
                if(hasToRollback){
                    body.z -= DEPTH;
                }
            }
            else{
                body.moveSpiral(centerX, centerY, commonZ);
            }
        }

        generateScreen(screenWidth, screenHeight, spaceMaterial, bodies, commonZ, screen,
            centerX, centerY, cameraAngle, cameraSpeed, hasToRollback, cameraZoom);

        dipslayScreen(screen, screenWidth, screenHeight);

        //handleShootingStars(bodies, centerX, centerY, commonZ, lastTime, 5);

        //wait for the next frame
        std::this_thread::sleep_for(std::chrono::milliseconds(frameDelayMS));
    }
    return 0;
}

void showCursor(int signal) {
    // display back the cursor at the end of the program
    std::cout << "\033[?25h" << std::flush;
    std::exit(signal);
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
