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

/** @file box.h
 * @brief A box
  */

//== BEGINNING OF CODE ===============================================================

#ifndef __SBNW_LAYOUT_BOX_H_
#define __SBNW_LAYOUT_BOX_H_

//== INCLUDES ========================================================================

#include "graphfab/core/SagittariusCore.h"
#include "graphfab/math/min_max.h"

//-- C++ code --
#ifdef __cplusplus

#include "point.h"

// #include <string>

#include <iostream>

namespace Graphfab {

    class Box {
        public:
            Box()
                {}

            /// Construct from upper-left & lower-right extents resp.
            Box(const Point& min, const Point& max)
                : _min(min), _max(max) { AT(_min.x <= _max.x && _min.y <= _max.y, "Min/max mismatch"); }

            /// Construct from upper-left & lower-right extents directly
            Box(double x1, double y1, double x2, double y2)
                : _min(x1,y1), _max(x2,y2) {
                    if(!(_min.x <= _max.x && _min.y <= _max.y)) {
                        dump(std::cerr);
                        std::cerr << "\n";
                    }
                    AT(_min.x <= _max.x && _min.y <= _max.y, "Min/max mismatch");
                }

            /// Upper-left
            const Point& getMin() const { return _min; }
            double getMinX() const { return getMin().x; }
            double getMinY() const { return getMin().y; }

            void setMin(const Point& p) { _min = p; }

            void setMinX(const double x) { _min.x = x; }
            void setMinY(const double y) { _min.y = y; }

            /// Lower-right
            const Point& getMax() const { return _max; }
            double getMaxX() const { return getMax().x; }
            double getMaxY() const { return getMax().y; }

            void setMax(const Point& p) { _max = p; }

            void setMaxX(const double x) { _max.x = x; }
            void setMaxY(const double y) { _max.y = y; }

            Point getCenter() const { return (getMax() + getMin())/2.; }

            Point getFirstQuadCorner()  const { return getMax(); }
            Point getSecondQuadCorner() const { return Point(_min.x, _max.y); }
            Point getThirdQuadCorner()  const { return getMin(); }
            Point getFourthQuadCorner() const { return Point(_max.x, _min.y); }

            /// Min to max
            Point getDiag() const { return getMax() - getMin(); }

            /// Return the maximum of {width, height}
            double maxDim() const {
                double w = _max.x - _min.x;
                double h = _max.y - _min.y;
                return max(w,h);
            }

            /// Return the minimum of {width, height}
            double minDim() const {
                double w = _max.x - _min.x;
                double h = _max.y - _min.y;
                return min(w,h);
            }

            Point getTopRightCorner() const { return Point(_max.x, _min.y); }

            Point getBottomLeftCorner() const { return Point(_min.x, _max.y); }

            /// Get the width
            double width() const { return _max.x - _min.x; }

            /// Alters the extents such that the width is set to spec. value; method undefined
            void setWidth(const double w) { _max.x = _min.x+w; }

            /// Get the height
            double height() const { return _max.y - _min.y; }

            /// Alters the extents such that the height is set to spec. value; method undefined
            void setHeight(const double w) { _max.y = _min.y+w; }

            /// Get the area
            double area() const {
                double w = _max.x - _min.x;
                double h = _max.y - _min.y;
                return w*h;
            }

            /// Determine if box can be shrunk by specified amt
            bool canShrink(const double v) const {
                if(_min.x + 2*v <= _max.x && _min.y+2*v <= _max.y)
                    return true;
                else
                    return false;
            }

            /// Shrink all sides by specified amount (safe)
            Box shrink(const double v) const {
                return Box(_min+Point(v,v), _max-Point(v,v));
            }

            /// Shrink all sides by specified amount (not safe)
            void shrink_(const double v) {
                _min.x += v;
                _min.y += v;
                _max.x -= v;
                _max.y -= v;
            }

            /// Pad all sides by specified amount (safe)
            Box padded(const double v) const {
                return Box(_min-Point(v,v), _max+Point(v,v));
            }

            // expand! to contain the other box and this box
            void expandx(const Box& other) {
                _min = Point::emin(_min, other._min);
                _max = Point::emax(_max, other._max);
            }

            void displace(const Point& d) {
                _min += d;
                _max += d;
            }

            void dump(std::ostream& o) const {
                o << "[" << getMin() << ", " << getMax() << "]";
            }

            /// Return true if the given point is inside the box
            bool contains(const Point& p) const {
              return (p-getMin()).inFirstQuadrant() && (getMax()-p).inFirstQuadrant();
            }

        protected:
            /// Extents
            Point _min, _max;
    };

    /// Dump to stream
    std::ostream& operator<< (std::ostream& os, const Box& b);

    /// Box/line segement intersection
    std::pair<bool, Point> intersectBoxLine(const Box& b, const Point& u, const Point& v);

}

#endif

#endif
