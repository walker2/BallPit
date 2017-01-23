#ifndef FALCONLATINUM_GLTEXTURE_H
#define FALCONLATINUM_GLTEXTURE_H

//#include "SDL2/SDL_opengl.h"
#include <GLES2/gl2.h>
namespace Falcon
{
    struct GLTexture {
        GLuint id;
        int width;
        int height;
    };
}
#endif
