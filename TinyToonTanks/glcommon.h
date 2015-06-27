////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - glcommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "opengl/gl_core_4_4.h"
#include "glfw/glfw3.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "logger.h"

const int MULTISAMPLING_COUNT = 4;

/**
* OpenGL call checking
* @return whether the last call to OpenGL has failed
* @note requires an OpenGL context to be created
*/
inline bool HasCallFailed()
{
    switch(glGetError())
    {
    case GL_NO_ERROR:
        return false;
    case GL_INVALID_VALUE:
        LogError("OpenGL: Invalid Value");
        return true;
    case GL_INVALID_OPERATION:
        LogError("OpenGL: Invalid Operation");
        return true;
    default:
        LogError("OpenGL: Unknown Error");
        return true;
    }
}
