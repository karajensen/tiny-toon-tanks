////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - unittests.cpp
////////////////////////////////////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES
#define BOOST_TEST_MAIN

#include "OutputHelper.h"
#include "float3.h"
#include "matrix.h"

#include <iostream>
#include <fstream>
#include <array>

#include "boost/test/unit_test.hpp"
#include "boost/test/results_reporter.hpp"
#include "boost/test/unit_test_log.hpp"

namespace
{
    std::ofstream stream;  ///< stream to output results

    /**
    * Helps redirect boost results to a file
    * Requires static level stream and default constructor
    */
    struct ResultReporter
    {
        ResultReporter()
        {
            stream.open("results.txt");
            boost::unit_test::unit_test_log.set_stream(stream);
            boost::unit_test::results_reporter::set_stream(stream);
        }
    };

    /**
    * Converts degrees to radians
    * @param degrees The value to convert
    * @return the radians result
    */
    template<typename T> T DegToRad(T degrees)
    {
        return static_cast<T>(M_PI / 180.0) * degrees;
    }
}

namespace UnitTest
{
    BOOST_GLOBAL_FIXTURE(ResultReporter);

    /**
    * Test suite for vector math class
    */
    BOOST_AUTO_TEST_CASE(vector_tests)
    {
        BOOST_MESSAGE(LogSuite("Vector Tests"));
        Float3 vec1(1.0f, 2.0f, 3.0f);
        Float3 vec2(4.0f, 5.0f, 6.0f);  
        Float3 vec3(0.0f, 3.0f, 4.0f);

        BOOST_MESSAGE(LogTest("Vector3::Dot"));
        {
            const float dot = 18.0f;
            BOOST_CHECK(vec1.Dot(vec3) == dot);
            BOOST_MESSAGE(LogValue(vec1.Dot(vec3), dot));
        }

        BOOST_MESSAGE(LogTest("Vector3::Cross"));
        {
            const Float3 cross(-3.0f, 6.0f, -3.0);
            BOOST_CHECK(vec1.Cross(vec2) == cross);
            BOOST_MESSAGE(LogValue(vec1.Cross(vec2), cross));
        }

        BOOST_MESSAGE(LogTest("Vector3::Length"));
        {
            const float length = 5.0f;
            BOOST_CHECK(vec3.Length() == length);
            BOOST_MESSAGE(LogValue(vec3.Length(), length));
        }

        BOOST_MESSAGE(LogTest("Vector3::SquaredLength"));
        {
            const float length = 25.0f;
            BOOST_CHECK(vec3.SquaredLength() == length);
            BOOST_MESSAGE(LogValue(vec3.SquaredLength(), length));
        }

        BOOST_MESSAGE(LogTest("Vector3::Normalise"));
        {
            Float3 normalised = vec1;
            normalised.Normalize();
            BOOST_CHECK(normalised == vec1.GetNormalized());
            BOOST_MESSAGE(LogValue(normalised, vec1.GetNormalized()));
        }

        BOOST_MESSAGE(LogTest("Vector3::IsPerpendicular"));
        {
            Float3 perpendicular = vec1.Cross(vec2).Cross(vec1);
            BOOST_CHECK(vec1.IsPerpendicular(perpendicular));
            BOOST_MESSAGE(LogValue(vec1, perpendicular));
        }

        BOOST_MESSAGE(LogTest("Vector3::Angle"));
        {
            const float angle = DegToRad(90.0f);
            Float3 perpendicular = vec2.Cross(vec3).Cross(vec2);
            BOOST_CHECK(vec2.Angle(perpendicular) == angle);
            BOOST_MESSAGE(LogValue(vec2.Angle(perpendicular), angle));
        }

        BOOST_MESSAGE(LogTest("Vector3::operator=="));
        {
            BOOST_CHECK(vec2 == vec2);
            BOOST_MESSAGE(LogValue(vec2, vec2));
        }

        BOOST_MESSAGE(LogTest("Vector3::operator!="));
        {
            BOOST_CHECK(vec3 != vec1);
            BOOST_MESSAGE(LogValue(vec3, vec1));
        }

        BOOST_MESSAGE(LogTest("Vector3::operator+"));
        {
            Float3 addition(1.0f, 5.0f, 7.0f);
            BOOST_CHECK(addition == vec1 + vec3);
            BOOST_MESSAGE(LogValue(addition, vec1 + vec3));
        }

        BOOST_MESSAGE(LogTest("Vector3::operator-"));
        {
            Float3 subtraction(1.0f, -1.0f, -1.0f);
            BOOST_CHECK(subtraction == vec1 - vec3);
            BOOST_MESSAGE(LogValue(subtraction, vec1 - vec3));
        }
    }

