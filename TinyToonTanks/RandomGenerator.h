////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - random_generator.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <random>

/**
* Utility class to get a random value
*/
class Random
{
public:

    /**
    * Initialises the random generator
    */
    static void Initialise();

    /**
    * @return a random int between min/max
    */
    static int Generate(int min, int max);

    /**
    * @return a random float between min/max
    */
    static float Generate(float min, float max);

private:

    static std::default_random_engine sm_generator;
};