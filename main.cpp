#include <chrono>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846
#define WIDTH 30
#define HEIGHT 30
#define DEPTH 50
#define spaceSize 50

void initializeSpace(std::vector<std::vector<std::vector<char>>>& space, std::vector<std::vector<char>>& screen){
        space.resize(spaceSize, std::vector<std::vector<char>>(spaceSize, std::vector<char>(spaceSize, '.')));
        screen.resize(HEIGHT, std::vector<char>(WIDTH, '.'));
}

void moveBody(std::vector<std::vector<float>>& trailList, float& z, std::vector<std::vector<std::vector<char>>>& space){
    std::vector<float> currentPostion = trailList[trailList.size() - 1];
    float x = currentPostion[0];
    float y = currentPostion[1];
    z += M_PI / 24;
    if( z >= DEPTH){
        z -= DEPTH;
        space.assign(spaceSize, std::vector<std::vector<char>>(spaceSize, std::vector<char>(spaceSize, '.')));

        //TODO when max depth is reached need to find a way to rollback to z = 0, and make sure that the user notices nothing:
    }
    

    float cx = WIDTH / 2.0f;
    float cy = HEIGHT / 2.0f;

    float offsetX = x - cx; 
    float offsetY = y - cy;

    float theta = atan2(offsetY, offsetX);
    float angleDiff = M_PI / 24;
    float nextTheta = theta + angleDiff;

    float r = std::sqrt(offsetX * offsetX + offsetY * offsetY);
    x = cx + r * std::cos(nextTheta);
    y = cy + r * std::sin(nextTheta);


    trailList.push_back({x,y,z});
    std::cout << x << " " << y << " " << theta << " " << r <<"\n";
    if (x >= 0 && x < spaceSize &&
    y >= 0 && y < spaceSize &&
    z >= 0 && z < spaceSize)
{
    space[(int)z][(int)y][(int)x] = '*';
}
    
}

void generateScreen(std::vector<std::vector<std::vector<char>>>& space, float commonZ, std::vector<std::vector<char>>& screen,float &cameraX,float &cameraY){
    //clear screen
    screen.assign(HEIGHT, std::vector<char>(WIDTH, '.'));
    //transform a 3D space into a 2D screen
    cameraX = WIDTH / 2  + commonZ / 2 ;
    cameraY = HEIGHT / 2  - commonZ / 2  ;

    for (int z = 0; z < DEPTH; z++) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                char currentChar = space[z][y][x];
                if(currentChar != '.'){
                    //isometric view
                    
                    int sx = static_cast<int>((x - z) + cameraX);
                    int sy = static_cast<int>((x + z)/2 - y + cameraY);

                    if(sx >= 0 && sx < WIDTH && sy >= 0 && sy < HEIGHT)
                        screen[sy][sx] = currentChar;
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

int main(){

    std::vector<std::vector<std::vector<char>>> space;
    std::vector<std::vector<char>> screen;

    std::vector<std::vector<float>> trailList = {{WIDTH / 2 + 5,HEIGHT/2}}; // position is only 2d because we keep z apart : it's the same for everyone
    initializeSpace(space, screen);
    float z = 0;

    float cameraX = WIDTH / 2;
    float cameraY = HEIGHT / 2;

    while(true){
        std::cout << "\033[H\033[J";
        moveBody(trailList, z, space);
        generateScreen(space, z, screen, cameraX, cameraY);
        dipslayScreen(screen);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return 0;
}