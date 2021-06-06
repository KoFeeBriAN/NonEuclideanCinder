#pragma once

#include "cinder/gl/gl.h"
#include <unordered_map>
#include <vector>

using namespace ci;

class Room {
protected:
    // Class Attributes
    vec3 mRoomSize; // Room Size
    vec3 mRoomOrigin; // Origin Position
    float mWallThickness; // Wall thickness

    float mRotDeg = 0.0f;

    // Floor attributes
    gl::BatchRef mFloor; // Floor Mesh
    gl::TextureRef mFloorTexture; // Floor Texture
    gl::GlslProgRef mFloorShader; // Floor Shader Prog

    // Walls attributes
    std::vector<gl::BatchRef> mWalls; // Walls Mesh
    std::vector<vec3> mWallPositions; // Wall Positions
    gl::TextureRef mWallTexture; // Walls Texture
    gl::GlslProgRef mWallShader; // Walls Shader Prog

public:
    Room() = delete;
    Room(vec3 size, vec3 origin);
    ~Room() = default;
    void setup(const std::unordered_map<std::string, DataSourceRef>& assets);
    void update();
    void draw();

    void setFloorTexture(gl::TextureRef texture);
    void setWallTexture(gl::TextureRef texture);
    void setRotateRoom(float deg);
    vec3 getRoomSize();
};