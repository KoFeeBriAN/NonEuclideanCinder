#include "Room.h"

#include "cinder/gl/gl.h"

using namespace ci;

Room::Room(vec3 size, vec3 origin)
    : mRoomSize(size)
    , mRoomOrigin(origin)
{
}

void Room::setup(const std::unordered_map<std::string, DataSourceRef>& assets)
{
    // Setup Floor
    mFloorTexture = gl::Texture::create( loadImage( assets.at("checkerboard.png") ) );
    mFloorShader = gl::getStockShader( gl::ShaderDef().texture() );
    mFloor = gl::Batch::create(
        geom::Plane()
            .origin( mRoomOrigin )
            .size({ mRoomSize[0], mRoomSize[1] })
            .normal({ 0, 1, 0 }),
        mFloorShader
    );

    // Setup Walls
    mWallThickness = 2.0;
    mWallTexture = gl::Texture::create( loadImage( assets.at("rock-toon.jpg") ) );
    mWallShader = gl::getStockShader( gl::ShaderDef().texture() );

    mWallPositions.push_back({ mRoomSize.x / 2, mRoomSize.y / 2, 0 });
    mWallPositions.push_back({ -mRoomSize.x / 2, mRoomSize.y / 2, 0 });
    mWallPositions.push_back({ 0, mRoomSize.y / 2, mRoomSize.z / 2 });
    mWallPositions.push_back({ 0, mRoomSize.y / 2, -mRoomSize.z / 2 });

    auto wall = geom::Cube();
    mWalls.push_back(gl::Batch::create(wall.size({}) >> geom::Translate(mWallPositions[0]), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({}) >> geom::Translate(mWallPositions[1]), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({}) >> geom::Translate(mWallPositions[2]), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({}) >> geom::Translate(mWallPositions[3]), mWallShader));
}

void Room::update()
{
    return;
}

void Room::draw()
{
    mFloor->draw();
    for (auto &wall: mWalls) wall->draw();
}