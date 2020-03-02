//
// Created by cwelsh on 2/29/2020.
//


#include "gtest/gtest.h"
#include "SBNWClientAPI.h"

#include <iostream>

using namespace std;


TEST(SBNWSharedTests, t) {
    SBNWClientAPI sbnw;
    const char *version = sbnw.gf_getCurrentLibraryVersion();
    const char *expected = "1.3.27";
    ASSERT_STREQ(expected, version);
}

//TEST_F(SBNWSharedTestFixture, t) {
//    auto gf_getCurrentLibraryVersion = (gf_getCurrentLibraryVersionPtr) GetProcAddress(
//            handle, "gf_getCurrentLibraryVersion");
//    if (gf_getCurrentLibraryVersion == nullptr) {
//        GetLastError();
//        throw std::invalid_argument("deaded");
//    }
//    const char *version = gf_getCurrentLibraryVersion();
//    if (version == nullptr) {
//        GetLastError();
//    }
//    ASSERT_STREQ("1.3.27", version);
//}
//
//TEST_F(SBNWSharedTestFixture, TestGetFunction) {
//    auto gf_getCurrentLibraryVersion = getFunction<gf_getCurrentLibraryVersionPtr>(
//            "gf_getCurrentLibraryVersion");
//    if (gf_getCurrentLibraryVersion == nullptr) {
//        GetLastError();
//    }
//    const char *version = gf_getCurrentLibraryVersion();
//    const char *expected = "1.3.27";
//    cout << version << endl;
//    ASSERT_STREQ(expected, version);
//}
//
//TEST_F(SBNWSharedTestFixture, TestGetVersion) {
//    const char *version = gf_getCurrentLibraryVersion();
////    const char* version = getVersion();
////    cout << gf_getCurrentLibraryVersion << endl;
//    cout << version << endl;
////    const char *expected = "1.3.27";
////    ASSERT_STREQ(expected, version);
//}
