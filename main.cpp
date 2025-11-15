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
#define WIDTH 80
#define HEIGHT 40
#define DEPTH 500


void generateScreen(char spaceMaterial, std::vector<Body> bodies, float commonZ, std::vector<std::vector<char>>& screen, float centerX, float centerY){
    //clear screen
    screen.assign(HEIGHT, std::vector<char>(WIDTH, spaceMaterial));



    float cameraX = WIDTH / 2.0f - (centerX - commonZ);
    float cameraY = HEIGHT / 2.0f - ((centerX + commonZ) / 2.0f - centerY);


    for(int i = 0; i < bodies.size(); i++){
        Body& body = bodies[i];
        for(int j = body.trailList.size() - 1; j >= 0 ; j--){
            std::vector<float> trailUnit = body.trailList[j];
            int x = trailUnit[0];
            int y = trailUnit[1];
            int z = trailUnit[2];
            //isometric view
            int sx = static_cast<int>((x - z) + cameraX);
            int sy = static_cast<int>((x + z)/2 - y + cameraY);

            if(sx >= 0 && sx < WIDTH && sy >= 0 && sy < HEIGHT){
                if(j == body.trailList.size() - 1){ //display body
                    screen[sy][sx] = body.skin;
                }
                else{ //display body's trail
                    screen[sy][sx] = body.trailSkin;
                }
                
            }   
            
        }
    }
}

void dipslayScreen(std::vector<std::vector<char>>& screen){
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            std::cout << " " << screen[y][x];
        }
        std::cout << "\n";
    }  
}

void spawnBody(std::vector<Body>& bodies, float angleSpeed){
    Body body = Body((float)WIDTH / 2 + 10, (float)HEIGHT / 2, angleSpeed, '@', '*');
    bodies.push_back(body);
}

int main(){
    char spaceMaterial = ' ';
    std::vector<std::vector<char>> screen;
    screen.resize(HEIGHT, std::vector<char>(WIDTH, spaceMaterial));

    std::vector<Body> bodies;

    float commonZ = 0;
    float zSpeed = 0.5;

    float centerX = WIDTH / 2.0f;
    float centerY = HEIGHT / 2.0f;

    spawnBody(bodies, 0.1);

    while(true){
        std::cout << "\033[H\033[J";
        commonZ += zSpeed;
        if(commonZ >= DEPTH){
            //commonZ -= DEPTH;
        }
        for(int i = 0; i < bodies.size(); i++){
            Body& body = bodies[i];
            body.move(centerX, centerY, commonZ);
        }
        generateScreen(spaceMaterial, bodies, commonZ, screen, centerX, centerY);
        dipslayScreen(screen);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return 0;
}