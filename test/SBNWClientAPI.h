//
// Created by Ciaran on 3/1/2020.
//

#ifndef SBNW_SBNWCLIENTAPI_H
#define SBNW_SBNWCLIENTAPI_H

#if defined(_WIN32) || defined (_WIN64) || defined(__CYGWIN__)

#   include <windows.h>
#include <stdexcept>

#elif defined(__linux__)
#   include <dlfcn.h>
#endif

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

typedef struct __fr_options {
    /// Stiffness
    double k;
    int boundary;
    int mag;
    /// Strength of gravity (must be greater than 5 to have an effect)
    double grav;
    /// Center of gravitational force
    double baryx, baryy;
    /// Should the barycenter be set automatically from layout info?
    int autobary;
    /// Enable compartment-compartment and compartment-node interaction?
    int enable_comps;
    /// Randomize node positions before doing layout algo (library code DOES NOT call srand for reproducibility reasons)
    int prerandomize;
    /// Padding on compartments
    double padding;
} fr_options;


typedef struct __gf_SBMLLayout {
    void *pdoc; /// Pointer to SBMLDocument cast to void
} gf_SBMLModel;

typedef const char *(*gf_getCurrentLibraryVersionPtr)();

typedef void (*gf_freeSBMLModelPtr)(gf_SBMLModel *);

typedef gf_SBMLModel *(*gf_loadSBMLbufPtr)(const char *);

typedef gf_SBMLModel *(*gf_loadSBMLfilePtr)(const char *);

typedef void (*gf_freeLayoutInfoPtr)(gf_layoutInfo *);

typedef void (*gf_freeLayoutInfoHierarchPtr)(gf_layoutInfo *);

typedef void (*gf_freeModelAndLayoutPtr)(gf_SBMLModel *, gf_layoutInfo *);

typedef gf_SBMLModel (*gf_SBMLModel_newPtr)();

typedef gf_SBMLModel *(*gf_SBMLModel_newpPtr)();

typedef gf_layoutInfo (*gf_layoutInfo_newPtr)(uint64_t, uint64_t, uint64_t, uint64_t);

typedef gf_layoutInfo *(*gf_layoutInfo_newpPtr)(uint64_t, uint64_t, uint64_t, uint64_t);

typedef gf_layoutInfo *(*gf_processLayoutPtr)(gf_SBMLModel *);

typedef gf_layoutInfo *(*gf_loadSBMLIntoLayoutEnginePtr)(const char *, gf_SBMLModel *);

typedef void (*gf_setModelNamespacePtr)(gf_layoutInfo *, unsigned long, unsigned long);

typedef const char *(*gf_getDefaultCompartmentIdPtr)();

typedef void (*gf_setDefaultCompartmentIdPtr)(const char *);

typedef gf_network (*gf_getNetworkPtr)(gf_layoutInfo *);

typedef gf_network *(*gf_getNetworkpPtr)(gf_layoutInfo *);

typedef void (*gf_clearNetworkPtr)(gf_network *);

typedef void (*gf_releaseNetworkPtr)(gf_network *);

typedef char *(*gf_nw_getIdPtr)(gf_network *);

typedef void (*gf_nw_setIdPtr)(gf_network *, const char *);

typedef uint64_t (*gf_nw_getNumNodesPtr)(const gf_network *);

typedef uint64_t (*gf_nw_getNumUniqueNodesPtr)(const gf_network *);

typedef uint64_t (*gf_nw_getNumRxnsPtr)(const gf_network *);

typedef uint64_t (*gf_nw_getNumCompsPtr)(const gf_network *);

typedef gf_node (*gf_nw_getNodePtr)(gf_network *, uint64_t);

typedef gf_node (*gf_nw_getUniqueNodePtr)(gf_network *, uint64_t);

typedef gf_node *(*gf_nw_getNodepPtr)(gf_network *, uint64_t);

typedef gf_node *(*gf_nw_getUniqueNodepPtr)(gf_network *, uint64_t);

typedef gf_node *(*gf_nw_getNodepFromIdPtr)(gf_network *, const char *);

typedef gf_reaction (*gf_nw_getRxnPtr)(gf_network *, uint64_t);

typedef gf_reaction *(*gf_nw_getRxnpPtr)(gf_network *, uint64_t);

typedef void (*gf_nw_removeRxnPtr)(gf_network *, gf_reaction *);

typedef gf_compartment (*gf_nw_getCompartmentPtr)(gf_network *, uint64_t);

typedef gf_compartment *(*gf_nw_getCompartmentpPtr)(gf_network *, uint64_t);

typedef gf_compartment *(*gf_nw_findCompartmentByIdPtr)(gf_network *, const char *);

typedef void (*gf_nw_rebuildCurvesPtr)(gf_network *);

typedef void (*gf_nw_recenterJunctionsPtr)(gf_network *);

typedef gf_compartment (*gf_nw_newCompartmentPtr)(gf_network *, const char *, const char *);

typedef gf_compartment *(*gf_nw_newCompartmentpPtr)(gf_network *, const char *, const char *);

typedef gf_node (*gf_nw_newNodePtr)(gf_network *, const char *, const char *, gf_compartment *);

typedef gf_node (*gf_nw_aliasOfPtr)(gf_network *, gf_node *);

typedef gf_node *(*gf_nw_newNodepPtr)(gf_network *, const char *, const char *, gf_compartment *);

typedef gf_node *(*gf_nw_newAliasNodepPtr)(gf_network *, gf_node *);

typedef int (*gf_nw_removeNodePtr)(gf_network *, gf_node *);

typedef int (*gf_nw_connectNodePtr)(gf_network *, gf_node *, gf_reaction *, gf_specRole);

typedef int (*gf_nw_connectNodeRoleStrPtr)(gf_network *, gf_node *, gf_reaction *, const char *);

typedef int (*gf_nw_isNodeConnectedPtr)(gf_network *, gf_node *, gf_reaction *);

typedef int (*gf_nw_isLayoutSpecifiedPtr)(gf_network *);

typedef int (*gf_nw_getNumInstancesPtr)(gf_network *, gf_node *);

typedef int (*gf_nw_getNumAliasInstancesPtr)(gf_network *, gf_node *);

typedef gf_node (*gf_nw_getInstancePtr)(gf_network *, gf_node *, uint64_t);

