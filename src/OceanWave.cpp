#include "ExampleApp.h"

#define FONTSTASH_IMPLEMENTATION
#include <fontstash.h>
#define GLFONTSTASH_IMPLEMENTATION
#include <glfontstash.h>

#include <config/VRDataIndex.h>

#include <glm/gtx/orthonormalize.hpp>

using namespace basicgraphics;
using namespace std;
using namespace glm;

ExampleApp::ExampleApp(int argc, char** argv) : VRApp(argc, argv)
{
	_lastTime = 0.0;
    _curFrameTime = 0.0;
}

ExampleApp::~ExampleApp()
{
	glfonsDelete(fs);
	shutdown();
}

void ExampleApp::onAnalogChange(const VRAnalogEvent &event) {
    // This routine is called for all Analog_Change events.  Check event->getName()
    // to see exactly which analog input has been changed, and then access the
    // new value with event->getValue().
    
	if (event.getName() == "FrameStart") {
		_lastTime = _curFrameTime;
		_curFrameTime = event.getValue();
	}


}

void ExampleApp::onButtonDown(const VRButtonEvent &event) {
    // This routine is called for all Button_Down events.  Check event->getName()
    // to see exactly which button has been pressed down.
	
	//std::cout << "ButtonDown: " << event.getName() << std::endl;
}

void ExampleApp::onButtonUp(const VRButtonEvent &event) {
    // This routine is called for all Button_Up events.  Check event->getName()
    // to see exactly which button has been released.

	//std::cout << "ButtonUp: " << event.getName() << std::endl;
}

void ExampleApp::onCursorMove(const VRCursorEvent &event) {
	// This routine is called for all mouse move events. You can get the absolute position
	// or the relative position within the window scaled 0--1.
	
	//std::cout << "MouseMove: "<< event.getName() << " " << event.getPos()[0] << " " << event.getPos()[1] << std::endl;
}

void ExampleApp::onTrackerMove(const VRTrackerEvent &event) {
    // This routine is called for all Tracker_Move events.  Check event->getName()
    // to see exactly which tracker has moved, and then access the tracker's new
    // 4x4 transformation matrix with event->getTransform().

	// We will use trackers when we do a virtual reality assignment. For now, you can ignore this input type.
}

