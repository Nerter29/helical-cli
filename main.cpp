#include <chrono>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <vector>
#include <cmath>

#include "body.h"

#define M_PI 3.14159265358979323846

#define DEPTH 200

void getWindowSize(int& screenWidth, int& screenHeight){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    screenWidth = w.ws_col / 2; // / 2 because I put spaces beetween every points
    screenHeight = w.ws_row - 1; // -1 because there is one extra row
}


void calculateIsomtricPosition(int& isometricX,int& isometricY, float centerX, float centerY, float centerZ,
float cameraAngle, float cameraX, float cameraY, int x, int y, int z){
    float localX = x - centerX;
    float localY = y - centerY;
    float localZ = z - centerZ;
    float rotatedX = localX * cos(cameraAngle) - localZ * sin(cameraAngle);
    float rotatedZ = localX * sin(cameraAngle) + localZ * cos(cameraAngle);
    rotatedX += centerX;
    rotatedZ += centerZ;
    isometricX = static_cast<int>((rotatedX - rotatedZ) + cameraX);
    isometricY = static_cast<int>((rotatedX + rotatedZ)/2 - y + cameraY);
}


void generateScreen(int screenWidth, int screenHeight, char spaceMaterial, std::vector<Body>& bodies, float commonZ, std::vector<std::vector<char>>& screen,
float centerX, float centerY, float& cameraAngle, float cameraSpeed, bool hasToRollback){
    //clear screen
    screen.assign(screenHeight, std::vector<char>(screenWidth, spaceMaterial));
    cameraAngle += cameraSpeed;
    if(cameraAngle > 2  * M_PI){cameraAngle -= 2 * M_PI;}

    //follow center
    float cameraX = screenWidth / 2.0f - (centerX - commonZ);
    float cameraY = screenHeight / 2.0f - ((centerX + commonZ) / 2.0f - centerY);

    for(int i = 0; i < bodies.size(); i++){
        Body& body = bodies[i];
        for(int j = body.trailList.size() - 1; j >= 0 ; j--){
            std::vector<float>& trailUnit = body.trailList[j];
            int x = trailUnit[0];
            int y = trailUnit[1];
            int z = trailUnit[2];

            //rollback everything when we go to deep in space, so commonZ doesn't grow iniftly
            if(hasToRollback){
                trailUnit[2] -= DEPTH;
                z -= DEPTH;
            }


            //isometric view
            int isometricX, isometricY;
            calculateIsomtricPosition(isometricX, isometricY, centerX, centerY, commonZ, cameraAngle, cameraX, cameraY, x, y, z);

            if(isometricX >= 0 && isometricX < screenWidth && isometricY >= 0 && isometricY < screenHeight){
                if(j == body.trailList.size() - 1){ //display body
                    screen[isometricY][isometricX] = body.skin;
                }
                else{ //display body's trail
                    screen[isometricY][isometricX] = body.trailSkin;
                }
                
            }   
            
        }
    }
}

void dipslayScreen(std::vector<std::vector<char>>& screen, int screenWidth, int screenHeight){
    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            std::cout << " " << screen[y][x];
        }
        std::cout << "\n";
    }  
}

void spawnBody(std::vector<Body>& bodies, float angleSpeed, int screenWidth, int screenHeight, int maxTrailLength){
    Body body = Body((float)screenWidth / 2 + 10, (float)screenHeight / 2, angleSpeed, '@', '*', maxTrailLength);
    bodies.push_back(body);
}

int main(){
    int screenWidth = 80;
    int screenHeight = 50;

    getWindowSize(screenWidth, screenHeight);
    
    char spaceMaterial = ' ';
    std::vector<std::vector<char>> screen;
    screen.resize(screenHeight, std::vector<char>(screenWidth, spaceMaterial));

    std::vector<Body> bodies;

    float commonZ = 0;
    float zSpeed = 0.5;

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    float cameraAngle = 0;
    float cameraSpeed = 0.005;

    int maxTrailLength = 200;

    bool hasToRollback = false;

    spawnBody(bodies, 0.1, screenWidth, screenHeight, maxTrailLength);

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
        generateScreen(screenWidth, screenHeight, spaceMaterial, bodies, commonZ, screen, centerX, centerY, cameraAngle, cameraSpeed, hasToRollback);
        dipslayScreen(screen, screenWidth, screenHeight);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return 0;
}