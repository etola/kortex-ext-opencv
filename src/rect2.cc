// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See licence.txt file for licence information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
//
// ---------------------------------------------------------------------------

#include <kortex/rect2.h>

namespace kortex {

    bool Rect2f::is_inside(float x, float y) const {
        if( y>=ly && y<uy && x>=lx && x<ux ) return true;
        else                                 return false;
    }

    bool Rect2f::is_inside_y(float y) const {
        if( y>=ly && y<uy ) return true;
        else                return false;
    }

    bool Rect2f::is_inside_x(float x) const {
        if( x>=lx && x<ux ) return true;
        else                return false;
    }


    bool Rect2f::is_inside(int x, int y) const {
        if( y>=ly && y<uy && x>=lx && x<ux ) return true;
        else                                 return false;
    }

    bool Rect2f::is_inside_y(int y) const {
        if( y>=ly && y<uy ) return true;
        else                return false;
    }

    bool Rect2f::is_inside_x(int x) const {
        if( x>=lx && x<ux ) return true;
        else                return false;
    }
//
//
//


    bool Rect2i::is_inside(int x, int y) const {
        if( x>=lx && x<ux && y>=ly && y<uy ) return true;
        else                                 return false;
    }

    bool Rect2i::is_inside_x(int x) const {
        if( x>=lx && x<ux ) return true;
        else                return false;
    }

    bool Rect2i::is_inside_y(int y) const {
        if( y>=ly && y<uy ) return true;
        else                return false;
    }

}