typedef gf_node *(*gf_nw_getInstancepPtr)(gf_network *, gf_node *, uint64_t);

typedef gf_node *(*gf_nw_getAliasInstancepPtr)(gf_network *, gf_node *, uint64_t);

typedef void (*gf_node_setCompartmentPtr)(gf_node *, gf_compartment *);

typedef void (*gf_clearNodePtr)(gf_node *);

typedef void (*gf_releaseNodePtr)(const gf_node *);

typedef int (*gf_node_isLockedPtr)(gf_node *);

typedef void (*gf_node_lockPtr)(gf_node *);

typedef void (*gf_node_unlockPtr)(gf_node *);

typedef int (*gf_node_aliasPtr)(gf_node *, gf_network *);

typedef int (*gf_node_make_aliasPtr)(gf_node *, gf_network *);

typedef int (*gf_node_isAliasedPtr)(gf_node *);

typedef void (*gf_node_setIsAliasPtr)(gf_node *, int);

typedef gf_point (*gf_node_getCentroidPtr)(gf_node *);

typedef void (*gf_node_getCentroidXYPtr)(gf_node *, double *, double *);

typedef void (*gf_node_setCentroidPtr)(gf_node *, gf_point);

typedef double (*gf_node_getWidthPtr)(gf_node *);

typedef void (*gf_node_setWidthPtr)(gf_node *, double);

typedef double (*gf_node_getHeightPtr)(gf_node *);

typedef void (*gf_node_setHeightPtr)(gf_node *, double);

typedef char *(*gf_node_getIDPtr)(gf_node *);

typedef void (*gf_node_setIDPtr)(gf_node *, const char *);

typedef const char *(*gf_node_getNamePtr)(gf_node *);

typedef void (*gf_node_setNamePtr)(gf_node *, const char *);

typedef int (*gf_node_getConnectedReactionsPtr)(gf_node *, gf_network *, unsigned int *, gf_reaction **);

typedef int (*gf_node_getAttachedCurvesPtr)(gf_node *, gf_network *, unsigned int *, gf_curve **);

typedef int (*gf_node_isIdenticalPtr)(gf_node *, gf_node *);

typedef int (*gf_nw_nodeHasCompartmentPtr)(gf_network *, gf_node *);

typedef gf_compartment *(*gf_nw_nodeGetCompartmentPtr)(gf_network *, gf_node *);

typedef void (*gf_releaseRxnPtr)(const gf_reaction *);

typedef gf_reaction (*gf_nw_newReactionPtr)(gf_network *, const char *, const char *);

typedef gf_reaction *(*gf_nw_newReactionpPtr)(gf_network *, const char *, const char *);

typedef char *(*gf_reaction_getIDPtr)(gf_reaction *);

typedef gf_point (*gf_reaction_getCentroidPtr)(gf_reaction *);

typedef void (*gf_reaction_setCentroidPtr)(gf_reaction *, gf_point);

typedef uint64_t (*gf_reaction_getNumSpecPtr)(const gf_reaction *);

typedef int (*gf_reaction_hasSpecPtr)(const gf_reaction *, const gf_node *);

typedef gf_specRole (*gf_reaction_getSpecRolePtr)(const gf_reaction *, uint64_t);

typedef const char *(*gf_roleToStrPtr)(gf_specRole);

typedef gf_specRole (*gf_strToRolePtr)(const char *);

typedef uint64_t (*gf_reaction_specGetiPtr)(const gf_reaction *, uint64_t);

typedef uint64_t (*gf_reaction_getNumCurvesPtr)(const gf_reaction *);

typedef gf_curve (*gf_reaction_getCurvePtr)(const gf_reaction *, uint64_t);

typedef gf_curve *(*gf_reaction_getCurvepPtr)(const gf_reaction *, uint64_t);

typedef void (*gf_reaction_recenterPtr)(gf_reaction *);

typedef void (*gf_reaction_recalcCurveCPsPtr)(gf_reaction *);

typedef void (*gf_releaseCurvePtr)(const gf_curve *);

typedef char *(*gf_curve_getIDPtr)(gf_curve *);

typedef gf_specRole (*gf_curve_getRolePtr)(gf_curve *);

typedef gf_curveCP (*gf_getCurveCPsPtr)(const gf_curve *);

typedef int (*gf_curve_hasArrowheadPtr)(const gf_curve *);

typedef int (*gf_curve_getArrowheadVertsPtr)(const gf_curve *, unsigned int *, gf_point **);

typedef void (*gf_releaseCompartmentPtr)(const gf_compartment *);

typedef char *(*gf_compartment_getIDPtr)(gf_compartment *);

typedef gf_point (*gf_compartment_getMinCornerPtr)(gf_compartment *);

typedef void (*gf_compartment_setMinCornerPtr)(gf_compartment *, gf_point);

typedef gf_point (*gf_compartment_getMaxCornerPtr)(gf_compartment *);

typedef void (*gf_compartment_setMaxCornerPtr)(gf_compartment *, gf_point);

typedef double (*gf_compartment_getWidthPtr)(gf_compartment *);

typedef double (*gf_compartment_getHeightPtr)(gf_compartment *);

typedef uint64_t (*gf_compartment_getNumEltPtr)(gf_compartment *);

typedef int (*gf_compartment_addNodePtr)(gf_compartment *, gf_node *);

typedef int (*gf_compartment_removeNodePtr)(gf_compartment *, gf_node *);

typedef int (*gf_compartment_containsNodePtr)(gf_compartment *, gf_node *);

typedef int (*gf_compartment_containsReactionPtr)(gf_compartment *, gf_reaction *);

typedef void (*gf_fit_to_windowPtr)(gf_layoutInfo *, double, double, double, double);

typedef gf_transform *(*gf_tf_fitToWindowPtr)(gf_layoutInfo *, double, double, double, double);

typedef void (*gf_moveNetworkToFirstQuadPtr)(gf_layoutInfo *, double, double);

typedef CPoint (*gf_tf_apply_to_pointPtr)(gf_transform *, CPoint);

typedef gf_point (*gf_tf_getScalePtr)(gf_transform *);

typedef gf_point (*gf_tf_getDisplacementPtr)(gf_transform *);

typedef gf_point (*gf_tf_getPostDisplacementPtr)(gf_transform *);

