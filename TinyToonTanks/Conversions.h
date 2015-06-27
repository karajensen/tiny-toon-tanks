////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Conversions.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "Matrix.h"

class Conversion
{
public:

    /**
    * Converts a GLM matrix to an internal Matrix
    */
    static Matrix Convert(const glm::mat4& original)
    {
        Matrix converted;
        converted.m11 = original[0][0];
        converted.m12 = original[1][0];
        converted.m13 = original[2][0];
        converted.m14 = original[3][0];
        converted.m21 = original[0][1];
        converted.m22 = original[1][1];
        converted.m23 = original[2][1];
        converted.m24 = original[3][1];
        converted.m31 = original[0][2];
        converted.m32 = original[1][2];
        converted.m33 = original[2][2];
        converted.m34 = original[3][2];
        return converted;
    }

    /**
    * Converts an internal matrix to a GLM Matrix
    */
    static glm::mat4 Convert(const Matrix& original)
    {
        glm::mat4 converted;
        converted[0][0] = original.m11;  
        converted[1][0] = original.m12;
        converted[2][0] = original.m13;
        converted[3][0] = original.m14;
        converted[0][1] = original.m21;
        converted[1][1] = original.m22;
        converted[2][1] = original.m23;
        converted[3][1] = original.m24;
        converted[0][2] = original.m31;
        converted[1][2] = original.m32;
        converted[2][2] = original.m33;
        converted[3][2] = original.m34;
        return converted;
    }
};