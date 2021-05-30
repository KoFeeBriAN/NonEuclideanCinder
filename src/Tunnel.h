#include "cinder/gl/Batch.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/Texture.h"
#include "glm/glm.hpp"
#include "vector"

using namespace ci;

class Tunnel {
public:
    void setupTunnel();
    void setTexture(DataSourceRef source);
    void setGlsl(gl::GlslProgRef glsl);
    void setCount(int count);
    void setPosition(vec3 position);
    void draw();

private:
    std::vector<gl::BatchRef> mBatches;
    gl::GlslProgRef mGlsl = gl::getStockShader(gl::ShaderDef().texture());
    gl::TextureRef mTexture;
    Color mColor = Color(1, 0, 0);

    vec3 mFace = vec3(0, 0, 1);
    vec3 mPosition = vec3(0, 3, 0); // middle of the door of the tunnel

    // tunnel properties
    float mHeight = 6.0f;
    float mWidth = 4.5f;
    float mThickness = 1.0f;
    float mLong = 6.0f;
    int mCount = 1;
};
