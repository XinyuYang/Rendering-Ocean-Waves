#include "OceanWave.h"

#define FONTSTASH_IMPLEMENTATION
#include <fontstash.h>
#define GLFONTSTASH_IMPLEMENTATION
#include <glfontstash.h>

#include <config/VRDataIndex.h>

#include <glm/gtx/orthonormalize.hpp>

using namespace basicgraphics;
using namespace std;
using namespace glm;

//// settings
//const unsigned int SCR_WIDTH = 1280;
//const unsigned int SCR_HEIGHT = 720;

OceanWave::OceanWave(int argc, char** argv) : VRApp(argc, argv)
{
	_lastTime = 0.0;
    _curFrameTime = 0.0;
    
    eye_world = glm::vec3(0, 1000, 1000);
}

OceanWave::~OceanWave()
{
	shutdown();
}

void OceanWave::onAnalogChange(const VRAnalogEvent &event) {
    // This routine is called for all Analog_Change events.  Check event->getName()
    // to see exactly which analog input has been changed, and then access the
    // new value with event->getValue().
    
	if (event.getName() == "FrameStart") {
		_lastTime = _curFrameTime;
		_curFrameTime = event.getValue();
	}


}

void OceanWave::onButtonDown(const VRButtonEvent &event) {
    // This routine is called for all Button_Down events.  Check event->getName()
    // to see exactly which button has been pressed down.
	
	//std::cout << "ButtonDown: " << event.getName() << std::endl;
    turntable->onButtonDown(event);
}

void OceanWave::onButtonUp(const VRButtonEvent &event) {
    // This routine is called for all Button_Up events.  Check event->getName()
    // to see exactly which button has been released.
     turntable->onButtonUp(event);

    std::cout << "ButtonUp: " << event.getName() << std::endl;
}

void OceanWave::onCursorMove(const VRCursorEvent &event) {
	// This routine is called for all mouse move events. You can get the absolute position
	// or the relative position within the window scaled 0--1.
    turntable->onCursorMove(event);
//    std::cout << "MouseMove: "<< event.getName() << " " << event.getPos()[0] << " " << event.getPos()[1] << std::endl;
}

void OceanWave::onTrackerMove(const VRTrackerEvent &event) {
    // This routine is called for all Tracker_Move events.  Check event->getName()
    // to see exactly which tracker has moved, and then access the tracker's new
    // 4x4 transformation matrix with event->getTransform().

	// We will use trackers when we do a virtual reality assignment. For now, you can ignore this input type.
}

void OceanWave::onRenderGraphicsContext(const VRGraphicsState &renderState) {
    // This routine is called once per graphics context at the start of the
    // rendering process.  So, this is the place to initialize textures,
    // load models, or do other operations that you only want to do once per
    // frame.
    
	// Is this the first frame that we are rendering after starting the app?
    if (renderState.isInitialRenderCall()) {

		//For windows, we need to initialize a few more things for it to recognize all of the
		// opengl calls.
		#ifndef __APPLE__
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				std::cout << "Error initializing GLEW." << std::endl;
			}
		#endif     


        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_MULTISAMPLE);

		// This sets the background color that is used to clear the canvas
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// This load shaders from disk, we do it once when the program starts up.
		// build and compile shaders
        reloadShaders();
        
        // load model
        // -----------
        _modelMesh.reset(new Model("Ocean3.obj", 0.05, glm::vec4(1.0)));
        
        // load ocean surface texture
        // -----------
        _bumpMap = Texture::create2DTextureFromFile("txt_002_bump.jpg");
        _bumpMap->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        _bumpMap->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        
        _reflectionTextureMap = Texture::create2DTextureFromFile("sky.jpg");
        _reflectionTextureMap->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        _reflectionTextureMap->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        float radius = 5.0;
        _lightPosition = vec4(-1.7*radius, 0.3*radius, -1.0*radius, 1.0);
        
        // Initialize the texture environment map
        // Order should be:
        // +X (right)
        // -X (left)
        // +Y (top)
        // -Y (bottom)
        // +Z (front)
        // -Z (back)
        string textureFiles[] = {"right.jpg", "left.jpg", "up.jpg", "down.jpg", "forward.jpg", "back.jpg"};
//        string textureFiles[] = {"desert_evening_east.jpg", "desert_evening_west.jpg", "desert_evening_up.jpg", "desert_evening_down.jpg", "desert_evening_north.jpg", "desert_evening_south.jpg"};
        environmentMap = Texture::createCubeMapFromFiles(textureFiles, true, 4);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        environmentMap->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        environmentMap->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        skyBox.reset(new Skybox(environmentMap));
        
        turntable.reset(new TurntableManipulator());
        turntable->setCenterPosition(vec3(0,0,0));
    }
}

