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
    gf_SBMLModel *sbml = sbnw.gf_loadSBMLfile(R"(D:\sbnw\test\teusink2000.xml)");
    ASSERT_NE(sbml->pdoc, nullptr);
}


TEST(SBNWSharedTests, Test) {
    const char* outfile = R"(D:\sbnw\test\teusink2000LayoutFromCpp.xml)";
    SBNWClientAPI sbnw;
    gf_SBMLModel *sbml = sbnw.gf_loadSBMLfile(R"(D:\sbnw\test\teusink2000.xml)");

    //  type to store layout info
    gf_layoutInfo *l;

    //  options for layout algo
    fr_options opt;
    //  read layout info from the model
    l = sbnw.gf_processLayout(sbml);
    //  randomize node positions
    sbnw.gf_randomizeLayout(l);
    //  do layout algo
    opt.k = 20.;
    opt.boundary = 1;
    opt.mag = 100;
    opt.grav = 5.;
    opt.baryx = opt.baryy = 500.;
    opt.autobary = 1;
    opt.prerandomize = true;
    opt.enable_comps = false;
    opt.padding = 20;
    sbnw.gf_doLayoutAlgorithm(opt, l);
    //  save layout information to new SBML file
    sbnw.gf_writeSBMLwithLayout(outfile, sbml, l);
    //  run destructors on the model
    sbnw.gf_freeSBMLModel(sbml);
    //  run destructors on the layout
    sbnw.gf_freeLayoutInfo(l);

}

TEST(SBNWSharedTests, Test2) {
    SBNWClientAPI sbnw;
//    gf_SBMLModel *sbml = sbnw.gf_loadSBMLfile(R"(D:\sbnw\test\teusink2000.xml)");
    const char* outfile = R"(D:\sbnw\test\teusink2000LayoutFromCpp.xml)";
    const char* outfile2 = R"(D:\sbnw\test\teusink2000LayoutFromCpp2.xml)";
    gf_SBMLModel *sbml = sbnw.gf_loadSBMLfile(outfile);

    //  type to store layout info
    gf_layoutInfo *l;

    //  options for layout algo
    fr_options opt;
    //  read layout info from the model
    l = sbnw.gf_processLayout(sbml);
    //  randomize node positions
    sbnw.gf_randomizeLayout(l);
    gf_network* network = sbnw.gf_getNetworkp(l);

    int s = sbnw.gf_nw_isLayoutSpecified(network);

    gf_compartment*  compp = sbnw.gf_nw_getCompartmentp(network, 0);

    //  do layout algo
    opt.k = 20.;
    opt.boundary = 1;
    opt.mag = 100;
    opt.grav = 5.;
    opt.baryx = opt.baryy = 500.;
    opt.autobary = 1;
    opt.prerandomize = true;
    opt.enable_comps = false;
    opt.padding = 20;

//    compp.c
    sbnw.gf_doLayoutAlgorithm(opt, l);



//
//    //    sbnw.gf_tf_fitToWindow(l, 0.0, 1024.0, 0.0, 1024.0);
//    //  save layout information to new SBML file
//    sbnw.gf_writeSBMLwithLayout(outfile2, sbml, l);
//    //  run destructors on the model
//    sbnw.gf_freeSBMLModel(sbml);
//    //  run destructors on the layout
//    sbnw.gf_freeLayoutInfo(l);

}

