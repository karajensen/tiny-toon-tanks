////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - random_generator.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "RandomGenerator.h"
#include "common.h"

#include <time.h>

std::default_random_engine Random::sm_generator;

void Random::Initialise()
{
    const auto seed = static_cast<unsigned long>(time(0));
    sm_generator.seed(seed);
    LogInfo("Starting initialisation with seed " + std::to_string(seed));
}

int Random::Generate(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(sm_generator);
}

float Random::Generate(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(sm_generator);
}