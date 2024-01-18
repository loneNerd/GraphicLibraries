#include "tools/utils/random.hpp"

namespace Utils = Engine::Tools::Utils;

std::default_random_engine Utils::Random::m_randomGenerator;

int Utils::Random::Generate(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(m_randomGenerator);
}

float Utils::Random::Generate(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(m_randomGenerator);
}

bool Utils::Random::CheckPercentage(float percentage)
{
    std::uniform_real_distribution<float> distribution(0.0f, 100.0f);
    return distribution(m_randomGenerator) <= percentage;
}
