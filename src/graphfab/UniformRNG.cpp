//
// Created by cwelsh on 3/5/2020.
//

#include <functional>
#include "UniformRNG.h"

namespace Graphfab {


    std::uniform_int_distribution<int> UniformRNG::makeDist() {
            std::uniform_int_distribution<int> unifdist(low, high);
            return unifdist;
    }

    std::mt19937 UniformRNG::makeGenerator() {
        generator = std::mt19937(dev());
        return generator;
    }

    std::mt19937 UniformRNG::makeGeneratorWithSeed() {
        generator = std::mt19937(dev());
        generator.seed(seed);
        return generator;
    }
}







