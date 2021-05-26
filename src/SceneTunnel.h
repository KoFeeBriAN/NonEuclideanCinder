// #pragma once

// #include "Scene.h"
// #include <vector>

// #include "CameraFP.h"
// #include "cinder/gl/Batch.h"
// #include "cinder/gl/Texture.h"

// using namespace ci;

// class SceneTunnel : public Scene {
// private:
//     CameraFP mCam;
//     gl::TextureRef mTexture;
//     gl::GlslProgRef mGlsl;
//     // std::vector<gl::BatchRef> mBatches;
//     geom::Cube mCube;
//     float cubex = 1;

//     // Camera attributes
//     ivec2 lastPos;
//     ivec2 currentPos;

//     double mlastTime;
//     double mTimeOffset;
//     bool firstMouseMove = true;

// public:
//     ~SceneTunnel() = default;
//     void setup(const std::unordered_map<std::string, DataSourceRef>& assets) override;
//     void update(double currentTime) override;
//     void draw() override;
//     Camera* getCamera() override;
//     void handleMouseMove(MouseEvent event) override;
//     void handleKeyDown(KeyEvent event) override;

// private:
//     void processInput();
// };
