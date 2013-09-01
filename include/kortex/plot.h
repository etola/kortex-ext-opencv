// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_PLOT_H
#define KORTEX_PLOT_H

#include <vector>
#include "kortex/gui_window.h"

using std::vector;

namespace kortex {

    void plot( vector<float>& xs, vector<float>& ys );
    void plot( vector<float>& ys );

    enum PlotTypes { PT_DOT=1, PT_LINE=2, PT_STEM=4 };

    struct PlotParams {
        float x_shift;
        float y_shift;
        float zoom_factor;
        float g_steps;
        float x_margin;
        float y_margin;

        float x_min;
        float x_max;
        float y_min;
        float y_max;
        float x_scale;
        float y_scale;

        int  ptype;

        PlotParams() {
            x_shift     = 0.0;
            y_shift     = 0.0;
            zoom_factor = 1.0;
            g_steps     = 1.0;
            x_margin    = 80;
            y_margin    = 50;

            x_min = 0.0;
            x_max = 1.0;
            y_min = 0.0;
            y_max = 1.0;
            x_scale = 1.0;
            y_scale = 1.0;
            ptype = PT_DOT | PT_LINE;
        }
    };

    class Plot {
    public:
        Plot();
        ~Plot();
        void set_params( PlotParams& gparams );
        void create( int w, int h );
        void display();
        void shift_x( float xs );
        void shift_y( float ys );
        void zoom   ( float zf );
        void center_coordinate( float tx, float ty );

        void set( const vector<float>& ys );
        void set( const vector<float>& xs, const vector<float>& ys );
        void set_type( int pt ) { params.ptype = pt; }

    private:
        PlotParams params;
        GUIWindow  wmain;
        int  gw, gh;
        bool bgrid;
        int  mx, my;

        void refresh();
        void reset_display();
        void reset_mouse();
        void catch_mouse();
        bool catch_keyboard();

        void draw_mouse_shadow();
        void draw_grid();
        void draw_points();
        void draw_line();
        void draw_data();

        void redraw();

        void find_shift_to_center( float tx, float ty, float& xs, float& ys ) const;

        void gxy_to_rxy( float gx, float gy, float& rx, float& ry ) const;
        void rxy_to_gxy( float rx, float ry, float& gx, float& gy ) const;
        void cxy_to_gxy( float cx, float cy, float& gx, float& gy ) const;
        void gxy_to_cxy( float gx, float gy, float& cx, float& cy ) const;

        void rxy_to_cxy( float rx, float ry, float& cx, float& cy ) const;
        void cxy_to_rxy( float cx, float cy, float& rx, float& ry ) const;

        bool is_visible( float gx, float gy ) const;
        bool is_visible_x( float gx ) const;
        bool is_visible_y( float gy ) const;

        float x_range() const;
        float y_range() const;

        int find_closest( float rx, float ry ) const;

        vector<float> xvals;
        vector<float> yvals;
    };

}

#endif
