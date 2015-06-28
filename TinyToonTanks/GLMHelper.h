////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GlmHelper.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"

namespace glm
{
    /**
    * Retrieves position from the matrix
    */
    inline glm::vec3 matrix_get_position(const glm::mat4& matrix)
    {
        return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
    }

    /**
    * Retrieves the up vector from the matrix
    */
    inline glm::vec3 matrix_get_up(const glm::mat4& matrix)
    {
        return glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]);
    }

    /**
    * Retrieves the forward vector from the matrix
    */
    inline glm::vec3 matrix_get_forward(const glm::mat4& matrix)
    {
        return glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2]);
    }

    /**
    * Retrieves the right vector from the matrix
    */
    inline glm::vec3 matrix_get_right(const glm::mat4& matrix)
    {
        return glm::vec3(matrix[0][0], matrix[0][1], matrix[0][2]);
    }

    /**
    * Sets the position for the matrix
    */
    inline void matrix_set_position(glm::mat4& matrix, const glm::vec3& vec)
    {
        matrix[3][0] = vec.x;
        matrix[3][1] = vec.y;
        matrix[3][2] = vec.z;
    }

    /**
    * Sets the up vector for the matrix
    */
    inline void matrix_set_up(glm::mat4& matrix, const glm::vec3& vec)
    {
        matrix[1][0] = vec.x;
        matrix[1][1] = vec.y;
        matrix[1][2] = vec.z;
    }

    /**
    * Sets the forward vector for the matrix
    */
    inline void matrix_set_forward(glm::mat4& matrix, const glm::vec3& vec)
    {
        matrix[2][0] = vec.x;
        matrix[2][1] = vec.y;
        matrix[2][2] = vec.z;
    }

    /**
    * Sets the right vector for the matrix
    */
    inline void matrix_set_right(glm::mat4& matrix, const glm::vec3& vec)
    {
        matrix[0][0] = vec.x;
        matrix[0][1] = vec.y;
        matrix[0][2] = vec.z;
    }
}