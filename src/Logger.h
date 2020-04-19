////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - logger.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <string>

/**
* Logs info to the outputstream
* @param info The information to log
*/
inline void LogInfo(const std::string& info)
{
    std::cout << "INFO: \t" << info << std::endl;
}

/**
* Logs error to the outputstream
* @param error The error to log
*/
inline void LogError(const std::string& error)
{
    std::cout << "ERROR: \t" << error << std::endl;
}