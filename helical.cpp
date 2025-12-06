#include "body.h"
#include "helical.h"

#include <string>
#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846
#define DEPTH 100



float calculateIsomtricPosition(int& isometricX,int& isometricY, float centerX, float centerY, float centerZ,
float cameraAngle, float cameraX, float cameraY, int x, int y, int z, float cameraZoom){
    //transforms a 3D space into a 2D space, with an isometric view
    //it retruns the depths of the point that will be usefull to know which point is over which one

    //get the local coordinates based on the center
    float localX = x - centerX;
    float localY = y - centerY;
    float localZ = z - centerZ;

    //we edit x and z, because we turn around y axe
    // rotatedX and rotatedY are the new coordinates that take into account the cameraAngle
    float rotatedX = localX * cos(cameraAngle) - localZ * sin(cameraAngle);
    float rotatedZ = localX * sin(cameraAngle) + localZ * cos(cameraAngle);

    //we get back the global coordinates (no more local)
    rotatedX += centerX;
    rotatedZ += centerZ;

    //transform 3 axes into only 2, and take into account the camera position and its zoom
    isometricX = int(((rotatedX - rotatedZ) * cameraZoom) + cameraX);
    isometricY = int(((rotatedX + rotatedZ) / 2 - y) * cameraZoom + cameraY);

    return rotatedZ;
}


void generateScreen(int screenWidth, int screenHeight, std::string spaceMaterial, std::vector<Body>& bodies,
    float commonZ, std::vector<std::vector<std::string>>& screen, float centerX, float centerY, float& cameraAngle,
    float cameraSpeed, bool hasToRollback, float cameraZoom){
    //clear screen
    screen.assign(screenHeight, std::vector<std::string>(screenWidth, spaceMaterial));

    //increase the angle of the camera and set a modulo on it
    cameraAngle += cameraSpeed;
    if(cameraAngle > 2  * M_PI){
        cameraAngle -= 2 * M_PI;
    }

    //calculate the center point in isometric view to know where to focus the camera on
    float baseIsometricX = (centerX - commonZ) * cameraZoom;
    float baseIsometricY = ((centerX + commonZ) / 2 - centerY) * cameraZoom;

    //focus the camera on the center
    float cameraX = screenWidth  / 2.0f - baseIsometricX;
    float cameraY = screenHeight / 2.0f - baseIsometricY;

    //to remember the depth of every chars, we create a vector like screen, with the depth of every points
    std::vector<std::vector<float>> depthMemory(screenHeight, std::vector<float>(screenWidth, -1e9)); 

    for(int i = 0; i < bodies.size(); i++){
        Body& body = bodies[i];

        //create the body's skin
        body.generateSkin(commonZ);
        
        //we browse trailList and headList, by starting by the last element of headList, and ending at the first element of trailList
        for(int j = body.trailList.size() - 1 + body.headList.size() - 1; j >= 0 ; j--){
            bool isHead;


            std::vector<float>* bodyPoint;// body point is a head or a trail, depending of j
            //know if j is an index of the head or the trail
            if(j >= body.trailList.size() - 1){
                isHead = true;
                bodyPoint = &body.headList[j - (body.trailList.size() - 1)];
            }
            else{
                isHead = false;
                bodyPoint = &body.trailList[j];
            }

            int x = (*bodyPoint)[0];
            int y = (*bodyPoint)[1];
            int z = (*bodyPoint)[2];

            //rollback everything when we go to deep in space, so commonZ doesn't grow iniftly
            if(hasToRollback && !isHead){
                (*bodyPoint)[2] -= DEPTH;
                z -= DEPTH;
            }

            //isometric view
            int isometricX, isometricY;
            float depth = calculateIsomtricPosition(isometricX, isometricY, centerX, centerY, commonZ,
            cameraAngle, cameraX, cameraY, x, y, z, cameraZoom);
            
            //make sure that we stay in bound of the screen
            if(isometricX >= 0 && isometricX < screenWidth && isometricY >= 0 && isometricY < screenHeight){
                if(depth > depthMemory[isometricY][isometricX]){ // we only display the char if it is over the last one
                    depthMemory[isometricY][isometricX] = depth;
                    
                    if(j == body.trailList.size() - 1 || isHead){ 
                        //display Head
                        screen[isometricY][isometricX] = getColoredString(body.color, body.skin);
                    }
                    else{ 
                        //display trail
                        screen[isometricY][isometricX] = getColoredString(body.color, body.trailSkin);
                    }
                }     
            }   
        }
    }
}
std::string getColoredString(std::tuple<float, float, float> color, std::string skin){
    //get the colored string corresponding to the ratio of index / maxIndex, on a color gradient

    int r = std::get<0>(color);
    int g = std::get<1>(color);
    int b = std::get<2>(color);

    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + skin + "\033[0m";
}

void spawnBodies(std::vector<Body>& bodies, std::vector<BodyAttributes> bodiesAttributes, int maxTrailLength){
    //adds a list of bodies in bodies

    for(int i = 0; i < bodiesAttributes.size(); i++){
        float r = bodiesAttributes[i].r;
        float g = bodiesAttributes[i].g;
        float b = bodiesAttributes[i].b;
        std::tuple<float, float, float> color = {r, g, b};

        Body body = Body(bodiesAttributes[i].x, bodiesAttributes[i].y, bodiesAttributes[i].angleSpeed, bodiesAttributes[i].headSkin,
            bodiesAttributes[i].trailSkin, maxTrailLength, color, bodiesAttributes[i].skinSize);
        bodies.push_back(body);
    }
}

