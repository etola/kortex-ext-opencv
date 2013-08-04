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

#ifndef KORTEX_RECT2_H
#define KORTEX_RECT2_H

namespace kortex {

    struct Rect2f {
        float lx, ly, ux, uy;
        float dx, dy;
        int id;

        Rect2f() { lx = ux = ly = uy = dx = dy = 0.0f; id=0; }
        Rect2f(float xmin, float xmax, float ymin, float ymax) {
            lx = xmin;
            ux = xmax;
            ly = ymin;
            uy = ymax;
            id = 0;
            update();
        }

        void crop(float xmin, float xmax, float ymin, float ymax) {
            if( lx<xmin ) lx=xmin;
            if( ly<ymin ) ly=ymin;
            if( ux>xmax ) ux=xmax;
            if( uy>ymax ) uy=ymax;
        }

        void update() { dx=ux-lx; dy=uy-ly; }

        void update(float x, float y) {
            if( x<lx ) lx = x;
            if( x>ux ) ux = x;
            if( y<ly ) ly = y;
            if( y>uy ) uy = y;
            update();
        }

        bool is_inside(float x, float y) const;
        bool is_inside_y(float y) const;
        bool is_inside_x(float x) const;

        bool is_inside(int x, int y) const;
        bool is_inside_y(int y) const;
        bool is_inside_x(int x) const;

    };

    struct Rect2i {
        int lx, ly, ux, uy;
        int dx, dy;
        int id;

        Rect2i() { lx = ux = ly = uy = dx = dy = 0; id = 0; }
        Rect2i(int xmin, int xmax, int ymin, int ymax) {
            lx = xmin;
            ux = xmax;
            ly = ymin;
            uy = ymax;
            id = 0;
            update();
        }

        void crop(int xmin, int xmax, int ymin, int ymax) {
            if( lx<xmin ) lx=xmin;
            if( ly<ymin ) ly=ymin;
            if( ux>xmax ) ux=xmax;
            if( uy>ymax ) uy=ymax;
        }

        void update() { dx=ux-lx; dy=uy-ly; }

        void update(int x, int y) {
            if( x<lx ) lx = x;
            if( x>ux ) ux = x;
            if( y<ly ) ly = y;
            if( y>uy ) uy = y;
            update();
        }

        bool is_inside(int x, int y) const;
        bool is_inside_y(int y) const;
        bool is_inside_x(int x) const;
    };

}

#endif
