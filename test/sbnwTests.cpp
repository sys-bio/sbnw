//
// Created by cwelsh on 2/29/2020.
//


#include "gtest/gtest.h"
#include <filesystem>
#include "SBNWClientAPI.h"

#include <iostream>



using namespace std;




TEST(SBNWSharedTests, TestVersion) {
    SBNWClientAPI sbnw;
    const char *version = sbnw.gf_getCurrentLibraryVersion();
    const char *expected = "1.3.27";
    ASSERT_STREQ(expected, version);
}

TEST(SBNWSharedTests, TestLoadSbml) {
    SBNWClientAPI sbnw;
    gf_SBMLModel* sbml = sbnw.gf_loadSBMLfile(R"(D:\sbnw\test\teusink2000.xml)");
    ASSERT_NE(sbml->pdoc, nullptr );
}

