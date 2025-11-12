#include <chrono>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846
#define WIDTH 50
#define HEIGHT 20
#define DEAPTH 50

void initializeSpace(std::vector<std::vector<std::vector<char>>>& space, std::vector<std::vector<char>>& screen){
        space.resize(DEAPTH, std::vector<std::vector<char>>(HEIGHT, std::vector<char>(WIDTH, '.')));
        screen.resize(HEIGHT, std::vector<char>(WIDTH, '.'));
}

void moveBody(std::vector<float>& position, std::vector<std::vector<float>>& trailList, std::vector<std::vector<std::vector<char>>>& space){
    float x = position[0];
    float y = position[1];
    float z = position[2];
    
    z += 1;

    float cx = WIDTH / 2.0f;
    float cy = HEIGHT / 2.0f;

    float angleDiff = M_PI / 6;
    float r = std::sqrt(x*x + y*y);
    float theta = atan2(x, y);
    float nextTheta = theta + angleDiff;

    x = cx + r * std::cos(nextTheta);
    y = cy + r * std::cos(nextTheta);


    trailList.push_back({x,y,z});

    for (int i = 0; i < trailList.size(); i++) {
        std::vector<float> trailPoint = trailList[i];
        space[trailPoint[0]][trailPoint[1]][trailPoint[2]] = '*';
    }
    
}

void generateScreen(std::vector<std::vector<std::vector<char>>>& space, std::vector<std::vector<char>>& screen){
    for (int z = 0; z < DEAPTH; z++) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if(space[z][y][x] != '.'){
                    screen[y][x] = space[z][y][x];
                }
            }
        }   
    }
}

void dipslayScreen(std::vector<std::vector<char>>& screen){
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            std::cout << screen[y][x];
        }
        std::cout << "\n";
    }  
}

int main(){

    std::vector<std::vector<std::vector<char>>> space;
    std::vector<std::vector<char>> screen;

    std::vector<float> position = {WIDTH / 2 + 5,HEIGHT/2,0};
    std::vector<std::vector<float>> trailList;
    initializeSpace(space, screen);

    while(true){
        std::cout << "\033[H\033[J";
        std::cout << "oui" << std::endl;
        moveBody(position, trailList, space);
        generateScreen(space, screen);
        dipslayScreen(screen);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}