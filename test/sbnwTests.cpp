//
// Created by cwelsh on 2/29/2020.
//


#include "gtest/gtest.h"

#if defined(_WIN32) || defined (_WIN64) || defined(__CYGWIN__)

#   include <windows.h>

#elif defined(__linux__)
#   include <dlfcn.h>
#endif

#include <iostream>

using namespace std;

typedef struct {
    /// @private
    void *n;
} gf_network;

typedef struct {
    /// @private
    void *n;
} gf_node;

typedef struct {
    /// @private
    void *r;
} gf_reaction;

typedef struct {
    /// @private
    void *c;
} gf_curve;

typedef struct {
    /// @private
    void *c;
} gf_compartment;

typedef struct {
    /// @private
    void *canv;
} gf_canvas;

typedef struct {
    /// @privatesection
    /// Network*
    void *net;
    /// Canvas*
    void *canv;
    /// SBML content
    char *cont;

    // sbml opts
    int level;
    int version;
} gf_layoutInfo;

//DEPRECATED: rename to gf_point
typedef struct {
    /// @privatesection
    double x;
    double y;
} CPoint;

typedef struct {
    /// x coordinate
    double x;
    /// y coordinate
    double y;
} gf_point;

typedef struct {
    /// @private
    void *tf;
} gf_transform;

typedef struct {
    /// Start
    gf_point s;
    /// Control 1
    gf_point c1;
    /// Control 2
    gf_point c2;
    /// End
    gf_point e;
} gf_curveCP;

typedef enum {
    GF_ROLE_SUBSTRATE,
    GF_ROLE_PRODUCT,
    GF_ROLE_SIDESUBSTRATE,
    GF_ROLE_SIDEPRODUCT,
    GF_ROLE_MODIFIER,
    GF_ROLE_ACTIVATOR,
    GF_ROLE_INHIBITOR
} gf_specRole;

typedef struct __gf_SBMLLayout {
    void *pdoc; /// Pointer to SBMLDocument cast to void
} gf_SBMLModel;

class SBNWSharedTestFixture : public ::testing::Test {
public:

#if defined(_WIN32)
    HINSTANCE handle;
#elif defined(__linux__)
    void* handle;
#elif defined(__APPLE__)
    // something else
#endif

    void SetUp() override {
        this->getLibrary();
    }

    void getLibrary() {
#if defined(_WIN32) || defined(__CYGWIN__)
        handle = LoadLibraryA(TEXT("sbnw.dll"));
        if (handle == nullptr) {
            GetLastError();
            throw std::invalid_argument("sbnw library not found");
        }
#elif defined(__linux__)
        void* handle;
        handle = dlopen("sbnw.so", RTLD_LAZY);
        ASSERT_NE(handle, nullptr);
        auto add = (addPtr) dlsym(handle, "add");
#elif defined(__APPLE__)
        // something else.
#endif
    }

    virtual void TearDown() {
    }

    template<class T>
    T getFunction(const CHAR *funcName) {
#if defined(_WIN32)
        T func = (T) GetProcAddress(handle, reinterpret_cast<LPCSTR>(funcName));
        if (func == nullptr) {
            GetLastError();
            throw std::invalid_argument("dead");
        }
        return func;
    }

#elif defined(__linux__)

#elif defined(__APPLE__)

#endif

    typedef void gf_freeSBMLModel(gf_SBMLModel *);

    typedef gf_SBMLModel *gf_loadSBMLbuf(const char *);

    typedef gf_SBMLModel *gf_loadSBMLfile(const char *);

    typedef void gf_freeLayoutInfo(gf_layoutInfo *);

    typedef void gf_freeLayoutInfoHierarch(gf_layoutInfo *);

    typedef void gf_freeModelAndLayout(gf_SBMLModel *, gf_layoutInfo *);

    typedef gf_SBMLModel gf_SBMLModel_new();

    typedef gf_SBMLModel *gf_SBMLModel_newp();

    typedef gf_layoutInfo gf_layoutInfo_new(uint64_t, uint64_t, uint64_t, uint64_t);

    typedef gf_layoutInfo *gf_layoutInfo_newp(uint64_t, uint64_t, uint64_t, uint64_t);

