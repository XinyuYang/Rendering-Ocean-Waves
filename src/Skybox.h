/*!
 * /author Bret Jackson
 *
 * /file  Skybox.h
 * /brief Creates a skybox environment map.
 * Based on the skybox example from learnopengl.com
 *
 */

#ifndef SKYBOX_H
#define SKYBOX_H

#include <Texture.h>
#include <GLSLProgram.h>

namespace basicgraphics {
    
    class Skybox : public std::enable_shared_from_this<Skybox>
    {
        public:
            Skybox(std::shared_ptr<Texture> cubeMap);
            ~Skybox() {}
        
        void draw(const glm::mat4 &view, const glm::mat4 &projection);
        
        private:
            std::shared_ptr<Texture> cubeMap;
            GLSLProgram skyboxShader;
            GLuint skyboxVAO;
        
    };
}//namespace
#endif
