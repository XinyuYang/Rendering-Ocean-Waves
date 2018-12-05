#ifndef OCEANWAVE_H
#define OCEANWAVE_H

#include <api/MinVR.h>
using namespace MinVR;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#ifdef _WIN32
#include "GL/glew.h"
#include "GL/wglew.h"
#elif (!defined(__APPLE__))
#include "GL/glxew.h"
#endif

// OpenGL Headers
#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#define GL_GLEXT_PROTOTYPES
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#include <BasicGraphics.h>
#include "TurntableManipulator.h"
using namespace basicgraphics;

class OceanWave : public VRApp {
public:
    
    /** The constructor passes argc, argv, and a MinVR config file on to VRApp.
     */
	OceanWave(int argc, char** argv);
    virtual ~OceanWave();

    
    /** USER INTERFACE CALLBACKS **/
    virtual void onAnalogChange(const VRAnalogEvent &state);
    virtual void onButtonDown(const VRButtonEvent &state);
    virtual void onButtonUp(const VRButtonEvent &state);
	virtual void onCursorMove(const VRCursorEvent &state);
    virtual void onTrackerMove(const VRTrackerEvent &state);
    
    
    /** RENDERING CALLBACKS **/
    virtual void onRenderGraphicsScene(const VRGraphicsState& state);
    virtual void onRenderGraphicsContext(const VRGraphicsState& state);
    
private:

	double _lastTime;
	double _curFrameTime;
    
    glm::vec3 eye_world;
    
    virtual void reloadShaders();
    
    glm::mat4 model;
    
    // Add bumpMap
    std::shared_ptr<Texture> _bumpMap;
    std::shared_ptr<Texture> _reflectionTextureMap;
    
    basicgraphics::GLSLProgram _shader;
    
    std::unique_ptr<Model> _modelMesh;
    std::shared_ptr<basicgraphics::TurntableManipulator> turntable;
    
    glm::vec4 _lightPosition;
    glm::vec3 absorptionCoeff;
    glm::vec3 Kr;
    glm::vec3 Kt;
    glm::vec3 It;
};


#endif //EXAMPLEAPP_H