    typedef gf_layoutInfo *gf_processLayout(gf_SBMLModel *);

    typedef gf_layoutInfo *gf_loadSBMLIntoLayoutEngine(const char *, gf_SBMLModel *);

    typedef void gf_setModelNamespace(gf_layoutInfo *, unsigned long, unsigned long);

    typedef const char *gf_getDefaultCompartmentId();

    typedef void gf_setDefaultCompartmentId(const char *);

    typedef gf_network gf_getNetwork(gf_layoutInfo *);

    typedef gf_network *gf_getNetworkp(gf_layoutInfo *);

    typedef void gf_clearNetwork(gf_network *);

    typedef void gf_releaseNetwork(gf_network *);

    typedef char *gf_nw_getId(gf_network *);

    typedef void gf_nw_setId(gf_network *, const char *);

    typedef uint64_t gf_nw_getNumNodes(const gf_network *);

    typedef uint64_t gf_nw_getNumUniqueNodes(const gf_network *);

    typedef uint64_t gf_nw_getNumRxns(const gf_network *);

    typedef uint64_t gf_nw_getNumComps(const gf_network *);

    typedef gf_node gf_nw_getNode(gf_network *, uint64_t);

    typedef gf_node gf_nw_getUniqueNode(gf_network *, uint64_t);

    typedef gf_node *gf_nw_getNodep(gf_network *, uint64_t);

    typedef gf_node *gf_nw_getUniqueNodep(gf_network *, uint64_t);

    typedef gf_node *gf_nw_getNodepFromId(gf_network *, const char *);

    typedef gf_reaction gf_nw_getRxn(gf_network *, uint64_t);

    typedef gf_reaction *gf_nw_getRxnp(gf_network *, uint64_t);

    typedef void gf_nw_removeRxn(gf_network *, gf_reaction *);

    typedef gf_compartment gf_nw_getCompartment(gf_network *, uint64_t);

    typedef gf_compartment *gf_nw_getCompartmentp(gf_network *, uint64_t);

    typedef gf_compartment *gf_nw_findCompartmentById(gf_network *, const char *);

    typedef void gf_nw_rebuildCurves(gf_network *);

    typedef void gf_nw_recenterJunctions(gf_network *);

    typedef gf_compartment gf_nw_newCompartment(gf_network *, const char *, const char *);

    typedef gf_compartment *gf_nw_newCompartmentp(gf_network *, const char *, const char *);

    typedef gf_node gf_nw_newNode(gf_network *, const char *, const char *, gf_compartment *);

    typedef gf_node gf_nw_aliasOf(gf_network *, gf_node *);

    typedef gf_node *gf_nw_newNodep(gf_network *, const char *, const char *, gf_compartment *);

    typedef gf_node *gf_nw_newAliasNodep(gf_network *, gf_node *);

    typedef int gf_nw_removeNode(gf_network *, gf_node *);

    typedef int gf_nw_connectNode(gf_network *, gf_node *, gf_reaction *, gf_specRole);

    typedef int gf_nw_connectNodeRoleStr(gf_network *, gf_node *, gf_reaction *, const char *);

    typedef int gf_nw_isNodeConnected(gf_network *, gf_node *, gf_reaction *);

    typedef int gf_nw_isLayoutSpecified(gf_network *);

    typedef int gf_nw_getNumInstances(gf_network *, gf_node *);

    typedef int gf_nw_getNumAliasInstances(gf_network *, gf_node *);

    typedef gf_node gf_nw_getInstance(gf_network *, gf_node *, uint64_t);

    typedef gf_node *gf_nw_getInstancep(gf_network *, gf_node *, uint64_t);

    typedef gf_node *gf_nw_getAliasInstancep(gf_network *, gf_node *, uint64_t);

    typedef void gf_node_setCompartment(gf_node *, gf_compartment *);

    typedef void gf_clearNode(gf_node *);

    typedef void gf_releaseNode(const gf_node *);

    typedef int gf_node_isLocked(gf_node *);

    typedef void gf_node_lock(gf_node *);

    typedef void gf_node_unlock(gf_node *);

