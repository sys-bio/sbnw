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

//== FILEDOC =========================================================================

/** @file canvas.h
 * @brief Canvas for drawing diagram, dimensions
  */

//== BEGINNING OF CODE ===============================================================

#ifndef __SBNW_LAYOUT_CURVE_H_
#define __SBNW_LAYOUT_CURVE_H_

//== INCLUDES ========================================================================

#include "graphfab/core/SagittariusCore.h"
#include "point.h"
#include "graphfab/math/transform.h"
#include "arrowhead.h"

//-- C++ code --
#ifdef __cplusplus

#include <string>

#include <iostream>

namespace Graphfab {

    /// Role for the curve (substrate, product, etc.)
    typedef enum {
        RXN_CURVE_SUBSTRATE,
        RXN_CURVE_PRODUCT,
        RXN_CURVE_ACTIVATOR,
        RXN_CURVE_INHIBITOR,
        RXN_CURVE_MODIFIER
    } RxnCurveType;

    class Node;

    /** @brief A Bezier curve
     * @details One curve represents one species in a reaction.
     * The curves are drawn in different representative styles for
     * activators, inhibitors etc.
     */

    class RxnBezier {
        public:
            RxnBezier() {
              ns = ne = NULL;
            }

            virtual ~RxnBezier() {
                if(as && owns)
                    delete as;
                if(ae && owne)
                    delete ae;
            }

            /// Get the role of this curve
            virtual RxnCurveType getRole() const = 0;

            void applyTransform(const Affine2d& t) {
                s  = xformPoint(s, t);
                c1 = xformPoint(c1,t);
                c2 = xformPoint(c2,t);
                e  = xformPoint(e, t);
            }

            /// Start anchor
            Point* as;
            /// Does this object own the start point (i.e. weak vs. strong ref)?
            int owns;

            /// End anchor
            Point* ae;
            /// Does this object own the end point?
            int owne;

            Node* ns;
            Node* ne;

            //Calculated points:
            /// Start & end point resp., control points
            Point s, e, c1, c2;

            Point getTransformedS() const { return tf_*s; }
            Point getTransformedE() const { return tf_*e; }
            Point getTransformedC1() const { return tf_*c1; }
            Point getTransformedC2() const { return tf_*c2; }

            virtual Point getCentroidCP() const = 0;

            virtual bool isStartNodeSide() const = 0;

            Point getNodeSideCP() const {
              if (isStartNodeSide())
                return c1;
              else
                return c2;
            }

            void setNodeSideCP(const Point& p) {
              if (isStartNodeSide())
                c1 = p;
              else
                c2 = p;
            }

            Point getNodeSide() const {
              if (isStartNodeSide())
                return s;
              else
                return e;
            }

            void setNodeSide(const Point& p) {
              if (isStartNodeSide())
                s = p;
              else
                e = p;
            }

            Node* getNodeUsed() const {
              if (isStartNodeSide())
                return ns;
              else
                return ne;
            }

            bool includes(const Node* n) {
              if (ns ==  n)
                return true;
              else if (ne == n)
                return true;
              else
                return false;
            }

            virtual bool hasArrowhead() const { return false; }

            void transformArrowhead(Arrowhead& a) {
              Point v;
              if ((e - c2).mag2() < 1e-2)
                v = (e - s).normed() * 5.;
              else
                v = (e - c2).normed() * 5.;
              Point u = v.dextro();

              a.setTransform(tf_*Affine2d::fromBasis(u, v, e));
              a.setInverseTransform(a.getTransform().inv());
            }

            // For non-curved segments
//             void transformArrowheadFlat(Arrowhead& a) {
//               Point v = (e - s).normed() * 5.;
//               Point u = v.dextro();
//
//               a.setTransform(tf_*Affine2d::fromBasis(u, v, e));
//               a.setInverseTransform(a.getTransform().inv());
//             }

            /// Callee owns
            virtual Arrowhead* getArrowhead() {
              Arrowhead* result = new PlainArrowhead();
              transformArrowhead(*result);
              return result;
            }

            Affine2d getTransform() const { return tf_; }

            void setTransform(const Affine2d& tf, bool recurse = true) { tf_ = tf; }

            Affine2d getInverseTransform() const { return itf_; }

            void setInverseTransform(const Affine2d& itf, bool recurse = true) { itf_ = itf; }

            virtual ArrowheadStyle getArrowheadStyle() const = 0;

            /** Clips the last point at the specified interpolation value
              * and adjusts the control point to match
            */
            std::pair<Point,Point> clipForward(double t) const {
              Point p1(s), p2(c1), p3(c2), p4(e);
              Point q1(p1.interpolate(p2, t)),
                    q2(p2.interpolate(p3, t)),
                    q3(p3.interpolate(p4, t));
              Point r1(q1.interpolate(q2, t)),
                    r2(q2.interpolate(q3, t));
              Point s1(r1.interpolate(r2, t));

              return std::make_pair(s1,r2);
            }

            void clipForwardToBox(const Box& b, const double cutoff=0.1) {
              double t = 0.75;
              double delta=0.125;
              Point ep=e, c2p;
              double distance; // distance this iteration
              do {
                std::pair<Point,Point> r = clipForward(t);
                distance = (ep-r.first).mag2();
                ep = r.first;
                c2p = r.second;
                if (b.contains(ep)) {
                  // intersection is closer to start
                  t -= delta;
                  delta *= 0.5;
                } else {
                  // intersection is closer to end
                  t += delta;
                  delta *= 0.5;
                }
              } while (distance > cutoff*cutoff);
              e = ep;
              c2 = c2p;
            }

            /** Clips the last point at the specified interpolation value
              * and adjusts the control point to match
            */
            std::pair<Point,Point> clipReverse(double t) const {
              Point p1(s), p2(c1), p3(c2), p4(e);
              Point q1(p1.interpolate(p2, t)),
                    q2(p2.interpolate(p3, t)),
                    q3(p3.interpolate(p4, t));
              Point r1(q1.interpolate(q2, t)),
                    r2(q2.interpolate(q3, t));
              Point s1(r1.interpolate(r2, t));

              return std::make_pair(s1,r1);
            }

            void clipReverseToBox(const Box& b, const double cutoff=0.1) {
              double t = 0.25;
              double delta=0.125;
              Point sp=s, c1p;
              double distance; // distance this iteration
              do {
                std::pair<Point,Point> r = clipForward(t);
                distance = (sp-r.first).mag2();
                sp = r.first;
                c1p = r.second;
                if (b.contains(sp)) {
                  // intersection is closer to end
                  t += delta;
                  delta *= 0.5;
                } else {
                  // intersection is closer to start
                  t -= delta;
                  delta *= 0.5;
                }
              } while (distance > cutoff*cutoff);
              s = sp;
              c1 = c1p;
            }


            /// Transform
            Affine2d tf_;
            /// Inverse transform
            Affine2d itf_;
        protected:
    };

    inline std::ostream& operator<< (std::ostream& os, const RxnBezier& b) {
        os << b.s << "-" << b.c1 << "-" << b.c2 << "-" << b.e;
        return os;
    }

}

#endif

#endif
