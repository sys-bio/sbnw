//
// Created by cwelsh on 3/5/2020.
//

#ifndef SBNW_UNIFORMRNG_H
#define SBNW_UNIFORMRNG_H

#include <random>
#include <chrono>

namespace Graphfab {

    class UniformRNG {
        int low, high;
        double seed; //
        std::mt19937 generator;
        std::uniform_int_distribution<int> dist;
        std::random_device dev;


    public:
        UniformRNG(int low, int high) : low(low), high(high) {
            generator = makeGenerator();
            dist = makeDist();
        };

        UniformRNG(int low, int high, int seed) : low(low), high(high), seed(seed) {
            generator = makeGeneratorWithSeed();
            dist = makeDist();
        };

        ~UniformRNG() = default;

        // copy constructor
        UniformRNG(const UniformRNG &uniformRng) = default;

        // copy assignment constructor
        UniformRNG &operator=(const UniformRNG &uniformRng) = default;

        // move constructor
        UniformRNG(UniformRNG &&uniformRng) = default;

        // move assignment constructor
        UniformRNG &operator=(UniformRNG &&uniformRng) = default;

        int generate() {
            return dist(generator);
        };


    private:

        std::uniform_int_distribution<int> makeDist();

        std::mt19937 makeGenerator();

        std::mt19937 makeGeneratorWithSeed();

    };



}
#endif //SBNW_UNIFORMRNG_H