    typedef int gf_node_alias(gf_node *, gf_network *);

    typedef int gf_node_make_alias(gf_node *, gf_network *);

    typedef int gf_node_isAliased(gf_node *);

    typedef void gf_node_setIsAlias(gf_node *, int);

    typedef gf_point gf_node_getCentroid(gf_node *);

    typedef void gf_node_getCentroidXY(gf_node *, double *, double *);

    typedef void gf_node_setCentroid(gf_node *, gf_point);

    typedef double gf_node_getWidth(gf_node *);

    typedef void gf_node_setWidth(gf_node *, double);

    typedef double gf_node_getHeight(gf_node *);

    typedef void gf_node_setHeight(gf_node *, double);

    typedef char *gf_node_getID(gf_node *);

    typedef void gf_node_setID(gf_node *, const char *);

    typedef const char *gf_node_getName(gf_node *);

    typedef void gf_node_setName(gf_node *, const char *);

    typedef int gf_node_getConnectedReactions(gf_node *, gf_network *, unsigned int *, gf_reaction **);

    typedef int gf_node_getAttachedCurves(gf_node *, gf_network *, unsigned int *, gf_curve **);

    typedef int gf_node_isIdentical(gf_node *, gf_node *);

    typedef int gf_nw_nodeHasCompartment(gf_network *, gf_node *);

    typedef gf_compartment *gf_nw_nodeGetCompartment(gf_network *, gf_node *);

    typedef void gf_releaseRxn(const gf_reaction *);

    typedef gf_reaction gf_nw_newReaction(gf_network *, const char *, const char *);

    typedef gf_reaction *gf_nw_newReactionp(gf_network *, const char *, const char *);

    typedef char *gf_reaction_getID(gf_reaction *);

    typedef gf_point gf_reaction_getCentroid(gf_reaction *);

    typedef void gf_reaction_setCentroid(gf_reaction *, gf_point);

    typedef uint64_t gf_reaction_getNumSpec(const gf_reaction *);

    typedef int gf_reaction_hasSpec(const gf_reaction *, const gf_node *);

    typedef gf_specRole gf_reaction_getSpecRole(const gf_reaction *, uint64_t);

    typedef const char *gf_roleToStr(gf_specRole);

    typedef gf_specRole gf_strToRole(const char *);

    typedef uint64_t gf_reaction_specGeti(const gf_reaction *, uint64_t);

    typedef uint64_t gf_reaction_getNumCurves(const gf_reaction *);

    typedef gf_curve gf_reaction_getCurve(const gf_reaction *, uint64_t);

    typedef gf_curve *gf_reaction_getCurvep(const gf_reaction *, uint64_t);

    typedef void gf_reaction_recenter(gf_reaction *);

    typedef void gf_reaction_recalcCurveCPs(gf_reaction *);

    typedef void gf_releaseCurve(const gf_curve *);

    typedef char *gf_curve_getID(gf_curve *);

    typedef gf_specRole gf_curve_getRole(gf_curve *);

    typedef gf_curveCP gf_getCurveCPs(const gf_curve *);

    typedef int gf_curve_hasArrowhead(const gf_curve *);

    typedef int gf_curve_getArrowheadVerts(const gf_curve *, unsigned int *, gf_point **);

    typedef void gf_releaseCompartment(const gf_compartment *);

    typedef char *gf_compartment_getID(gf_compartment *);

    typedef gf_point gf_compartment_getMinCorner(gf_compartment *);

    typedef void gf_compartment_setMinCorner(gf_compartment *, gf_point);

    typedef gf_point gf_compartment_getMaxCorner(gf_compartment *);

    typedef void gf_compartment_setMaxCorner(gf_compartment *, gf_point);

    typedef double gf_compartment_getWidth(gf_compartment *);

    typedef double gf_compartment_getHeight(gf_compartment *);

    typedef uint64_t gf_compartment_getNumElt(gf_compartment *);

    typedef int gf_compartment_addNode(gf_compartment *, gf_node *);

    typedef int gf_compartment_removeNode(gf_compartment *, gf_node *);

    typedef int gf_compartment_containsNode(gf_compartment *, gf_node *);

