=Automatic Layout Tool for SBML=

C API Documentation (via Doxygen) can be found in the <a href="http://code.google.com/p/graphfab/w/list">Wiki</a> (JKM: Not anymore. The autogenerated documentation should never have been committed to the repo (it pollutes the diffs). We need a permanent web hosting solution).


==How to compile the library==

 * Install the latest version of <a href="http://sourceforge.net/projects/sbml/files/libsbml/">libsbml</a> (tested with 5.6, 5.8, 5.10, 5.11)
 * Checkout the latest revision of the <a href="http://code.google.com/p/graphfab/source/checkout">source code</a> (Google has done away with downloads).
 * Download and run <a href="http://www.cmake.org/">CMake</a> (version 2 or 3)
 * Using CMake, set the `LIBSBML_DIR` variable to point to the directory where libSBML is installed/downloaded
 * Configure & generate via CMake (see <a href="http://code.google.com/p/roadrunnerlib/wiki/BuildingRoadRunner">here</a> for an example), choosing a generator that matches the required configuration (32-bit x86 is recommended on Windows; on Linux the default generator is sufficient)
 * Open the generated .sln in Visual Studio, and change the configuration to "Release" (the easy step)
 * For all physical targets (graphfab & basic, in my project), set the runtime library appropriately. Right click on the projects and select Properties->Configuration Properties->C/C++->Code Generation. The value that the runtime library must be set to ostensibly depends on the options used to build libsbml.  The libsbml docs specifically suggest the multi-threaded (non-debug) DLL.  However, this always failed for me, so I had to use the non-DLL version ("Multi-threaded(/MT)").  This was the case for both 32 and 64-bit binaries.  If you know the cause of this, please share.
 * Right click on the ALL_BUILD target and select the "build" option to compile the library.

==Using the Library==

The C API is exposed via graphfab/autolayoutc_api.h.  There's an example in sandbox/basic that demonstrates all the functionality you need to get the layout working.  The relevant lines are:

<code language="c">
        // type to store layout info
        gf_layoutInfo* l;

        // load the model
        gf_SBMLModel* mod = gf_loadSBMLbuf(buf);

        // options for layout algo
        fr_options opt;

        // read layout info from the model
        l = gf_processLayout(mod);

        // randomize node positions
        gf_randomizeLayout(l);

        // do layout algo
        opt.k = 20.;
        opt.boundary = 1;
        opt.mag = 0;
        opt.grav = 0.;
        opt.baryx = opt.baryy = 500.;
        opt.autobary = 1;
        gf_doLayoutAlgorithm(opt, l);

        // save layout information to new SBML file
        gf_writeSBMLwithLayout(outfile, mod, l);

        // run destructors on the model
        gf_freeSBMLModel(mod);

        // run destructors on the layout
        gf_freeLayoutInfo(l);
</code>