    /**
    * Test suite for matrix math class
    */
    BOOST_AUTO_TEST_CASE(matrix_tests)
    {
        BOOST_MESSAGE(LogSuite("Matrix Tests"));
        Matrix mat(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 
            7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f);

        BOOST_MESSAGE(LogTest("Matrix::MakeIdentity"));
        {
            Matrix identity = mat;
            identity.MakeIdentity();
            BOOST_CHECK(identity == Matrix());
            BOOST_MESSAGE(LogMatrix(identity, Matrix()));
        }

        BOOST_MESSAGE(LogTest("Matrix::Set"));
        {
            std::array<float, 16> arr = 
            { 
                1.0f,   5.0f,   9.0f,   13.0f,
                2.0f,   6.0f,   10.0f,  14.0f,
                3.0f,   7.0f,   11.0f,  15.0f,
                4.0f,   8.0f,   12.0f,  16.0f
            };
            Matrix arrayMat;
            arrayMat.Set(arr.data());

            BOOST_CHECK(mat == arrayMat);
            BOOST_MESSAGE(LogMatrix(mat, arrayMat));
        }

        BOOST_MESSAGE(LogTest("Matrix::GetTranspose3x3"));
        {
            Matrix transpose(mat.m11, mat.m21, mat.m31, 0.0f, mat.m12, 
                mat.m22, mat.m32, 0.0f, mat.m13, mat.m23, mat.m33, 0.0f);

            BOOST_CHECK(transpose == mat.GetTranspose3x3());
            BOOST_MESSAGE(LogMatrix(transpose, mat.GetTranspose3x3()));
        }

        BOOST_MESSAGE(LogTest("Matrix::RotateX"));
        {
            Matrix rotated = mat;
            Matrix created = Matrix::CreateRotateX(DegToRad(45.0f));
            rotated.RotateX(DegToRad(45.0f));

            BOOST_CHECK(rotated == mat * created);
            BOOST_MESSAGE(LogMatrix(rotated, mat * created));  
        }
        
        BOOST_MESSAGE(LogTest("Matrix::RotateY"));
        {
            Matrix rotated = mat;
            Matrix created = Matrix::CreateRotateY(DegToRad(45.0f));
            rotated.RotateY(DegToRad(45.0f));

            BOOST_CHECK(rotated == mat * created);
            BOOST_MESSAGE(LogMatrix(rotated, mat * created));          
        }
        
        BOOST_MESSAGE(LogTest("Matrix::RotateZ"));
        {
            Matrix rotated = mat;
            Matrix created = Matrix::CreateRotateZ(DegToRad(45.0f));
            rotated.RotateZ(DegToRad(45.0f));

            BOOST_CHECK(rotated == mat * created);
            BOOST_MESSAGE(LogMatrix(rotated, mat * created));          
        }
        
        BOOST_MESSAGE(LogTest("Matrix::RotateArbitrary"));
        {
            Float3 axis(1.0f, 0.0f, 0.0f);
            Matrix rotated = mat;
            Matrix created = Matrix::CreateRotateArbitrary(axis, DegToRad(45.0f));
            rotated.RotateArbitrary(axis, DegToRad(45.0f));

            BOOST_CHECK(rotated == mat * created);
            BOOST_MESSAGE(LogMatrix(rotated, mat * created));            
        }

        BOOST_MESSAGE(LogTest("Matrix::RotateXLocal"));
        {
            Matrix rotated = mat;
            Float3 position = rotated.Position();
            rotated.RotateXLocal(DegToRad(30.0f));
            Float3 rotatedPosition = rotated.Position();

            BOOST_CHECK(position == rotatedPosition);
            BOOST_MESSAGE(LogValue(position, rotatedPosition));        
        }

        BOOST_MESSAGE(LogTest("Matrix::RotateYLocal"));
        {
            Matrix rotated = mat;
            Float3 position = rotated.Position();
            rotated.RotateYLocal(DegToRad(30.0f));
            Float3 rotatedPosition = rotated.Position();

            BOOST_CHECK(position == rotatedPosition);
            BOOST_MESSAGE(LogValue(position, rotatedPosition));        
        }

        BOOST_MESSAGE(LogTest("Matrix::RotateZLocal"));
        {
            Matrix rotated = mat;
            Float3 position = rotated.Position();
            rotated.RotateZLocal(DegToRad(30.0f));
            Float3 rotatedPosition = rotated.Position();

            BOOST_CHECK(position == rotatedPosition);
            BOOST_MESSAGE(LogValue(position, rotatedPosition));        
        }
        
        BOOST_MESSAGE(LogTest("Matrix::RotateArbitraryLocal"));
        {
            Float3 axis(1.0f, 0.0f, 1.0f);
            Matrix rotated = mat;
            Float3 position = rotated.Position();
            rotated.RotateArbitraryLocal(axis, DegToRad(30.0f));
            Float3 rotatedPosition = rotated.Position();

            BOOST_CHECK(position == rotatedPosition);
            BOOST_MESSAGE(LogValue(position, rotatedPosition));              
        }

        BOOST_MESSAGE(LogTest("Matrix::operator*(float)"));
        {
            Matrix multiply(2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 
                14.0f, 16.0f, 18.0f, 20.0f, 22.0f, 24.0f);

            BOOST_CHECK(multiply == mat * 2.0f);
            BOOST_MESSAGE(LogMatrix(multiply, mat * 2.0f));
        }

        BOOST_MESSAGE(LogTest("Matrix::operator*(Float3)"));
        {
            Float3 multiply(1.0f, 2.0f, 3.0f);
            Float3 expected(18.0f, 46.0f, 74.0f);

            BOOST_CHECK(mat * multiply == expected);
            BOOST_MESSAGE(LogValue(mat * multiply, expected));
        } 
        
        BOOST_MESSAGE(LogTest("Matrix::operator*(Matrix)"));
        {
            Matrix multiply(38.0f, 44.0f, 50.0f, 60.0f, 98.0f, 116.0f,
                134.0f, 160.0f, 158.0f, 188.0f, 218.0f, 260.00f);

            BOOST_CHECK(multiply == mat * mat);
            BOOST_MESSAGE(LogMatrix(multiply, mat * mat));
        }
        
        BOOST_MESSAGE(LogTest("Matrix::operator+/-"));
        {
            Matrix addition(2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 
                14.0f, 16.0f, 18.0f, 20.0f, 22.0f, 24.0f);
            Matrix combined = mat + addition;

            BOOST_CHECK(mat == combined - addition);
            BOOST_MESSAGE(LogMatrix(mat, combined - addition));
        }

        BOOST_MESSAGE(LogTest("Matrix::Translate"));
        {
            Float3 translate(1.0f, 2.0f, 3.0f);
            Matrix translateMat = mat;
            Float3 expected = mat.Position() + translate;
            translateMat.Translate(translate);

            BOOST_CHECK(translateMat.Position() == expected);
            BOOST_MESSAGE(LogValue(translateMat.Position(), expected));        
        }
       
        BOOST_MESSAGE(LogTest("Matrix::Scale"));
        {
            Float3 scale(2.0f, 4.0f, 6.0f);
            Matrix scaleMat = mat;
            scaleMat.Scale(scale);
            Float3 expected(2.0f, 24.0f, 66.0f);
            Float3 result(scaleMat.m11, scaleMat.m22, scaleMat.m33);

            BOOST_CHECK(result == expected);
            BOOST_MESSAGE(LogValue(result, expected));     
        }
        
        BOOST_MESSAGE(LogTest("Matrix::ScaleLocal"));
        {
            Matrix scale = mat;
            Float3 position = mat.Position();
            scale.ScaleLocal(3.0f);
            Float3 scaledPosition = mat.Position();

            BOOST_CHECK(position == scaledPosition);
            BOOST_MESSAGE(LogValue(position, scaledPosition));        
        }

        BOOST_MESSAGE(LogTest("Matrix::Right"));
        {
            Float3 rightVec(1.0f, 2.0f, 3.0f);
            Matrix rightMat;
            rightMat.SetRight(rightVec);
            BOOST_CHECK(rightMat.Right() == rightVec);
            BOOST_MESSAGE(LogValue(rightMat.Right(), rightVec));
        }

        BOOST_MESSAGE(LogTest("Matrix::Up"));
        {
            Float3 upVec(4.0f, 5.0f, 6.0f);
            Matrix upMat;
            upMat.SetUp(upVec);
            BOOST_CHECK(upMat.Up() == upVec);
            BOOST_MESSAGE(LogValue(upMat.Up(), upVec));
        }

        BOOST_MESSAGE(LogTest("Matrix::Forward"));
        {
            Float3 forwardVec(7.0f, 8.0f, 9.0f);
            Matrix forwardMat;
            forwardMat.SetForward(forwardVec);
            BOOST_CHECK(forwardMat.Forward() == forwardVec);
            BOOST_MESSAGE(LogValue(forwardMat.Forward(), forwardVec));
        }
    }
}