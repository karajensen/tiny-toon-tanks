////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Conversions.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GLMHelper.h"
#include "bullet/include/linearMath/btTransform.h"

class Conversion
{
public:

    /**
    * Converts a matrix into just the position
    */
    static glm::vec3 Position(const glm::mat4& matrix)
    {
        return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
    }

    /**
    * Converts a bullet matrix to a GLM matrix
    */
    static glm::mat4 Convert(const btTransform& matrix)
    {
        glm::mat4 converted;
        const btMatrix3x3 basis = matrix.getBasis();
        glm::matrix_set_position(converted, Convert(matrix.getOrigin()));
        glm::matrix_set_right(converted, Convert(basis.getColumn(0)));
        glm::matrix_set_up(converted, Convert(basis.getColumn(1)));
        glm::matrix_set_forward(converted, Convert(basis.getColumn(2)));
        return converted;
    }

    /**
    * Converts a GLM matrix to a bullet matrix
    */
    static btTransform Convert(const glm::mat4& matrix)
    {
        btMatrix3x3 basis;
        basis.setValue(matrix[0][0], matrix[1][0], matrix[2][0], 
                       matrix[0][1], matrix[1][1], matrix[2][1], 
                       matrix[0][2], matrix[1][2], matrix[2][2]);

        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(Convert(glm::matrix_get_position(matrix)));
        transform.setBasis(basis);
        return transform;
    }

    /**
    * Converts a GLM vector to a bullet vector
    */
    static btVector3 Convert(const glm::vec3& vec)
    {
        return btVector3(btScalar(vec.x), btScalar(vec.y), btScalar(vec.z)); 
    }

    /**
    * Convert a bullet vector to a GLM vector
    */
    static glm::vec3 Convert(const btVector3& vec)
    {
        return glm::vec3(vec.getX(), vec.getY(), vec.getZ()); 
    }
};