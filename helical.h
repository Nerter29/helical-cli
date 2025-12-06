#ifndef HELICAL_H
#define HELICAL_H

#include <string>
#include <vector>
#include <cmath>

#include "body.h"

typedef struct {
    float x;         
    float y;
    float angleSpeed;
    float skinSize;
    std::string headSkin;
    std::string trailSkin;
    int r, g, b;

} BodyAttributes;

float calculateIsomtricPosition(int& isometricX,int& isometricY, float centerX, float centerY, float centerZ,
float cameraAngle, float cameraX, float cameraY, int x, int y, int z, float cameraZoom);

void generateScreen(int screenWidth, int screenHeight, std::string spaceMaterial, std::vector<Body>& bodies, float commonZ, std::vector<std::vector<std::string>>& screen,
float centerX, float centerY, float& cameraAngle, float cameraSpeed, bool hasToRollback, float cameraZoom);

std::string getColoredString(std::tuple<float, float, float> color, std::string skin);

void spawnBodies(std::vector<Body>& bodies, std::vector<BodyAttributes> bodiesAttributes, int maxTrailLength);



#endif // BODYHELICAL_H_H