typedef void (*gf_dump_transformPtr)(gf_transform *);

typedef void (*gf_release_transformPtr)(gf_transform *);

typedef gf_canvas (*gf_getCanvasPtr)(gf_layoutInfo *);

typedef gf_canvas *(*gf_getCanvaspPtr)(gf_layoutInfo *);

typedef void (*gf_clearCanvasPtr)(gf_canvas *);

typedef void (*gf_releaseCanvasPtr)(gf_canvas *);

typedef unsigned int (*gf_canvGetWidthPtr)(gf_canvas *);

typedef unsigned int (*gf_canvGetHeightPtr)(gf_canvas *);

typedef void (*gf_canvSetWidthPtr)(gf_canvas *, unsigned long);

typedef void (*gf_canvSetHeightPtr)(gf_canvas *, unsigned long);

typedef void (*gf_getNodeCentroidPtr)(gf_layoutInfo *, const char *, CPoint *);

typedef int (*gf_lockNodeIdPtr)(gf_layoutInfo *, const char *);

typedef int (*gf_unlockNodeIdPtr)(gf_layoutInfo *, const char *);

typedef int (*gf_aliasNodeIdPtr)(gf_layoutInfo *, const char *);

typedef void (*gf_aliasNodebyDegreePtr)(gf_layoutInfo *, const int);

typedef void (*gf_randomizeLayoutPtr)(gf_layoutInfo *);

typedef void (*gf_randomizeLayout2Ptr)(gf_network *, gf_canvas *);

typedef void (*gf_randomizeLayout_fromExtentsPtr)(gf_network *, double, double, double, double);

typedef int (*gf_writeSBMLwithLayoutPtr)(const char *, gf_SBMLModel *, gf_layoutInfo *);

typedef int (*gf_writeSBMLPtr)(const char *, gf_SBMLModel *);

typedef const char *(*gf_getSBMLwithLayoutStrPtr)(gf_SBMLModel *, gf_layoutInfo *);

typedef void (*gf_freePtr)(void *);

typedef gf_point (*gf_computeCubicBezierPointPtr)(gf_curveCP *, double);

typedef gf_point *(*gf_computeCubicBezierLineIntersecPtr)(gf_curveCP *, gf_point *, gf_point *);

typedef int (*gf_arrowheadStyleGetNumVertsPtr)(int);

typedef gf_point (*gf_arrowheadStyleGetVertPtr)(int, int);

typedef int (*gf_arrowheadStyleIsFilledPtr)(int);

typedef unsigned long (*gf_arrowheadNumStylesPtr)();

typedef void (*gf_arrowheadSetStylePtr)(gf_specRole, int);

typedef int (*gf_arrowheadGetStylePtr)(gf_specRole);

typedef void (*gf_doLayoutAlgorithmPtr)(fr_options, gf_layoutInfo *);

typedef void (*gf_doLayoutAlgorithm2Ptr)(fr_options, gf_network *, gf_canvas *);

typedef void (*gf_getLayoutOptDefaultsPtr)(fr_options *);

typedef void (*gf_layout_setStiffnessPtr)(fr_options *, double);

typedef int (*gf_generateRandomUniformIntPtr)(int, int);

typedef int (*gf_generateRandomUniformIntWithSeedPtr)(int, int, int);

class SBNWClientAPI {
private:

