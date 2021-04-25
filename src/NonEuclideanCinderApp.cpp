#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class NonEuclideanCinderApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void NonEuclideanCinderApp::setup()
{
}

void NonEuclideanCinderApp::mouseDown( MouseEvent event )
{
}

void NonEuclideanCinderApp::update()
{
}

void NonEuclideanCinderApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( NonEuclideanCinderApp, RendererGl )
