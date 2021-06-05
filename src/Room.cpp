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
    auto floor = geom::Plane().size({mRoomSize.x , mRoomSize.z})
        >> geom::Rotate(glm::radians(mRotDeg), vec3(0, 1, 0))
        >> geom::Translate(mRoomOrigin);

    mFloorTexture = gl::Texture::create(loadImage(assets.at("checkerboard.png")), fmt);
    mFloorShader = gl::getStockShader(gl::ShaderDef().texture());
    mFloor = gl::Batch::create(floor, mFloorShader);

    // Setup Walls
    mWallThickness = 1.0f;
    mWallTexture = gl::Texture::create(loadImage(assets.at("rock-toon.jpg")));
    mWallShader = gl::getStockShader(gl::ShaderDef().texture());

    // ! DO NOT SHUFFLE THIS ORDER
    mWallPositions.push_back({ mRoomSize.x / 2 - mWallThickness / 2, mRoomSize.y / 2, 0 });
    mWallPositions.push_back({ -mRoomSize.x / 2 + mWallThickness / 2, mRoomSize.y / 2, 0 });
    mWallPositions.push_back({ 0, mRoomSize.y / 2, mRoomSize.z / 2 - mWallThickness / 2 });
    mWallPositions.push_back({ 0, mRoomSize.y / 2, -mRoomSize.z / 2 + mWallThickness / 2 });

    auto wall = geom::Cube();
    mWalls.push_back(gl::Batch::create(wall.size({ mWallThickness, mRoomSize.y, mRoomSize.z * 4 / 5 }) 
        >> geom::Translate(mWallPositions[0] + vec3(0, 0, mRoomSize.z / 5 / 2)) 
        >> geom::Rotate(glm::radians(mRotDeg), vec3(0, 1, 0)) 
        >> geom::Translate(mRoomOrigin), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({ mWallThickness, mRoomSize.y, mRoomSize.z }) 
        >> geom::Translate(mWallPositions[1]) 
        >> geom::Rotate(glm::radians(mRotDeg), vec3(0, 1, 0)) 
        >> geom::Translate(mRoomOrigin), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({ mRoomSize.x * 4 / 5, mRoomSize.y, mWallThickness }) 
        >> geom::Translate(mWallPositions[2] + vec3(mRoomSize.x / 5 / 2, 0, 0)) 
        >> geom::Rotate(glm::radians(mRotDeg), vec3(0, 1, 0))
        >> geom::Translate(mRoomOrigin), mWallShader));
    mWalls.push_back(gl::Batch::create(wall.size({ mRoomSize.x, mRoomSize.y, mWallThickness }) 
        >> geom::Translate(mWallPositions[3]) 
        >> geom::Rotate(glm::radians(mRotDeg), vec3(0, 1, 0)) 
        >> geom::Translate(mRoomOrigin), mWallShader));
    // ! DO NOT SHUFFLE THIS ORDER
}

void Room::update()
{
    return;
}

void Room::draw()
{

    mWallTexture->bind();
    for (auto& wall : mWalls)
        wall->draw();

    mFloorTexture->bind();
    mFloor->draw();
}

void Room::setFloorTexture(gl::TextureRef texture)
{
    mFloorTexture = texture;
}

void Room::setWallTexture(gl::TextureRef texture)
{
    mWallTexture = texture;
}

vec3 Room::getRoomSize()
{
    return mRoomSize;
}

void Room::setRotateRoom(float deg)
{
    mRotDeg = deg;
}