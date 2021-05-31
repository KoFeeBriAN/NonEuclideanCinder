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
    auto fmt = gl::Texture::Format();
    fmt.setWrap(GL_REPEAT, GL_REPEAT);
    fmt.enableMipmapping(GL_TRUE);
    mFloorTexture = gl::Texture::create( loadImage( assets.at("checkerboard.png") ), fmt );
    mFloorShader = gl::getStockShader( gl::ShaderDef().texture() );
    mFloor = gl::Batch::create(
        geom::Plane()
            .origin( mRoomOrigin )
            .size({ mRoomSize[0], mRoomSize[2] })
            .normal({ 0, 1, 0 }),
        mFloorShader
    );

    // Setup Walls
    mWallThickness = 2.0;
    mWallTexture = gl::Texture::create( loadImage( assets.at("rock-toon.jpg") ) );
    mWallShader = gl::getStockShader( gl::ShaderDef().texture() );

    // ! DO NOT SHUFFLE THIS ORDER
    mWallPositions.push_back({ mRoomSize.x / 2, mRoomSize.y / 2, 0 });
    mWallPositions.push_back({ -mRoomSize.x / 2, mRoomSize.y / 2, 0 });
    mWallPositions.push_back({ 0, mRoomSize.y / 2, mRoomSize.z / 2 });
    mWallPositions.push_back({ 0, mRoomSize.y / 2, -mRoomSize.z / 2 });

    auto wall = geom::Cube();
    mWalls.push_back(gl::Batch::create(wall.size({mWallThickness, mRoomSize.y, mRoomSize.z}) >> geom::Translate(mRoomOrigin + mWallPositions[0]), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({mWallThickness, mRoomSize.y, mRoomSize.z}) >> geom::Translate(mRoomOrigin + mWallPositions[1]), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({mRoomSize.x, mRoomSize.y, mWallThickness}) >> geom::Translate(mRoomOrigin + mWallPositions[2]), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({mRoomSize.x, mRoomSize.y, mWallThickness}) >> geom::Translate(mRoomOrigin + mWallPositions[3]), mWallShader));
    // ! DO NOT SHUFFLE THIS ORDER
}

void Room::update()
{
    return;
}

void Room::draw()
{
    mFloorTexture->bind();
    mFloor->draw();

    mWallTexture->bind();
    for (auto &wall: mWalls) wall->draw();
}

void Room::setFloorTexture(gl::TextureRef texture)
{
    mFloorTexture = texture;
}

vec3 Room::getRoomSize()
{
    return mRoomSize;
}