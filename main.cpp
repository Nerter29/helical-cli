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

#define DEPTH 200

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
    float zSpeed = 2.5;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float cameraAngle = 0;
    float cameraSpeed = 0.01;
    float cameraZoom = 0.3;

    int maxTrailLength = 200;

    bool hasToRollback = false;
/*
    std::vector<std::vector<float>> bodiesAttributes = {
      //{x, y, angleSpeed, r, g, b}
        {centerX - 12, centerY + 10, 0.05},
        {centerX - 1, centerY + 16, 0.1},
        {centerX + 18, centerY + 3, 0.07},
        {centerX - 2, centerY - 25, 0.14},

    };*/
    std::vector<std::vector<float>> bodiesAttributes = {
          //{x, y, angleSpeed, r, g, b}


    {centerX - 30, centerY - 15, 0.035, 255, 0, 0},
    {centerX + 18, centerY + 25, 0.12, 0, 255, 0},
    {centerX + 9, centerY - 6, 0.05, 0, 0, 255},
    {centerX - 40, centerY + 10, 0.08, 255, 255, 0},
    {centerX + 8, centerY - 35, 0.15, 255, 0, 255},
};

    spawnBodies(bodies, bodiesAttributes, maxTrailLength);

    while(true){
        std::cout << "\033[H\033[J";
        std::cout << commonZ << " ";
        getWindowSize(screenWidth, screenHeight);

        commonZ += zSpeed;
        if(commonZ >= DEPTH * 2){
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