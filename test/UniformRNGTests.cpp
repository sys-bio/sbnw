//
// Created by cwelsh on 3/5/2020.
//

#include "gtest/gtest.h"
#include "SBNWClientAPI.h"

TEST(UniformRNGTests, TestRNGWorksWithSeed) {
    SBNWClientAPI sbnw;
    // seed version of rng makes sequence predictible for testing purpose
    int x = sbnw.gf_generateRandomUniformIntWithSeed(0, 10, 5);
    ASSERT_EQ(9, x);
}

TEST(UniformRNGTests, TestRNGWorksWithoutSeed) {
    SBNWClientAPI sbnw;
    // Pick a really large number to lower chances of getting same value
    int x = sbnw.gf_generateRandomUniformInt(0, 1000000);
    int y = sbnw.gf_generateRandomUniformInt(0, 1000000);
    ASSERT_NE(x, y);
}
























