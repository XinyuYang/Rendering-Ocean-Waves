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
    
    std::unique_ptr<basicgraphics::Mesh> _mesh;
    std::unique_ptr<basicgraphics::Mesh> _meshTop;
    std::unique_ptr<basicgraphics::Mesh> _meshBottom;
    
    virtual void reloadShaders();
    basicgraphics::GLSLProgram _shader;

	void initializeText();
	void drawText(const std::string text, float xPos, float yPos, GLfloat windowHeight, GLfloat windowWidth);
	struct FONScontext* fs;
    basicgraphics::GLSLProgram _textShader;
    
    /** Mesh and texture**/
    std::vector<Mesh::Vertex> topVertexArray;
    std::vector<int> topIndexArray;
    std::vector<std::shared_ptr<Texture>> topTextures;
    
    std::vector<Mesh::Vertex> bottomVertexArray;
    std::vector<int> bottomIndexArray;
    std::vector<std::shared_ptr<Texture>> bottomTextures;
    
    std::vector<Mesh::Vertex> cpuVertexArray;
    std::vector<int> cpuIndexArray;
    std::vector<std::shared_ptr<Texture>> textures;
    
    
};


#endif //EXAMPLEAPP_H
