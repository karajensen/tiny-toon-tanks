////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - outputhelper.h
////////////////////////////////////////////////////////////////////////////////////////    

#pragma once

#include "matrix.h"

#include "boost/algorithm/string.hpp"

#include <sstream>
#include <string>
#include <iomanip>

namespace
{
    /**
    * Logs an entry for a test suite
    * @param suite The name of the test suite
    * @return the string entry for a suite
    */
    std::string LogSuite(const std::string& suite)
    {
        std::ostringstream stream;
        const int linewidth = 100;
        stream << std::endl << std::setfill('-') << std::setw(linewidth) 
            << " " << std::endl << boost::algorithm::to_upper_copy(suite) 
            << std::endl << std::setw(linewidth) << " " 
            << std::setfill(' ') << std::endl;
        return stream.str();
    }

    /**
    * Logs an entry for a test
    * @param name The name of the test
    * @return the string entry for a test
    */
    std::string LogTest(const std::string& name)
    {
        return name+":";
    }

    /**
    * Logs the results for a test
    * @param value The value used to determine if the test passed
    * @param comparison The value used to compare if the test passed
    * @return a string of both values in readable format
    */
    template<typename T> 
    std::string LogValue(const T& value, const T& comparison)
    {
        std::ostringstream stream;
        stream.setf(std::ios_base::boolalpha|std::ios_base::fixed);
        stream << value << " | " << comparison << std::endl;
        return stream.str();
    }

    /**
    * Outputs matrices in a readable format
    * @param value The value used to determine if the test passed
    * @param comparison The value used to compare if the test passed
    * @return a string of both matrices in readable format
    */
    std::string LogMatrix(const Matrix& value, const Matrix& comparison)
    {
        std::ostringstream stream;
        stream.setf(std::ios_base::boolalpha|std::ios_base::fixed);
        auto width = std::setw(10);

        stream << width << value.m11 << " " << width << value.m12 << 
            " " << width << value.m13 << " " << width << value.m14;

        stream << " | " << width << comparison.m11 <<
            " " << width << comparison.m12 << " " << width << 
            comparison.m13 << " " << width << comparison.m14;

        stream << std::endl << width << value.m21 << " " << width << 
            value.m22 << " " << width << value.m23 << " " << width << value.m24;

        stream << " | " << width << comparison.m21 <<
            " " << width << comparison.m22 << " " << width << 
            comparison.m23 << " " << width << comparison.m24;

        stream << std::endl << width << value.m31 << " " << width << 
            value.m32 << " " << width << value.m33 << " " << width << value.m34;

        stream << " | " << width << comparison.m31 <<
            " " << width << comparison.m32 << " " << width << 
            comparison.m33 << " " << width << comparison.m34 << std::endl;

        return stream.str();
    }
}