    typedef int gf_compartment_containsReaction(gf_compartment *, gf_reaction *);

    typedef void gf_fit_to_window(gf_layoutInfo *, double, double, double, double);

    typedef gf_transform *gf_tf_fitToWindow(gf_layoutInfo *, double, double, double, double);

    typedef void gf_moveNetworkToFirstQuad(gf_layoutInfo *, double, double);

    typedef CPoint gf_tf_apply_to_point(gf_transform *, CPoint);

    typedef gf_point gf_tf_getScale(gf_transform *);

    typedef gf_point gf_tf_getDisplacement(gf_transform *);

    typedef gf_point gf_tf_getPostDisplacement(gf_transform *);

    typedef void gf_dump_transform(gf_transform *);

    typedef void gf_release_transform(gf_transform *);

    typedef gf_canvas gf_getCanvas(gf_layoutInfo *);

    typedef gf_canvas *gf_getCanvasp(gf_layoutInfo *);

    typedef void gf_clearCanvas(gf_canvas *);

    typedef void gf_releaseCanvas(gf_canvas *);

    typedef unsigned int gf_canvGetWidth(gf_canvas *);

    typedef unsigned int gf_canvGetHeight(gf_canvas *);

    typedef void gf_canvSetWidth(gf_canvas *, unsigned long);

    typedef void gf_canvSetHeight(gf_canvas *, unsigned long);

    typedef void gf_getNodeCentroid(gf_layoutInfo *, const char *, CPoint *);

    typedef int gf_lockNodeId(gf_layoutInfo *, const char *);

    typedef int gf_unlockNodeId(gf_layoutInfo *, const char *);

    typedef int gf_aliasNodeId(gf_layoutInfo *, const char *);

    typedef void gf_aliasNodebyDegree(gf_layoutInfo *, const int);

    typedef void gf_randomizeLayout(gf_layoutInfo *);

    typedef void gf_randomizeLayout2(gf_network *, gf_canvas *);

    typedef void gf_randomizeLayout_fromExtents(gf_network *, double, double, double, double);

    typedef int gf_writeSBMLwithLayout(const char *, gf_SBMLModel *, gf_layoutInfo *);

    typedef int gf_writeSBML(const char *, gf_SBMLModel *);

    typedef const char *gf_getSBMLwithLayoutStr(gf_SBMLModel *, gf_layoutInfo *);

    typedef const char *gf_getCurrentLibraryVersion();

    typedef void gf_free(void *);

    typedef gf_point gf_computeCubicBezierPoint(gf_curveCP *, double);

    typedef gf_point *gf_computeCubicBezierLineIntersec(gf_curveCP *, gf_point *, gf_point *);

    typedef int gf_arrowheadStyleGetNumVerts(int);

    typedef gf_point gf_arrowheadStyleGetVert(int, int);

    typedef int gf_arrowheadStyleIsFilled(int);

    typedef unsigned long gf_arrowheadNumStyles();

    typedef void gf_arrowheadSetStyle(gf_specRole, int);

    typedef int gf_arrowheadGetStyle(gf_specRole);
};


TEST_F(SBNWSharedTestFixture, t) {
    typedef const char *(*gf_getCurrentLibraryVersionPtr)();
    auto gf_getCurrentLibraryVersion = (gf_getCurrentLibraryVersionPtr) GetProcAddress(handle,
                                                                                       "gf_getCurrentLibraryVersion");
    if (gf_getCurrentLibraryVersion == nullptr) {
        GetLastError();
        throw std::invalid_argument("dead");
    }
    const char *version = gf_getCurrentLibraryVersion();
    if (version == nullptr) {
        GetLastError();
    }
    ASSERT_EQ("1.3.27", version);
}

TEST_F(SBNWSharedTestFixture, TestGetFunction) {
    typedef const char *(*gf_getCurrentLibraryVersionPtr)();
    auto getVersion = getFunction<gf_getCurrentLibraryVersionPtr>("gf_getCurrentLibraryVersion");
    const char *version = getVersion();
    if (version == nullptr) {
        GetLastError();
    }
    const char *expected = "1.3.27";
    ASSERT_STREQ(expected, version);
}