    void getLibrary() {
#if defined(_WIN32) || defined(__CYGWIN__)
        handle = LoadLibraryA(TEXT(lib));
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

    template<class T>
    T getFunction(const CHAR *funcName) {
#if defined(_WIN32)
        T func = (T) GetProcAddress(handle, reinterpret_cast<LPCSTR>(funcName));
        if (func == nullptr) {
            GetLastError();
        }
        return func;
    }

#elif defined(__linux__)

#elif defined(__APPLE__)

#endif
public:

#if defined(_WIN32)
    HINSTANCE handle;

#elif defined(__linux__)
    void* handle;
#elif defined(__APPLE__)
    // something else
#endif
    const char *lib;

    SBNWClientAPI(const char *lib = "sbnw.dll") : lib(lib) {
        getLibrary();
    }

    void gf_freeSBMLModel(gf_SBMLModel *lo) {
        auto function = getFunction<gf_freeSBMLModelPtr>("gf_freeSBMLModel");
        return function(lo);
    }

    gf_SBMLModel *gf_loadSBMLbuf(const char *buf) {
        auto function = getFunction<gf_loadSBMLbufPtr>("gf_loadSBMLbuf");
        return function(buf);
    }

    gf_SBMLModel *gf_loadSBMLfile(const char *file) {
        auto function = getFunction<gf_loadSBMLfilePtr>("gf_loadSBMLfile");
        return function(file);
    }

    void gf_freeLayoutInfo(gf_layoutInfo *l) {
        auto function = getFunction<gf_freeLayoutInfoPtr>("gf_freeLayoutInfo");
        return function(l);
    }

    void gf_freeLayoutInfoHierarch(gf_layoutInfo *l) {
        auto function = getFunction<gf_freeLayoutInfoHierarchPtr>("gf_freeLayoutInfoHierarch");
        return function(l);
    }

    void gf_freeModelAndLayout(gf_SBMLModel *mod, gf_layoutInfo *l) {
        auto function = getFunction<gf_freeModelAndLayoutPtr>("gf_freeModelAndLayout");
        return function(mod, l);
    }

    gf_SBMLModel gf_SBMLModel_new() {
        auto function = getFunction<gf_SBMLModel_newPtr>("gf_SBMLModel_new");
        return function();
    }

    gf_SBMLModel *gf_SBMLModel_newp() {
        auto function = getFunction<gf_SBMLModel_newpPtr>("gf_SBMLModel_newp");
        return function();
    }

    gf_layoutInfo gf_layoutInfo_new(uint64_t level, uint64_t version, uint64_t width, uint64_t height) {
        auto function = getFunction<gf_layoutInfo_newPtr>("gf_layoutInfo_new");
        return function(level, version, width, height);
    }

    gf_layoutInfo *gf_layoutInfo_newp(uint64_t level, uint64_t version, uint64_t width, uint64_t height) {
        auto function = getFunction<gf_layoutInfo_newpPtr>("gf_layoutInfo_newp");
        return function(level, version, width, height);
    }

    gf_layoutInfo *gf_processLayout(gf_SBMLModel *lo) {
        auto function = getFunction<gf_processLayoutPtr>("gf_processLayout");
        return function(lo);
    }

    gf_layoutInfo *gf_loadSBMLIntoLayoutEngine(const char *buf, gf_SBMLModel *r) {
        auto function = getFunction<gf_loadSBMLIntoLayoutEnginePtr>("gf_loadSBMLIntoLayoutEngine");
        return function(buf, r);
    }

    void gf_setModelNamespace(gf_layoutInfo *l, unsigned long level, unsigned long version) {
        auto function = getFunction<gf_setModelNamespacePtr>("gf_setModelNamespace");
        return function(l, level, version);
    }

    const char *gf_getDefaultCompartmentId() {
        auto function = getFunction<gf_getDefaultCompartmentIdPtr>("gf_getDefaultCompartmentId");
        return function();
    }

    void gf_setDefaultCompartmentId(const char *id) {
        auto function = getFunction<gf_setDefaultCompartmentIdPtr>("gf_setDefaultCompartmentId");
        return function(id);
    }

    gf_network gf_getNetwork(gf_layoutInfo *l) {
        auto function = getFunction<gf_getNetworkPtr>("gf_getNetwork");
        return function(l);
    }

    gf_network *gf_getNetworkp(gf_layoutInfo *l) {
        auto function = getFunction<gf_getNetworkpPtr>("gf_getNetworkp");
        return function(l);
    }

    void gf_clearNetwork(gf_network *n) {
        auto function = getFunction<gf_clearNetworkPtr>("gf_clearNetwork");
        return function(n);
    }

    void gf_releaseNetwork(gf_network *n) {
        auto function = getFunction<gf_releaseNetworkPtr>("gf_releaseNetwork");
        return function(n);
    }

    char *gf_nw_getId(gf_network *n) {
        auto function = getFunction<gf_nw_getIdPtr>("gf_nw_getId");
        return function(n);
    }

    void gf_nw_setId(gf_network *n, const char *id) {
        auto function = getFunction<gf_nw_setIdPtr>("gf_nw_setId");
        return function(n, id);
    }

    uint64_t gf_nw_getNumNodes(const gf_network *n) {
        auto function = getFunction<gf_nw_getNumNodesPtr>("gf_nw_getNumNodes");
        return function(n);
    }

    uint64_t gf_nw_getNumUniqueNodes(const gf_network *n) {
        auto function = getFunction<gf_nw_getNumUniqueNodesPtr>("gf_nw_getNumUniqueNodes");
        return function(n);
    }

    uint64_t gf_nw_getNumRxns(const gf_network *n) {
        auto function = getFunction<gf_nw_getNumRxnsPtr>("gf_nw_getNumRxns");
        return function(n);
    }

    uint64_t gf_nw_getNumComps(const gf_network *n) {
        auto function = getFunction<gf_nw_getNumCompsPtr>("gf_nw_getNumComps");
        return function(n);
    }

    gf_node gf_nw_getNode(gf_network *n, uint64_t i) {
        auto function = getFunction<gf_nw_getNodePtr>("gf_nw_getNode");
        return function(n, i);
    }

    gf_node gf_nw_getUniqueNode(gf_network *n, uint64_t i) {
        auto function = getFunction<gf_nw_getUniqueNodePtr>("gf_nw_getUniqueNode");
        return function(n, i);
    }

    gf_node *gf_nw_getNodep(gf_network *n, uint64_t i) {
        auto function = getFunction<gf_nw_getNodepPtr>("gf_nw_getNodep");
        return function(n, i);
    }

    gf_node *gf_nw_getUniqueNodep(gf_network *n, uint64_t i) {
        auto function = getFunction<gf_nw_getUniqueNodepPtr>("gf_nw_getUniqueNodep");
        return function(n, i);
    }

    gf_node *gf_nw_getNodepFromId(gf_network *nw, const char *id) {
        auto function = getFunction<gf_nw_getNodepFromIdPtr>("gf_nw_getNodepFromId");
        return function(nw, id);
    }

    gf_reaction gf_nw_getRxn(gf_network *n, uint64_t i) {
        auto function = getFunction<gf_nw_getRxnPtr>("gf_nw_getRxn");
        return function(n, i);
    }

    gf_reaction *gf_nw_getRxnp(gf_network *n, uint64_t i) {
        auto function = getFunction<gf_nw_getRxnpPtr>("gf_nw_getRxnp");
        return function(n, i);
    }

    void gf_nw_removeRxn(gf_network *n, gf_reaction *r) {
        auto function = getFunction<gf_nw_removeRxnPtr>("gf_nw_removeRxn");
        return function(n, r);
    }

    gf_compartment gf_nw_getCompartment(gf_network *n, uint64_t i) {
        auto function = getFunction<gf_nw_getCompartmentPtr>("gf_nw_getCompartment");
        return function(n, i);
    }

    gf_compartment *gf_nw_getCompartmentp(gf_network *n, uint64_t i) {
        auto function = getFunction<gf_nw_getCompartmentpPtr>("gf_nw_getCompartmentp");
        return function(n, i);
    }

    gf_compartment *gf_nw_findCompartmentById(gf_network *n, const char *id) {
        auto function = getFunction<gf_nw_findCompartmentByIdPtr>("gf_nw_findCompartmentById");
        return function(n, id);
    }

    void gf_nw_rebuildCurves(gf_network *n) {
        auto function = getFunction<gf_nw_rebuildCurvesPtr>("gf_nw_rebuildCurves");
        return function(n);
    }

    void gf_nw_recenterJunctions(gf_network *n) {
        auto function = getFunction<gf_nw_recenterJunctionsPtr>("gf_nw_recenterJunctions");
        return function(n);
    }

    gf_compartment gf_nw_newCompartment(gf_network *nw, const char *id, const char *name) {
        auto function = getFunction<gf_nw_newCompartmentPtr>("gf_nw_newCompartment");
        return function(nw, id, name);
    }

    gf_compartment *gf_nw_newCompartmentp(gf_network *nw, const char *id, const char *name) {
        auto function = getFunction<gf_nw_newCompartmentpPtr>("gf_nw_newCompartmentp");
        return function(nw, id, name);
    }

    gf_node gf_nw_newNode(gf_network *nw, const char *id, const char *name, gf_compartment *compartment) {
        auto function = getFunction<gf_nw_newNodePtr>("gf_nw_newNode");
        return function(nw, id, name, compartment);
    }

    gf_node gf_nw_aliasOf(gf_network *nw, gf_node *n) {
        auto function = getFunction<gf_nw_aliasOfPtr>("gf_nw_aliasOf");
        return function(nw, n);
    }

    gf_node *gf_nw_newNodep(gf_network *nw, const char *id, const char *name, gf_compartment *compartment) {
        auto function = getFunction<gf_nw_newNodepPtr>("gf_nw_newNodep");
        return function(nw, id, name, compartment);
    }

    gf_node *gf_nw_newAliasNodep(gf_network *nw, gf_node *source) {
        auto function = getFunction<gf_nw_newAliasNodepPtr>("gf_nw_newAliasNodep");
        return function(nw, source);
    }

    int gf_nw_removeNode(gf_network *nw, gf_node *node) {
        auto function = getFunction<gf_nw_removeNodePtr>("gf_nw_removeNode");
        return function(nw, node);
    }

    int gf_nw_connectNode(gf_network *nw, gf_node *node, gf_reaction *reaction, gf_specRole role) {
        auto function = getFunction<gf_nw_connectNodePtr>("gf_nw_connectNode");
        return function(nw, node, reaction, role);
    }

    int gf_nw_connectNodeRoleStr(gf_network *nw, gf_node *n, gf_reaction *r, const char *role_str) {
        auto function = getFunction<gf_nw_connectNodeRoleStrPtr>("gf_nw_connectNodeRoleStr");
        return function(nw, n, r, role_str);
    }

    int gf_nw_isNodeConnected(gf_network *nw, gf_node *node, gf_reaction *reaction) {
        auto function = getFunction<gf_nw_isNodeConnectedPtr>("gf_nw_isNodeConnected");
        return function(nw, node, reaction);
    }

    int gf_nw_isLayoutSpecified(gf_network *nw) {
        auto function = getFunction<gf_nw_isLayoutSpecifiedPtr>("gf_nw_isLayoutSpecified");
        return function(nw);
    }

    int gf_nw_getNumInstances(gf_network *nw, gf_node *n) {
        auto function = getFunction<gf_nw_getNumInstancesPtr>("gf_nw_getNumInstances");
        return function(nw, n);
    }

    int gf_nw_getNumAliasInstances(gf_network *nw, gf_node *n) {
        auto function = getFunction<gf_nw_getNumAliasInstancesPtr>("gf_nw_getNumAliasInstances");
        return function(nw, n);
    }

    gf_node gf_nw_getInstance(gf_network *nw, gf_node *n, uint64_t i) {
        auto function = getFunction<gf_nw_getInstancePtr>("gf_nw_getInstance");
        return function(nw, n, i);
    }

    gf_node *gf_nw_getInstancep(gf_network *nw, gf_node *n, uint64_t i) {
        auto function = getFunction<gf_nw_getInstancepPtr>("gf_nw_getInstancep");
        return function(nw, n, i);
    }

    gf_node *gf_nw_getAliasInstancep(gf_network *nw, gf_node *n, uint64_t i) {
        auto function = getFunction<gf_nw_getAliasInstancepPtr>("gf_nw_getAliasInstancep");
        return function(nw, n, i);
    }

    void gf_node_setCompartment(gf_node *n, gf_compartment *c) {
        auto function = getFunction<gf_node_setCompartmentPtr>("gf_node_setCompartment");
        return function(n, c);
    }

    void gf_clearNode(gf_node *n) {
        auto function = getFunction<gf_clearNodePtr>("gf_clearNode");
        return function(n);
    }

    void gf_releaseNode(const gf_node *n) {
        auto function = getFunction<gf_releaseNodePtr>("gf_releaseNode");
        return function(n);
    }

    int gf_node_isLocked(gf_node *n) {
        auto function = getFunction<gf_node_isLockedPtr>("gf_node_isLocked");
        return function(n);
    }

    void gf_node_lock(gf_node *n) {
        auto function = getFunction<gf_node_lockPtr>("gf_node_lock");
        return function(n);
    }

    void gf_node_unlock(gf_node *n) {
        auto function = getFunction<gf_node_unlockPtr>("gf_node_unlock");
        return function(n);
    }

    int gf_node_alias(gf_node *n, gf_network *m) {
        auto function = getFunction<gf_node_aliasPtr>("gf_node_alias");
        return function(n, m);
    }

    int gf_node_make_alias(gf_node *n, gf_network *m) {
        auto function = getFunction<gf_node_make_aliasPtr>("gf_node_make_alias");
        return function(n, m);
    }

    int gf_node_isAliased(gf_node *n) {
        auto function = getFunction<gf_node_isAliasedPtr>("gf_node_isAliased");
        return function(n);
    }

    void gf_node_setIsAlias(gf_node *n, int isAlias) {
        auto function = getFunction<gf_node_setIsAliasPtr>("gf_node_setIsAlias");
        return function(n, isAlias);
    }

    gf_point gf_node_getCentroid(gf_node *n) {
        auto function = getFunction<gf_node_getCentroidPtr>("gf_node_getCentroid");
        return function(n);
    }

    void gf_node_getCentroidXY(gf_node *n, double *x, double *y) {
        auto function = getFunction<gf_node_getCentroidXYPtr>("gf_node_getCentroidXY");
        return function(n, x, y);
    }

    void gf_node_setCentroid(gf_node *n, gf_point p) {
        auto function = getFunction<gf_node_setCentroidPtr>("gf_node_setCentroid");
        return function(n, p);
    }

    double gf_node_getWidth(gf_node *n) {
        auto function = getFunction<gf_node_getWidthPtr>("gf_node_getWidth");
        return function(n);
    }

    void gf_node_setWidth(gf_node *n, double width) {
        auto function = getFunction<gf_node_setWidthPtr>("gf_node_setWidth");
        return function(n, width);
    }

    double gf_node_getHeight(gf_node *n) {
        auto function = getFunction<gf_node_getHeightPtr>("gf_node_getHeight");
        return function(n);
    }

    void gf_node_setHeight(gf_node *n, double height) {
        auto function = getFunction<gf_node_setHeightPtr>("gf_node_setHeight");
        return function(n, height);
    }

    char *gf_node_getID(gf_node *n) {
        auto function = getFunction<gf_node_getIDPtr>("gf_node_getID");
        return function(n);
    }

    void gf_node_setID(gf_node *n, const char *id) {
        auto function = getFunction<gf_node_setIDPtr>("gf_node_setID");
        return function(n, id);
    }

    const char *gf_node_getName(gf_node *n) {
        auto function = getFunction<gf_node_getNamePtr>("gf_node_getName");
        return function(n);
    }

    void gf_node_setName(gf_node *n, const char *name) {
        auto function = getFunction<gf_node_setNamePtr>("gf_node_setName");
        return function(n, name);
    }

    int gf_node_getConnectedReactions(gf_node *n, gf_network *m, unsigned int *num, gf_reaction **rxns) {
        auto function = getFunction<gf_node_getConnectedReactionsPtr>("gf_node_getConnectedReactions");
        return function(n, m, num, rxns);
    }

    int gf_node_getAttachedCurves(gf_node *n, gf_network *m, unsigned int *num, gf_curve **curves) {
        auto function = getFunction<gf_node_getAttachedCurvesPtr>("gf_node_getAttachedCurves");
        return function(n, m, num, curves);
    }

    int gf_node_isIdentical(gf_node *u, gf_node *v) {
        auto function = getFunction<gf_node_isIdenticalPtr>("gf_node_isIdentical");
        return function(u, v);
    }

    int gf_nw_nodeHasCompartment(gf_network *nw, gf_node *x) {
        auto function = getFunction<gf_nw_nodeHasCompartmentPtr>("gf_nw_nodeHasCompartment");
        return function(nw, x);
    }

    gf_compartment *gf_nw_nodeGetCompartment(gf_network *nw, gf_node *x) {
        auto function = getFunction<gf_nw_nodeGetCompartmentPtr>("gf_nw_nodeGetCompartment");
        return function(nw, x);
    }

    void gf_releaseRxn(const gf_reaction *r) {
        auto function = getFunction<gf_releaseRxnPtr>("gf_releaseRxn");
        return function(r);
    }

    gf_reaction gf_nw_newReaction(gf_network *nw, const char *id, const char *name) {
        auto function = getFunction<gf_nw_newReactionPtr>("gf_nw_newReaction");
        return function(nw, id, name);
    }

    gf_reaction *gf_nw_newReactionp(gf_network *nw, const char *id, const char *name) {
        auto function = getFunction<gf_nw_newReactionpPtr>("gf_nw_newReactionp");
        return function(nw, id, name);
    }

    char *gf_reaction_getID(gf_reaction *r) {
        auto function = getFunction<gf_reaction_getIDPtr>("gf_reaction_getID");
        return function(r);
    }

    gf_point gf_reaction_getCentroid(gf_reaction *r) {
        auto function = getFunction<gf_reaction_getCentroidPtr>("gf_reaction_getCentroid");
        return function(r);
    }

    void gf_reaction_setCentroid(gf_reaction *r, gf_point p) {
        auto function = getFunction<gf_reaction_setCentroidPtr>("gf_reaction_setCentroid");
        return function(r, p);
    }

    uint64_t gf_reaction_getNumSpec(const gf_reaction *r) {
        auto function = getFunction<gf_reaction_getNumSpecPtr>("gf_reaction_getNumSpec");
        return function(r);
    }

    int gf_reaction_hasSpec(const gf_reaction *r, const gf_node *n) {
        auto function = getFunction<gf_reaction_hasSpecPtr>("gf_reaction_hasSpec");
        return function(r, n);
    }

    gf_specRole gf_reaction_getSpecRole(const gf_reaction *r, uint64_t i) {
        auto function = getFunction<gf_reaction_getSpecRolePtr>("gf_reaction_getSpecRole");
        return function(r, i);
    }

    const char *gf_roleToStr(gf_specRole role) {
        auto function = getFunction<gf_roleToStrPtr>("gf_roleToStr");
        return function(role);
    }

    gf_specRole gf_strToRole(const char *str) {
        auto function = getFunction<gf_strToRolePtr>("gf_strToRole");
        return function(str);
    }

    uint64_t gf_reaction_specGeti(const gf_reaction *r, uint64_t i) {
        auto function = getFunction<gf_reaction_specGetiPtr>("gf_reaction_specGeti");
        return function(r, i);
    }

    uint64_t gf_reaction_getNumCurves(const gf_reaction *r) {
        auto function = getFunction<gf_reaction_getNumCurvesPtr>("gf_reaction_getNumCurves");
        return function(r);
    }

    gf_curve gf_reaction_getCurve(const gf_reaction *r, uint64_t i) {
        auto function = getFunction<gf_reaction_getCurvePtr>("gf_reaction_getCurve");
        return function(r, i);
    }

    gf_curve *gf_reaction_getCurvep(const gf_reaction *r, uint64_t i) {
        auto function = getFunction<gf_reaction_getCurvepPtr>("gf_reaction_getCurvep");
        return function(r, i);
    }

    void gf_reaction_recenter(gf_reaction *r) {
        auto function = getFunction<gf_reaction_recenterPtr>("gf_reaction_recenter");
        return function(r);
    }

    void gf_reaction_recalcCurveCPs(gf_reaction *r) {
        auto function = getFunction<gf_reaction_recalcCurveCPsPtr>("gf_reaction_recalcCurveCPs");
        return function(r);
    }

    void gf_releaseCurve(const gf_curve *c) {
        auto function = getFunction<gf_releaseCurvePtr>("gf_releaseCurve");
        return function(c);
    }

    char *gf_curve_getID(gf_curve *c) {
        auto function = getFunction<gf_curve_getIDPtr>("gf_curve_getID");
        return function(c);
    }

    gf_specRole gf_curve_getRole(gf_curve *c) {
        auto function = getFunction<gf_curve_getRolePtr>("gf_curve_getRole");
        return function(c);
    }

    gf_curveCP gf_getCurveCPs(const gf_curve *c) {
        auto function = getFunction<gf_getCurveCPsPtr>("gf_getCurveCPs");
        return function(c);
    }

    int gf_curve_hasArrowhead(const gf_curve *c) {
        auto function = getFunction<gf_curve_hasArrowheadPtr>("gf_curve_hasArrowhead");
        return function(c);
    }

    int gf_curve_getArrowheadVerts(const gf_curve *c, unsigned int *n, gf_point **v) {
        auto function = getFunction<gf_curve_getArrowheadVertsPtr>("gf_curve_getArrowheadVerts");
        return function(c, n, v);
    }

    void gf_releaseCompartment(const gf_compartment *c) {
        auto function = getFunction<gf_releaseCompartmentPtr>("gf_releaseCompartment");
        return function(c);
    }

    char *gf_compartment_getID(gf_compartment *c) {
        auto function = getFunction<gf_compartment_getIDPtr>("gf_compartment_getID");
        return function(c);
    }

    gf_point gf_compartment_getMinCorner(gf_compartment *c) {
        auto function = getFunction<gf_compartment_getMinCornerPtr>("gf_compartment_getMinCorner");
        return function(c);
    }

    void gf_compartment_setMinCorner(gf_compartment *c, gf_point p) {
        auto function = getFunction<gf_compartment_setMinCornerPtr>("gf_compartment_setMinCorner");
        return function(c, p);
    }

    gf_point gf_compartment_getMaxCorner(gf_compartment *c) {
        auto function = getFunction<gf_compartment_getMaxCornerPtr>("gf_compartment_getMaxCorner");
        return function(c);
    }

    void gf_compartment_setMaxCorner(gf_compartment *c, gf_point p) {
        auto function = getFunction<gf_compartment_setMaxCornerPtr>("gf_compartment_setMaxCorner");
        return function(c, p);
    }

    double gf_compartment_getWidth(gf_compartment *c) {
        auto function = getFunction<gf_compartment_getWidthPtr>("gf_compartment_getWidth");
        return function(c);
    }

    double gf_compartment_getHeight(gf_compartment *c) {
        auto function = getFunction<gf_compartment_getHeightPtr>("gf_compartment_getHeight");
        return function(c);
    }

    uint64_t gf_compartment_getNumElt(gf_compartment *c) {
        auto function = getFunction<gf_compartment_getNumEltPtr>("gf_compartment_getNumElt");
        return function(c);
    }

    int gf_compartment_addNode(gf_compartment *c, gf_node *n) {
        auto function = getFunction<gf_compartment_addNodePtr>("gf_compartment_addNode");
        return function(c, n);
    }

    int gf_compartment_removeNode(gf_compartment *c, gf_node *n) {
        auto function = getFunction<gf_compartment_removeNodePtr>("gf_compartment_removeNode");
        return function(c, n);
    }

    int gf_compartment_containsNode(gf_compartment *c, gf_node *n) {
        auto function = getFunction<gf_compartment_containsNodePtr>("gf_compartment_containsNode");
        return function(c, n);
    }

    int gf_compartment_containsReaction(gf_compartment *c, gf_reaction *r) {
        auto function = getFunction<gf_compartment_containsReactionPtr>("gf_compartment_containsReaction");
        return function(c, r);
    }

    void gf_fit_to_window(gf_layoutInfo *l, double left, double top, double right, double bottom) {
        auto function = getFunction<gf_fit_to_windowPtr>("gf_fit_to_window");
        return function(l, left, top, right, bottom);
    }

    gf_transform *gf_tf_fitToWindow(gf_layoutInfo *l, double left, double top, double right, double bottom) {
        auto function = getFunction<gf_tf_fitToWindowPtr>("gf_tf_fitToWindow");
        return function(l, left, top, right, bottom);
    }

    void gf_moveNetworkToFirstQuad(gf_layoutInfo *l, double x_disp, double y_disp) {
        auto function = getFunction<gf_moveNetworkToFirstQuadPtr>("gf_moveNetworkToFirstQuad");
        return function(l, x_disp, y_disp);
    }

    CPoint gf_tf_apply_to_point(gf_transform *tf, CPoint p) {
        auto function = getFunction<gf_tf_apply_to_pointPtr>("gf_tf_apply_to_point");
        return function(tf, p);
    }

    gf_point gf_tf_getScale(gf_transform *tf) {
        auto function = getFunction<gf_tf_getScalePtr>("gf_tf_getScale");
        return function(tf);
    }

    gf_point gf_tf_getDisplacement(gf_transform *tf) {
        auto function = getFunction<gf_tf_getDisplacementPtr>("gf_tf_getDisplacement");
        return function(tf);
    }

    gf_point gf_tf_getPostDisplacement(gf_transform *tf) {
        auto function = getFunction<gf_tf_getPostDisplacementPtr>("gf_tf_getPostDisplacement");
        return function(tf);
    }

    void gf_dump_transform(gf_transform *tf) {
        auto function = getFunction<gf_dump_transformPtr>("gf_dump_transform");
        return function(tf);
    }

    void gf_release_transform(gf_transform *tf) {
        auto function = getFunction<gf_release_transformPtr>("gf_release_transform");
        return function(tf);
    }

    gf_canvas gf_getCanvas(gf_layoutInfo *l) {
        auto function = getFunction<gf_getCanvasPtr>("gf_getCanvas");
        return function(l);
    }

    gf_canvas *gf_getCanvasp(gf_layoutInfo *l) {
        auto function = getFunction<gf_getCanvaspPtr>("gf_getCanvasp");
        return function(l);
    }

    void gf_clearCanvas(gf_canvas *c) {
        auto function = getFunction<gf_clearCanvasPtr>("gf_clearCanvas");
        return function(c);
    }

    void gf_releaseCanvas(gf_canvas *c) {
        auto function = getFunction<gf_releaseCanvasPtr>("gf_releaseCanvas");
        return function(c);
    }

    unsigned int gf_canvGetWidth(gf_canvas *c) {
        auto function = getFunction<gf_canvGetWidthPtr>("gf_canvGetWidth");
        return function(c);
    }

    unsigned int gf_canvGetHeight(gf_canvas *c) {
        auto function = getFunction<gf_canvGetHeightPtr>("gf_canvGetHeight");
        return function(c);
    }

    void gf_canvSetWidth(gf_canvas *c, unsigned long width) {
        auto function = getFunction<gf_canvSetWidthPtr>("gf_canvSetWidth");
        return function(c, width);
    }

    void gf_canvSetHeight(gf_canvas *c, unsigned long height) {
        auto function = getFunction<gf_canvSetHeightPtr>("gf_canvSetHeight");
        return function(c, height);
    }

    void gf_getNodeCentroid(gf_layoutInfo *l, const char *id, CPoint *p) {
        auto function = getFunction<gf_getNodeCentroidPtr>("gf_getNodeCentroid");
        return function(l, id, p);
    }

    int gf_lockNodeId(gf_layoutInfo *l, const char *id) {
        auto function = getFunction<gf_lockNodeIdPtr>("gf_lockNodeId");
        return function(l, id);
    }

    int gf_unlockNodeId(gf_layoutInfo *l, const char *id) {
        auto function = getFunction<gf_unlockNodeIdPtr>("gf_unlockNodeId");
        return function(l, id);
    }

    int gf_aliasNodeId(gf_layoutInfo *l, const char *id) {
        auto function = getFunction<gf_aliasNodeIdPtr>("gf_aliasNodeId");
        return function(l, id);
    }

    void gf_aliasNodebyDegree(gf_layoutInfo *l, const int minDegree) {
        auto function = getFunction<gf_aliasNodebyDegreePtr>("gf_aliasNodebyDegree");
        return function(l, minDegree);
    }

    void gf_randomizeLayout(gf_layoutInfo *m) {
        auto function = getFunction<gf_randomizeLayoutPtr>("gf_randomizeLayout");
        return function(m);
    }

    void gf_randomizeLayout2(gf_network *n, gf_canvas *c) {
        auto function = getFunction<gf_randomizeLayout2Ptr>("gf_randomizeLayout2");
        return function(n, c);
    }

    void gf_randomizeLayout_fromExtents(gf_network *n, double left, double top, double right, double bottom) {
        auto function = getFunction<gf_randomizeLayout_fromExtentsPtr>("gf_randomizeLayout_fromExtents");
        return function(n, left, top, right, bottom);
    }

    int gf_writeSBMLwithLayout(const char *filename, gf_SBMLModel *m, gf_layoutInfo *l) {
        auto function = getFunction<gf_writeSBMLwithLayoutPtr>("gf_writeSBMLwithLayout");
        return function(filename, m, l);
    }

    int gf_writeSBML(const char *filename, gf_SBMLModel *m) {
        auto function = getFunction<gf_writeSBMLPtr>("gf_writeSBML");
        return function(filename, m);
    }

    const char *gf_getSBMLwithLayoutStr(gf_SBMLModel *m, gf_layoutInfo *l) {
        auto function = getFunction<gf_getSBMLwithLayoutStrPtr>("gf_getSBMLwithLayoutStr");
        return function(m, l);
    }

    const char *gf_getCurrentLibraryVersion() {
        auto function = getFunction<gf_getCurrentLibraryVersionPtr>("gf_getCurrentLibraryVersion");
        return function();
    }

    void gf_free(void *x) {
        auto function = getFunction<gf_freePtr>("gf_free");
        return function(x);
    }

    gf_point gf_computeCubicBezierPoint(gf_curveCP *c, double t) {
        auto function = getFunction<gf_computeCubicBezierPointPtr>("gf_computeCubicBezierPoint");
        return function(c, t);
    }

    gf_point *gf_computeCubicBezierLineIntersec(gf_curveCP *c, gf_point *line_start, gf_point *line_end) {
        auto function = getFunction<gf_computeCubicBezierLineIntersecPtr>("gf_computeCubicBezierLineIntersec");
        return function(c, line_start, line_end);
    }

    int gf_arrowheadStyleGetNumVerts(int style) {
        auto function = getFunction<gf_arrowheadStyleGetNumVertsPtr>("gf_arrowheadStyleGetNumVerts");
        return function(style);
    }

    gf_point gf_arrowheadStyleGetVert(int style, int n) {
        auto function = getFunction<gf_arrowheadStyleGetVertPtr>("gf_arrowheadStyleGetVert");
        return function(style, n);
    }

    int gf_arrowheadStyleIsFilled(int style) {
        auto function = getFunction<gf_arrowheadStyleIsFilledPtr>("gf_arrowheadStyleIsFilled");
        return function(style);
    }

    unsigned long gf_arrowheadNumStyles() {
        auto function = getFunction<gf_arrowheadNumStylesPtr>("gf_arrowheadNumStyles");
        return function();
    }

    void gf_arrowheadSetStyle(gf_specRole role, int style) {
        auto function = getFunction<gf_arrowheadSetStylePtr>("gf_arrowheadSetStyle");
        return function(role, style);
    }

    int gf_arrowheadGetStyle(gf_specRole role) {
        auto function = getFunction<gf_arrowheadGetStylePtr>("gf_arrowheadGetStyle");
        return function(role);
    }


    void gf_doLayoutAlgorithm(fr_options opt, gf_layoutInfo *l) {
        auto function = getFunction<gf_doLayoutAlgorithmPtr>("gf_doLayoutAlgorithm");
        return function(opt, l);
    }


    void gf_doLayoutAlgorithm2(fr_options opt, gf_network *n, gf_canvas *c) {
        auto function = getFunction<gf_doLayoutAlgorithm2Ptr>("gf_doLayoutAlgorithm2");
        return function(opt, n, c);
    }


    void gf_getLayoutOptDefaults(fr_options *opt) {
        auto function = getFunction<gf_getLayoutOptDefaultsPtr>("gf_getLayoutOptDefaults");
        return function(opt);
    }

    void gf_layout_setStiffness(fr_options *opt, double k) {
        auto function = getFunction<gf_layout_setStiffnessPtr>("gf_layout_setStiffness");
        return function(opt, k);
    }

    int gf_generateRandomUniformInt(int low, int high) {
        auto function = getFunction<gf_generateRandomUniformIntPtr>("gf_generateRandomUniformInt");
        return function(low, high);
    }

    int gf_generateRandomUniformIntWithSeed(int low, int high, int seed) {
        auto function = getFunction<gf_generateRandomUniformIntWithSeedPtr>("gf_generateRandomUniformIntWithSeed");
        return function(low, high, seed);
    }

};

#endif //SBNW_SBNWCLIENTAPI_H
