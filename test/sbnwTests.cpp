//
// Created by cwelsh on 2/29/2020.
//


#include "gtest/gtest.h"
#include "graphfab/layout.h"

int loadLibrary(){
    std::string lib = "";
}

TEST(test, t){

    gf_layoutInfo* l;
//  load the model
//    gf_SBMLModel* mod = gf_loadSBMLbuf(buf);
////  options for layout algo
//    fr_options opt;
////  read layout info from the model
//    l = gf_processLayout(mod);
////  randomize node positions
//    gf_randomizeLayout(l);
//// do layout algo
//    opt.k = 20.;
//    opt.boundary = 1;
//    opt.mag = 0;
//    opt.grav = 0.;
//    opt.baryx = opt.baryy = 500.;
//    opt.autobary = 1;
//    gf_doLayoutAlgorithm(opt, l);
////  save layout information to new SBML file
//    gf_writeSBMLwithLayout(outfile, mod, l);
////  run destructors on the model
//    gf_freeSBMLModel(mod);
////  run destructors on the layout
//    gf_freeLayoutInfo(l);
}



//#include "gtest/gtest.h"
//
//#if defined(_WIN32) || defined (_WIN64) || defined(__CYGWIN__)
//#include <windows.h>
//#elif defined(__linux__)
//
//#include <dlfcn.h>
//#endif

//
//TEST(test, test_add_windows) {
//    typedef int (*addPtr)(int, int);
//#if defined(_WIN32) || defined (_WIN64) || defined(__CYGWIN__)
//    std::cout << "windows" << std::endl;
//    HINSTANCE hinstLib = LoadLibraryA(TEXT("ACrossPlatformCppLibrary.dll"));
//    ASSERT_NE(hinstLib, nullptr);
//    auto add = (addPtr) GetProcAddress(hinstLib, "add");
//    ASSERT_NE(add, nullptr);
//#elif defined(__linux__)
//    std::cout << "GNU" << std::endl;
//    void* handle;
//    handle = dlopen("ACrossPlatformCppLibrary.dll.a", RTLD_LAZY);
//    ASSERT_NE(handle, nullptr);
//    auto add = (addPtr) dlsym(handle, "add");
//#elif defined(__APPLE__)
//    // something else.
//#endif
//    int x = 5;
//    int y = 6;
//    int answer = add(x, y);
//    ASSERT_EQ(answer, 11);