void ExampleApp::onRenderGraphicsContext(const VRGraphicsState &renderState) {
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
		reloadShaders();

		initializeText();

        // Or using GEO_TRIANGLES
        
//        vec3 pos1(0, 0, 0);
//
//        vec3 normal(0, 0, 1);
//        vec2 texCoord1(0, 0);
//
//        Mesh::Vertex line;
//        line.position = pos1;
//        line.normal = normal;
//        line.texCoord0 = texCoord1;
//        cpuVertexArray.push_back(line);
//        cpuIndexArray.push_back(0);
        
        
        // drawing top and bottom
        // drawing top circle area
        Mesh::Vertex topLine;
        int topCount = 0;
        vec3 topNormal = vec3(0, 1, 0);
        
        vec3 point1(0, 1, 0);
        topLine.position = point1;
        topLine.normal = topNormal;
        topVertexArray.push_back(topLine);
        topIndexArray.push_back(topCount);
        topCount++;
        
        vec3 point2(1, 1, 0);
        topLine.position = point2;
        topVertexArray.push_back(topLine);
        topIndexArray.push_back(topCount);
        topCount++;
        
        vec3 point3(cos(radians(float(1/360))), 1, sin(radians(float(1/360))));
        topLine.position = point3;
        topVertexArray.push_back(topLine);
        topIndexArray.push_back(topCount);
        topCount++;
        
        for (int i = 1; i < 360; i++){
            vec3 point2 = point3;
            topLine.position = point2;
            topVertexArray.push_back(topLine);
            topIndexArray.push_back(topCount);
            topCount++;
            
            vec3 point3(cos(radians(float(i/360))), 1, sin(radians(float(i/360))));
            topLine.position = point3;
            topVertexArray.push_back(topLine);
            topIndexArray.push_back(topCount);
            topCount++;
        }
        
        
        const int topNumVertices = topVertexArray.size();
        const int topVertexByteSize = sizeof(Mesh::Vertex) * topNumVertices;
        const int topIndexByteSize = sizeof(int) * topIndexArray.size();
        
        _meshTop.reset(new Mesh(textures, GL_TRIANGLES, GL_STATIC_DRAW,
                             topVertexByteSize, topIndexByteSize, 0, topVertexArray,
                             topIndexArray.size(), topIndexByteSize, &topIndexArray[0]));
        
        _meshTop->setMaterialColor(vec4(0, 0, 0, 0));
        
        // drawing bottom circle area
        Mesh::Vertex bottomLine;
        int bottomCount = 0;
        vec3 bottomNormal = vec3(0, -1, 0);
        
        vec3 bottompoint1(0, -1, 0);
        bottomLine.position = bottompoint1;
        bottomLine.normal = bottomNormal;
        bottomVertexArray.push_back(bottomLine);
        bottomIndexArray.push_back(bottomCount);
        bottomCount++;
        
        vec3 bottompoint2(1, -1, 0);
        bottomLine.position = bottompoint2;
        bottomVertexArray.push_back(bottomLine);
        bottomIndexArray.push_back(bottomCount);
        bottomCount++;
        
        vec3 bottompoint3(cos(radians(float(1/360))), -1, sin(radians(float(1/360))));
        bottomLine.position = bottompoint3;
        bottomVertexArray.push_back(bottomLine);
        bottomIndexArray.push_back(bottomCount);
        bottomCount++;
        
        for (int i = 1; i < 360; i++){
            vec3 bottompoint2 = bottompoint3;
            bottomLine.position = bottompoint2;
            bottomVertexArray.push_back(bottomLine);
            bottomIndexArray.push_back(bottomCount);
            bottomCount++;
            
            vec3 bottompoint3(cos(radians(float(i/360))), -1, sin(radians(float(i/360))));
            bottomLine.position = bottompoint3;
            bottomVertexArray.push_back(bottomLine);
            bottomIndexArray.push_back(bottomCount);
            bottomCount++;
        }
        
        
        const int bottomNumVertices = bottomVertexArray.size();
        const int bottomVertexByteSize = sizeof(Mesh::Vertex) * bottomNumVertices;
        const int bottomIndexByteSize = sizeof(int) * bottomIndexArray.size();
        
        _meshBottom.reset(new Mesh(textures, GL_TRIANGLES, GL_STATIC_DRAW,
                                bottomVertexByteSize, bottomIndexByteSize, 0, bottomVertexArray,
                                bottomIndexArray.size(), bottomIndexByteSize, &bottomIndexArray[0]));
        
        _meshBottom->setMaterialColor(vec4(0, 0, 0, 0));
        
        // drawing the side
        int count = 0;
        vec3 pos1(1, -1, 0);
        vec3 pos2(1, 1, 0);
        vec3 normal(1, 0, 0);
        vec2 texCoord1(0, 0);
        vec2 texCoord2(1, 0);
        Mesh::Vertex line;
        
        line.position = pos1;
        line.normal = normal;
        line.texCoord0 = texCoord1;
        cpuVertexArray.push_back(line);
        cpuIndexArray.push_back(count);
        count++;
        
        line.position = pos2;
        line.texCoord0 = texCoord2;
        cpuVertexArray.push_back(line);
        cpuIndexArray.push_back(count);
        count++;
        
        for (int i = 0; i < 360; i++){
            vec3 pos1(cos(radians(float(i))), -1, sin(radians(float(i))));
            vec3 pos2(cos(radians(float(i))), 1, sin(radians(float(i))));
            vec3 normal(cos(radians(float(i))), 0, sin(radians(float(i))));
            
            float angle = (270 - i) % 360 / 360.f;
            cout << to_string(angle) << endl;
            vec2 texCoord1(angle, 1);
            vec2 texCoord2(angle, 0);
            Mesh::Vertex line;
            
            line.position = pos1;
            line.normal = normal;
            line.texCoord0 = texCoord1;
            cpuVertexArray.push_back(line);
            cpuIndexArray.push_back(count);
            count++;
            
            line.position = pos2;
            line.normal = normal;
            line.texCoord0 = texCoord2;
            cpuVertexArray.push_back(line);
            cpuIndexArray.push_back(count);
            count++;
            
        }
        
        line.position = pos1;
        line.normal = normal;
        line.texCoord0 = texCoord1;
        cpuVertexArray.push_back(line);
        cpuIndexArray.push_back(count);
        count++;
        
        line.position = pos2;
        line.texCoord0 = texCoord2;
        cpuVertexArray.push_back(line);
        cpuIndexArray.push_back(count);
        
        //Initialize the _mesh variable with a triangle mesh uploaded to the GPU.
        
        const int numVertices = cpuVertexArray.size();
        const int cpuVertexByteSize = sizeof(Mesh::Vertex) * numVertices;
        const int cpuIndexByteSize = sizeof(int) * cpuIndexArray.size();
        
        std::shared_ptr<Texture> tex =
        Texture::create2DTextureFromFile("campbells.jpg");
        textures.push_back(tex);
        
        _mesh.reset(new Mesh(textures, GL_TRIANGLE_STRIP, GL_STATIC_DRAW,
                             cpuVertexByteSize, cpuIndexByteSize, 0, cpuVertexArray,
                             cpuIndexArray.size(), cpuIndexByteSize, &cpuIndexArray[0]));

        
    }
}

