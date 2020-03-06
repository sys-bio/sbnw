/*== SAGITTARIUS =====================================================================
 * Copyright (c) 2012, Jesse K Medley
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of The University of Washington nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//== BEGINNING OF CODE ===============================================================

//== INCLUDES ========================================================================

#include "graphfab/core/SagittariusCore.h"
#include "fr.h"
#include "canvas.h"
#include "graphfab/math/rand_unif.h"
#include "graphfab/math/min_max.h"
#include "graphfab/math/dist.h"
#include "graphfab/math/transform.h"

#if SBNW_USE_MAGICK
#include "graphfab/draw/magick.h"
#endif

#include <sstream>

//#include <math.h>

#define e 2.71828182845905

//#include <fenv.h>

static bool dumpForces_ = false;

void gf_getLayoutOptDefaults(fr_options *opt) {
    opt->k = 50.;
    opt->boundary = 0;
    opt->mag = 0;
    opt->grav = 0.;
    opt->baryx = opt->baryy = 500.;
    opt->autobary = 1;
    opt->enable_comps = 0;
    opt->prerandomize = 0;
    opt->padding = 15;
}

void gf_layout_setStiffness(fr_options *opt, double k) {
    opt->k = k;
}

void gf_doLayoutAlgorithm(fr_options opt, gf_layoutInfo *l) {
    using namespace Graphfab;

    auto *net = (Network *) l->net;
    AN(net, "No network");
    auto *can = (Canvas *) l->canv;
    AN(can, "No canvas");

    if (opt.prerandomize) {
        double w = can->getWidth();
        double h = can->getHeight();
        net->randomizePositions(
                Graphfab::Box(
                        Graphfab::Point(0., 0.),
                        Graphfab::Point(w, h)
                )
        );
    }

    FruchtermanReingold(opt, *net, can, l);
}

namespace Graphfab {

    bool eltTypesInteract(const NetworkEltType a, const NetworkEltType b, fr_options *opt) {
        if (!opt->enable_comps && typeMatchEither(a, b, NET_ELT_TYPE_COMP))
            return false;
        if (typeMatchEither(a, b, NET_ELT_TYPE_RXN) && typeMatchEither(a, b, NET_ELT_TYPE_COMP))
            //reactions & comps don't interact
            return false;
        return true;
    }

    double calc_fr(const double k, const double d) {
        return k * k / d;
    }

    double calc_fa(const double k, const double d) {
        return d * d / k;
    }

    // compute the internal force between a compartment & its node
    void do_internalForce(NetworkElement *u, Compartment &c, double k) {
        c.doInternalForce(u, k * k, 10.);
    }

    // compute the repulsion force between two elements & apply it
    void do_repulForce(NetworkElement &u, NetworkElement &v, double k, uint64 num) {

        Point delta(u.centroidDisplacementFrom(v).normed());
        Point f(0, 0);

        // needs to be large because force is no longer calculated on the
        // basis of centroids
        double ep = 1e-6;

        double d = max(u.centroidDisplacementFrom(v).mag(), 0.1);

        if (u.centroidDisplacementFrom(v).mag2() < ep) {
            // repel nodes very close together with a large force of unspecified magnitude
            double extreme = 100. * sqrt((double) num);
            f = Point(rand_range(-extreme, extreme), rand_range(-extreme, extreme));

        } else {
            double adjk = (k * log((double) u.degree() + v.degree() + 2) +
                           (max(v.getWidth(), v.getHeight()) + max(u.getWidth(), u.getHeight())) / 4);
//             std::cerr << "max(v.getWidth(), v.getHeight()) = " << max(v.getWidth(), v.getHeight()) << ", max(u.getWidth(), u.getHeight()) = " << max(u.getWidth(), u.getHeight()) << "\n";
            f = Point(delta * calc_fr(adjk, d));

            // compartment repulsion
            if (u.getType() == NET_ELT_TYPE_COMP && v.getType() == NET_ELT_TYPE_COMP) {
                f = 0.01 * f;
                if (d > 25.)
                    f = Point(0, 0);
            }
        }

        u.addDelta(f);

        v.addDelta(-f);
    }

    // apply the attraction force
    void do_attForce(NetworkElement &u, NetworkElement &v, double k) {
        Point delta(u.centroidDisplacementFrom(v).normed());

        double ep = 1.e-6;

        double d = u.centroidDisplacementFrom(v).mag();

        if (d > ep) {
            //double invd = 1./d;

            //double adjk = k*log((double)u.degree()+v.degree()+2);
            double adjk = (k * log((double) u.degree() + v.degree() + 2) +
                           (max(v.getWidth(), v.getHeight()) + max(u.getWidth(), u.getHeight())) / 4);
            u.addDelta(-delta * calc_fa(u.getType() == NET_ELT_TYPE_RXN ? k : adjk, d));
            if (dumpForces_)
                std::cerr << "attr force bet " << eltTypeToStr(u.getType()) << " & " << eltTypeToStr(v.getType())
                          << ": " << (delta * calc_fa(u.getType() == NET_ELT_TYPE_RXN ? k : adjk, d)).mag() / d << "\n";

            v.addDelta(delta * calc_fa(v.getType() == NET_ELT_TYPE_RXN ? k : adjk, d));
        }
    }

    // apply "gravitational" force
    void do_gravity(NetworkElement &u, Point bary, double strength, double k) {
        Point delta = u.getCentroid() - bary;

        if (delta.mag() < 1e-2)
            return;

        double adjk = strength / k;
        u.addDelta(-delta * adjk);
    }

    void FruchtermanReingold(fr_options opt, Network &net, Canvas *can, gf_layoutInfo *l) {
        std::cout <<  "opt.k: " << opt.k << std::endl;
        std::cout <<  "opt.boundary: " << opt.boundary << std::endl;
        std::cout <<  "opt.mag: " << opt.mag<< std::endl;
        std::cout <<  "opt.grav: " << opt.grav<< std::endl;
        std::cout <<  "opt.baryx: " << opt.baryx << std::endl;
        std::cout <<  "opt.baryx: " << opt.baryx << std::endl;
        std::cout <<  "opt.autobary: " << opt.autobary << std::endl;
        std::cout <<  "opt.prerandomize: " << opt.prerandomize << std::endl;
        std::cout <<  "opt.enable_comps: " << opt.enable_comps << std::endl;
        std::cout <<  "opt.padding: " << opt.padding << std::endl;

        Box bound;
        if (opt.boundary) {
            AN(can, "Boundary specified but no canvas");
            bound = can->getBox();
            if (bound.canShrink(20.))
                bound.shrink_(20.);
            if (opt.autobary) {
                //adjust barycenter
                opt.baryx = can->getWidth() * 0.5;
                opt.baryy = can->getHeight() * 0.5;
            }
        }

        uint64 num = net.getTotalNumPts();
        uint64 m = 100. * log((double) num + 2);

        double k = opt.k;

        // initial temperature
        double Ti = 1000. * log((double) num + 2);
        // Current temp
        double T;

        // time
        double t = 0.;
        // delta time
        double dt = 1. / m;

        double alpha = log(Ti / 0.25);

        double ep = 1.e-6;

        dumpForces_ = false;

        for (uint64 z = 0; z < m; ++z) {
            T = Ti * pow(e, -alpha * t);
            t += dt;

            net.resetActivity();

            net.updateExtents();

            // repulsive forces
            for (uint64 i = 0; i < net.getNElts(); ++i) {
                //NetworkElement* u = *i;
                NetworkElement *u = net.getElt(i);;

                // is the network element a container?
                Graphfab::Compartment *comp = nullptr;
                if (u->getType() == NET_ELT_TYPE_COMP)
                    comp = dynamic_cast<Compartment *>(u); //get the associated compartment

                for (uint64 j = i + 1; j < net.getNElts(); ++j) {
                    NetworkElement *v = net.getElt(j);
                    AT(u != v, "Cannot apply to self");
                    if (!eltTypesInteract(u->getType(), v->getType(), &opt))
                        continue; // elements do not interact

                    if (opt.enable_comps) {
                        if (u->getType() == NET_ELT_TYPE_COMP && v->getType() == NET_ELT_TYPE_COMP) {
                            do_repulForce(*u, *v, k, num);
                            continue;
                        }

                        if (!comp && v->getType() == NET_ELT_TYPE_COMP) {
                            comp = dynamic_cast<Compartment *>(v); //get the associated compartment
                            v = u;
                        }

                        if (comp) {
                            if (v->getType() != NET_ELT_TYPE_COMP) {
                                if (comp->contains(v)) {
                                    // node inside a compartment
                                    comp->doInternalForce(v, k * k, 10.0);
//                                    comp->doInternalForce(v, k, 10.0);
                                    continue;
                                }
                            }
                        }
                    }
                    do_repulForce(*u, *v, k, num);
                }
            }

            // attractive forces
            for (auto i = net.RxnsBegin(); i != net.RxnsEnd(); ++i) {
                Reaction *u = *i;
                for (auto j = u->NodesBegin(); j != u->NodesEnd(); ++j) {
                    Node *v = j->first;
                    do_attForce(*u, *v, k);
                }
            }

            if (opt.grav >= 5.) {
                for (uint64 i = 0; i < net.getNElts(); ++i) {
                    NetworkElement *u = net.getElt(i);;
                    if (u->getType() == NET_ELT_TYPE_SPEC) {
                        do_gravity(*u, Point(opt.baryx, opt.baryy), opt.grav, k);
                    }
                }
            }

            net.capDeltas(T);

            net.updatePositions(T);
//            Box newBox(
//                    net.getExtents().getMinX(),
//                    net.getExtents().getMinY(),
//                    net.getExtents().getMaxX(),
//                    net.getExtents().getMaxY()
//                    );
//            net.fitToWindow(newBox);

            net.alignToOrigin(); // not yet implemented

#if SBNW_USE_MAGICK && 0
            if(!(z % 10)) {
                net.rebuildCurves();
                std::stringstream ss;
                ss << "/tmp/tmpfs/pic" << z/10 << ".png";
                double w,h;
                if(can) {
                    w = can->getWidth();
                    h = can->getHeight();
                } else {
                    w = h = 1024.;
                }
                Affine2d view(Affine2d::makeXlate(-net.center())*
                    Affine2d::makeScale(0.5, 0.5)*Affine2d::makeXlate(Point(w*3/2,h*3/2)));
                AN(l, "Need layout to write images");
                gf_MagickRenderToFile(l, ss.str().c_str(), &view);
            }
#endif
        }

        if (!opt.enable_comps)
            net.resizeCompsEnclose(opt.padding);

        net.rebuildCurves();
    }

}
