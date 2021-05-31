#pragma once

#include "cinder/gl/gl.h"
#include <vector>
#include <unordered_map>
       
using namespace ci;

class Room
{
protected:
    // Class Attributes
    vec3 mRoomSize;                             // Room Size
    vec3 mRoomOrigin;                           // Origin Position
    float mWallThickness;                       // Wall thickness

    // Floor attributes
    gl::BatchRef mFloor;                        // Floor Mesh
    gl::TextureRef mFloorTexture;               // Floor Texture
    gl::GlslProgRef mFloorShader;               // Floor Shader Prog

    // Walls attributes
    std::vector<gl::BatchRef> mWalls;           // Walls Mesh
    std::vector<vec3> mWallPositions;           // Wall Positions
    gl::TextureRef mWallTexture;                // Walls Texture
    gl::GlslProgRef mWallShader;                // Walls Shader Prog

public:
    Room() = delete;
    Room(vec3 size,vec3 origin);
    ~Room();
    void setup(const std::unordered_map<std::string, DataSourceRef>& assets);
    void update();
    void draw();

    void setFloorTexture(gl::TextureRef texture);
    vec3 getRoomSize();

};