void ExampleApp::onRenderGraphicsScene(const VRGraphicsState &renderState) {
    // This routine is called once per eye/camera.  This is the place to actually
    // draw the scene.
    
	// clear the canvas and other buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Setup the view matrix to set where the camera is located in the scene
    glm::vec3 eye_world = glm::vec3(0, 0, 5);
    glm::mat4 view = glm::lookAt(eye_world, glm::vec3(0,0,0), glm::vec3(0,1,0));

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
    
    
    // Draw the mesh
    _mesh->draw(_shader);
    _meshTop->draw(_shader);
    _meshBottom->draw(_shader);

}

void ExampleApp::drawText(const std::string text, float xPos, float yPos, GLfloat windowHeight, GLfloat windowWidth) {
	//float lh = 0;
	//fonsVertMetrics(fs, NULL, NULL, &lh);
	//double width = fonsTextBounds(fs, text.c_str(), NULL, NULL) + 40;
	//double height = lh + 40;

	_textShader.use();
	_textShader.setUniform("projection_mat", glm::ortho(0.f, windowWidth, windowHeight, 0.f, -1.f, 1.f));
	_textShader.setUniform("view_mat", glm::mat4(1.0));
	_textShader.setUniform("model_mat", glm::mat4(1.0));
	_textShader.setUniform("lambertian_texture", 0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fonsDrawText(fs, xPos, yPos, text.c_str(), NULL);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	_shader.use();

}

void ExampleApp::reloadShaders()
{
	_shader.compileShader("texture.vert", GLSLShader::VERTEX);
	_shader.compileShader("texture.frag", GLSLShader::FRAGMENT);
	_shader.link();
	_shader.use();
}

void ExampleApp::initializeText() {
	int fontNormal = FONS_INVALID;
	fs = nullptr;

	fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	if (fs == NULL) {
		assert(false);//Could not create stash
	}

	fontNormal = fonsAddFont(fs, "sans", "DroidSansMono.ttf");
	if (fontNormal == FONS_INVALID) {
		assert(false);// Could not add font normal.
	}

	unsigned int black = glfonsRGBA(0, 0, 0, 255);

	fonsClearState(fs);
	fonsSetSize(fs, 20);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, black);
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);

	_textShader.compileShader("textRendering.vert", GLSLShader::VERTEX);
	_textShader.compileShader("textRendering.frag", GLSLShader::FRAGMENT);
	_textShader.link();
}
