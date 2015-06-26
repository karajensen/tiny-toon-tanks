////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - float3.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <cmath>
#include <ostream>

/**
* Vector with two floats
*/
struct Float2
{
    float x = 0.0f;
    float y = 0.0f;

    /**
    * Constructor
    */
    Float2() = default;
    Float2(float X, float Y) : x(X), y(Y) {}

    /**
    * Testing equality for two vectors
    * @param vec The vector to test equality against
    * @return Whether the two vectors are equal
    */
    bool operator==(const Float2& vec) const
    {
        return x == vec.x && y == vec.y;
    }
};

/**
* Three dimensional vector
*/
class Float3
{
public:

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    /**
    * Constructor
    */
    Float3() = default;

    /**
    * Constructor
    * @param x/y/z The components of the Float3
    */
    Float3(float X, float Y, float Z) : 
        x(X), 
        y(Y),
        z(Z) 
    {
    };

    /**
    * Testing equality for two vectors
    * @param vec The vector to test equality against
    * @return Whether the two vectors are equal
    */
    bool operator==(const Float3& vec) const
    {
        return x == vec.x && y == vec.y && z == vec.z;
    }

    /**
    * Testing inequality for two vectors
    * @param vec The vector to test inequality against
    * @return Whether the two vectors are inequal
    */
    bool operator!=(const Float3& vec) const
    {
        return x != vec.x || y != vec.y || z != vec.z;
    }
    
    /**
    * Inverts all components of the vector
    * @return A new vector from the vector inverted
    */
    Float3 operator-() const
    {
        return Float3(-x,-y,-z);
    }
    
    /**
    * Multiplication against a scalar
    * @param value Scalar to multiply against
    * @return A new vector from the vector multiplied by scalar
    */
    Float3 operator*(float value) const
    {
        return Float3(value*x, value*y, value*z); 
    }

    /**
    * Multiplication against a scalar
    * @param value Scalar to multiply against
    */
    void operator*=(float value)
    {
        x = x*value;
        y = y*value;
        z = z*value;
    }

    /**
    * Division against a scalar
    * @param value Scalar to divide against
    * @return A new vector from the vector divided by scalar
    */
    Float3 operator/(float value) const
    {
        return Float3(x/value, y/value, z/value); 
    }

    /**
    * Division against a scalar
    * @param value Scalar to divide against
    */
    void operator/=(float value)
    {
        x = x/value;
        y = y/value;
        z = z/value;
    }

    /**
    * Float3 addition with another vector
    * @param vec The vector to add
    * @return A new vector from the two added vectors
    */
    Float3 operator+(const Float3& vec) const
    {
        return Float3(x + vec.x, y + vec.y, z + vec.z);
    }
    
    /**
    * Float3 addition with another vector
    * @param vec The vector to add
    */    
    void operator+=(const Float3& vec)
    {
        x = x + vec.x;
        y = y + vec.y;
        z = z + vec.z;
    }

    /**
    * Float3 subtraction with another vector
    * @param vec The vector to subtract
    * @return A new vector from the two subtracted vectors
    */
    Float3 operator-(const Float3& vec) const
    {
        return Float3(x - vec.x, y - vec.y, z - vec.z);
    }
    
    /**
    * Float3 subtraction with another vector
    * @param vec The vector to subtract
    */    
    void operator-=(const Float3& vec)
    {
        x = x - vec.x;
        y = y - vec.y;
        z = z - vec.z;
    }

    /**
    * Cross Product
    * @param vec The vector to be crossed with 
    * @return The vector perpendicular to the given vectors
    */
    Float3 Cross(const Float3& vec) const
    {
        return Float3((y*vec.z)-(z*vec.y),(z*vec.x)-(x*vec.z),(x*vec.y)-(y*vec.x));
    } 

    /**
    * Vector Dot Product
    * @param vec The vector to be dotted with 
    * @return The dot product between the vectors
    */
    float Dot(const Float3& vec) const
    {
        return static_cast<float>((vec.x*x)+(vec.y*y)+(vec.z*z));
    }

    /**
    * Tests whether two vectors are perpendicular
    * @param vec The vector to be tested against
    * @return Whether the vectors are perpendicular
    */
    bool IsPerpendicular(const Float3& vec) const
    {
        return fabs(Dot(vec)) < 0.0001f;
    }

    /**
    * Gets the angle between two vectors in radians
    * @param vec The vector to get the angle between
    * @return The angle in radians
    */
    float Angle(const Float3& vec) const
    {
        float dot = Dot(vec);
        return acos(dot/(vec.Length()*Length()));
    }

    /**
    * @return The squared length of the vector
    */
    float SquaredLength() const
    {
        return static_cast<float>((x*x)+(y*y)+(z*z));
    }

    /**
    * @return The length of the vector
    */
    float Length() const
    {
        return static_cast<float>(std::sqrt(
            static_cast<double>((x*x)+(y*y)+(z*z))));
    }

    /**
    * Sets the vector from another vector
    * @param vec The vector to use when setting
    */
    void Set(const Float3& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    /**
    * Sets the vector from the given components
    * @param x/y/z The components to use when setting
    */
    void Set(float X, float Y, float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }

    /**
    * Normlizes into a unit vector
    */
    void Normalize()
    {
        float length = Length();
        x = x/length;
        y = y/length;
        z = z/length;
    }

    /**
    * @return A copy of the vector normalized
    */
    Float3 GetNormalized() const
    {
        float length = Length();
        return Float3(x/length, y/length, z/length);
    }

    /**
    * Sets all components to zero
    */
    void SetToZero()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    /**
    * @return whether all components are zero
    */
    bool IsZero() const
    {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }

    /**
    * Allows output of the vector to a standard stream
    * @param stream The stream to output in
    * @param vec The vector to output
    * @return the stream to allow for chaining
    */
    friend std::ostream& operator<<(std::ostream& stream, const Float3& vec)
    {
        stream << vec.x << ", " << vec.y << ", " << vec.z;
        return stream;
    }
};