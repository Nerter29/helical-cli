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

void getWindowSize(int& screenWidth, int& screenHeight){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    screenWidth = w.ws_col / 2; // / 2 because I put spaces beetween every points
    screenHeight = w.ws_row - 1; // -1 because there is one extra row
}


void dipslayScreen(std::vector<std::vector<std::string>>& screen, int screenWidth, int screenHeight){
    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            std::cout << " " << screen[y][x];
        }
        std::cout << "\n";
    }  
}



int main(){
    int screenWidth = 80;
    int screenHeight = 50;

    getWindowSize(screenWidth, screenHeight);
    
    std::string spaceMaterial = " ";
    std::vector<std::vector<std::string>> screen;
    screen.resize(screenHeight, std::vector<std::string>(screenWidth, spaceMaterial));

    std::vector<Body> bodies;

    float commonZ = 0;
    float zSpeed = 1.5;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float cameraAngle = 0;
    float cameraSpeed = 0.01;
    float cameraZoom = 0.4;

    int maxTrailLength = 100;

    bool hasToRollback = false;
    std::vector<std::vector<float>> bodiesAttributes = {
          //{x, y, angleSpeed, r, g, b}

    {centerX, centerY, 0, 255, 255, 0},
    {centerX - 10, centerY - 5, 0.15, 255, 0, 0}, //10
    {centerX + 0, centerY - 20, 0.05, 0, 0, 255}, //20
    {centerX - 25, centerY + 12, 0.02, 255, 0, 255}, //35
    {centerX + 10, centerY - 40, 0.07, 0, 255, 255}, //50
};

    spawnBodies(bodies, bodiesAttributes, maxTrailLength, centerX, centerY, commonZ);

    
    while(true){
        std::cout << "\033[H\033[J";
        getWindowSize(screenWidth, screenHeight);

        commonZ += zSpeed;
        if(commonZ >= DEPTH * 2 ){
            commonZ -= DEPTH;
            hasToRollback = true;
        }
        else{
            hasToRollback = false;
        }
        for(int i = 0; i < bodies.size(); i++){
            Body& body = bodies[i];

            body.move(centerX, centerY, commonZ);

        }
        generateScreen(screenWidth, screenHeight, spaceMaterial, bodies, commonZ, screen,
            centerX, centerY, cameraAngle, cameraSpeed, hasToRollback, cameraZoom);
        dipslayScreen(screen, screenWidth, screenHeight);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return 0;
}