void OceanWave::onRenderGraphicsScene(const VRGraphicsState &renderState) {
    // This routine is called once per eye/camera.  This is the place to actually
    // draw the scene.
    
	// clear the canvas and other buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Setup the view matrix to set where the camera is located in the scene
    glm::vec3 eye_world = glm::vec3(0, 10, 5);
    
    // Setup the camera with a good initial position and view direction to see the table
//    glm::mat4 view = glm::lookAt(eye_world, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 view = turntable->frame();

	// Setup the projection matrix so that things are rendered in perspective
	GLfloat windowHeight = renderState.index().getValue("WindowHeight");
	GLfloat windowWidth = renderState.index().getValue("WindowWidth");
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.01f, 100.0f);
    
    // Setup the model matrix
    glm::mat4 model = glm::mat4(1.0);

	// Tell opengl we want to use this specific shader.
	_shader.use();
	
	_shader.setUniform("view_mat", view);
	_shader.setUniform("projection_mat", projection);
	
	_shader.setUniform("model_mat", model);
	_shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
    _shader.setUniform("eye_world", eye_world);
    
    
    // Properties of the material the model is made out of (the "K" terms in the equations discussed in class)
    // These values should make the model look like it is made out of a metal, like brass
    vec3 ambientReflectionCoeff(0.5, .5, .5);
    vec3 diffuseReflectionCoeff(0.7, 0.7, 0.7);
    vec3 specularReflectionCoeff(0.9, 0.9, 0.9);
    float m = 0.55;
    float r0 = 0.7;
    
//    float absorptionCoeff;
//    vec3 C(0.65, 0.8, 0.95);
//    float Kr1 =  1*
//    float Kr= (K);
//    float Kt();
//    float It();
//    It = (1-Ka)*C;
    
    // Properties of the light source (the "I" terms in the equations discussed in class)
    // These values are for a white light so the r,g,b intensities are all the same
    vec3 ambientLightIntensity(0.3, 0.3, 0.3);
    vec3 diffuseLightIntensity(0.6, 0.6, 0.6);
    vec3 specularLightIntensity(1.0, 1.0, 1.0);
    
    
    float glassR0 = 0.0200;
    vec3 glassEta(0.65, 0.67, 0.68);

    // Pass these parameters into your shader programs... in shader programs these are called "uniform variables"
    
    // TODO: Set shader material properties (k coeffs)
    
    //send the values to the shaders
    _shader.setUniform("ambientReflectionCoeff", ambientReflectionCoeff);
    _shader.setUniform("diffuseReflectionCoeff", diffuseReflectionCoeff);
    _shader.setUniform("specularReflectionCoeff", specularReflectionCoeff);
    
    _shader.setUniform("ambientLightIntensity", ambientLightIntensity);
    _shader.setUniform("diffuseLightIntensity", diffuseLightIntensity);
    _shader.setUniform("specularLightIntensity", specularLightIntensity);
    
//    
//    
//    _shader.setUniform("absorptionCoeff", absorptionCoeff);
//    _shader.setUniform("C", C);
//    _shader.setUniform("Kr", Kr);
//    
//    _shader.setUniform("Kt", Kt);
//    _shader.setUniform("It", It);

    
    // Cook-Torrance specific properties, m is roughness of water surface
    //Todo: calculate the water r0=0.0200
    _shader.setUniform("r0", r0);
    _shader.setUniform("m", m);
    
    _shader.setUniform("glassR0", glassR0);
    _shader.setUniform("glassEta", glassEta);
    
    // TODO: Set shader light properties (intensity and position)
    _shader.setUniform("lightPosition", _lightPosition);
    
    // Set texture uniform
    //This sets the texture to associate with the cubeMap sampler in the shader which is bound to texture unit 1;
    _bumpMap->bind(1);
    _shader.setUniform("_bumpMap", 1);
    
    _reflectionTextureMap->bind(2);
    _shader.setUniform("_reflectionTextureMap", 2);
    
    //This sets the texture to associate with the cubeMap sampler in the shader which is bound to texture unit 3;
    environmentMap->bind(3);
    _shader.setUniform("environmentMap", 3);
    
    vec3 eyePosition = turntable->getPos();
    _shader.setUniform("eye_world", eyePosition);
    
    // Draw the model
    _modelMesh->draw(_shader);
    
    // Draw the skybox. Should be the last thing to draw
    skyBox->draw(view, projection);

}

void OceanWave::reloadShaders()
{
	_shader.compileShader("OceanWave.vert", GLSLShader::VERTEX);
	_shader.compileShader("OceanWave.frag", GLSLShader::FRAGMENT);
	_shader.link();
	_shader.use();
}


