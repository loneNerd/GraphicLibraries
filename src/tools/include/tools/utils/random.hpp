#pragma once
#ifndef ENGINE_TOOLS_UTILS_RANDOM_HPP_
#define ENGINE_TOOLS_UTILS_RANDOM_HPP_

#include <random>

namespace Engine::Tools::Utils
{
    class Random
    {
    public:
        Random() = delete;

        static int Generate(int min, int max);
        static float Generate(float min, float max);
        static bool CheckPercentage(float percentage);

    private:
        static std::default_random_engine m_randomGenerator;
    };
}

#endif // ENGINE_TOOLS_UTILS_RANDOM_HPP_
