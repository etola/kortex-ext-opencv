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

#ifdef WITH_OPENCV

#include <kortex/mem_manager.h>
#include <kortex/minmax.h>
#include <kortex/string.h>
#include <cstring>

#include "kortex/figure.h"
#include "kortex/gui_window.h"

using namespace std;

namespace kortex {

    Figure::Figure() {
        init_();
    }

    Figure::Figure( const string& name ) {
        init_();
        wname = name;
    }

    void Figure::set_name( const string& name ) {
        wname = name;
    }

    void Figure::init_() {
        yticks =  4;
        xticks = 10;

        narr   = 0;

        baxes   = true;
        bgrid   = true;
        blegend = false;

        margin = 40;

        h = 300;
        w = 600;

        line_thickness = 1;
        mark_color = Color(255,0,0);
        text_color = Color(180,180,0);
        axis_color = Color(255,255,255);
        line_color = Color(100,255,100);
        bar_color  = Color(255,0,0);

        title="";
        wname = "figure window";
        wnd     = NULL;
    }

    Figure::~Figure() {
        delete wnd;  wnd  = NULL;
    }

    void Figure::create(const string& name) {
        if( name != "" )
            wname = name;
        wnd = new GUIWindow(wname);
        wnd->init(w,h,3);
        wnd->create();
    }

    void Figure::destroy() {
        if( wnd ) wnd->destroy();
        delete wnd;
        wnd = NULL;
    }

    void Figure::init_mouse() {
        assert_pointer( wnd );
        wnd->init_mouse();
    }
    void Figure::reset_mouse() {
        assert_pointer( wnd );
        wnd->reset_mouse();
    }
    bool Figure::mouse_click( const int& button, int &x, int &y ) const {
        assert_pointer( wnd );
        return wnd->mouse_click( button, x, y );
    }
    bool Figure::mouse_move_event(int &x, int &y ) const {
        assert_pointer( wnd );
        return wnd->mouse_move_event( x, y );
    }

    void Figure::save_screen( const string& file ) const {
        assert_pointer( wnd );
        wnd->save_screen( file );
    }


    void Figure::reset( const int& nw, const int& nh ) {
        wnd->reset_display();
    }

    void Figure::resize( const int& nw, const int& nh ) {
        h = nh;
        w = nw;
        wnd->init(w,h,3);
    }

    void Figure::show() {
        wnd->set_margin(margin);
        if( baxes ) draw_axis();//wnd->draw_axes(range,yticks,xticks);
        if( bgrid ) draw_grid();
        if( title != "" ) {
            wnd->set_color(axis_color);
            wnd->write(margin,margin/2,title);
            wnd->set_color(line_color);
        }
        wnd->show();
    }

    void Figure::refresh() {
        wnd->refresh();
    }

    void Figure::set( const float* arr, const int& n ) {
        farr.resize( n );
        for( int i=0; i<n; i++ )
            farr[i] = arr[i];
        if(range.dy==0) {
            float mn,mx;
            min(farr, mn);
            max(farr, mx);
            range = Rect2f(mn,mx,0,narr);
        }
        narr = farr.size();
    }

    void Figure::set(const int* arr, const int& n) {
        farr.resize( n );
        for(int i=0; i<n; i++)
            farr[i] = arr[i];
        if(range.dy==0) {
            float mn,mx;
            min(farr, mn);
            max(farr, mx);
            range = Rect2f(mn,mx,0,narr);
        }
        narr = farr.size();
    }

    void Figure::set(const uchar* arr, const int& n) {
        farr.resize( n );
        for(int i=0; i<n; i++)
            farr[i] = arr[i];
        if(range.dy==0) {
            float mn,mx;
            min(farr, mn);
            max(farr, mx);
            range = Rect2f(mn,mx,0,narr);
        }
        narr = farr.size();
    }
    void Figure::set(const vector<float>& arr) {
        farr = arr;
        if(range.dy==0) {
            float mn,mx;
            min(farr, mn);
            max(farr, mx);
            range = Rect2f(mn,mx,0,narr);
        }
        narr = farr.size();
    }

    void Figure::plot( const float* arr, int n, int r, int g, int b ) {
        if( r!=-1 ) set_line_color(r,g,b);
        set(arr,n);
        plot();
    }

    void Figure::move( const int& x, const int& y ) {
        wnd->move(x,y);
    }

    void Figure::mark(int n, int r, int g, int b) {
        Color tmp = line_color;
        if( r==-1 ) line_color = mark_color;
        else        line_color = Color(r,g,b);
        scatter(n);
        stem(n);
        line_color = tmp;
    }

    int Figure::wait(const int& n) const {
        return wnd->wait(n);
    }

    void Figure::draw_axis() {
        wnd->set_color( axis_color );
        wnd->set_thickness( line_thickness );
        wnd->draw_line( margin, h-margin,   margin,   margin );
        wnd->draw_line( margin, h-margin, w-margin, h-margin );

        float dw    = (w-2*margin)/(xticks);
        float dh    = (h-2*margin)/(yticks);
        float ystep = range.dy/(yticks);
        float xstep = range.dx/(xticks);
        int   tl    = dh/10;
        float gx, gy;
        // xticks
        for( int i=0; i<=xticks; i++ ) {
            wnd->draw_line( margin+i*dw, h-margin-tl, margin+i*dw,  h-margin+tl );
            xy_to_gui_xy( (i-0.2)/xticks*narr, range.ly, gx, gy );

            // wnd->write( margin+(i-0.2)*dw, h-margin+10, num2str((int)(range.lx+i*xstep)) );
            wnd->write( gx, gy+10, num2str((int)(range.lx+i*xstep)) );
        }
        // yticks
        for( int i=0; i<=yticks; i++ ) {
            wnd->draw_line( margin-tl, h-margin-i*dh, margin+tl, h-margin-i*dh );
            float yval = range.ly+i*ystep;
            string yvalstr;
            if( yval-int(yval) != 0 ) yvalstr = num2str(yval,2);
            else yvalstr = num2str(int(yval));
            int shift=yvalstr.length()*10;
            if( shift > margin-5 ) shift = margin-5;
            wnd->write( margin-tl-shift, h-margin-(i+0.2)*dh, yvalstr );
        }
    }

    void Figure::draw_grid() {
        float dw = (w-2*margin)/(xticks);
        float dh = (h-2*margin)/(yticks);

        wnd->set_color(90,90,90);
        wnd->set_thickness( 1 );

        for( int i=1; i<=yticks; i++ ) { // x-grid
            int x=margin;
            while( x<w-margin ) {
                wnd->draw_line( x, h-margin-i*dh, x+5, h-margin-i*dh );
                x+=10;
            }
        }
        for( int i=1; i<=xticks; i++ ) { // y-grid
            int y=h-margin;
            while( y>margin ) {
                wnd->draw_line( margin+i*dw, y, margin+i*dw, y-5 );
                y-=10;
            }
        }
    }

    void Figure::plot() {
        wnd->set_color    ( line_color     );
        wnd->set_thickness( line_thickness );
        for( int i=0; i<narr-1; i++ ) {
            float gx0, gy0, gx1, gy1;
            xy_to_gui_xy( i,   farr[i  ], gx0, gy0 );
            xy_to_gui_xy( i+1, farr[i+1], gx1, gy1 );
            wnd->draw_line( gx0, gy0, gx1, gy1 );
        }
    }

    void Figure::xy_to_gui_xy( float x, float y, float& gx, float& gy ) const {
        float dw = (w-2*margin)/(float)narr;
        float dh = (h-2*margin)/(float)range.dy;
        gx = margin + x * dw;
        gy = h-margin-(y-range.ly)*dh;
    }

    void Figure::line(int n0, float y0, int n1, float y1) {
        wnd->set_color    ( line_color     );
        wnd->set_thickness( line_thickness );
        float gx0, gy0, gx1, gy1;
        xy_to_gui_xy( n0, y0, gx0, gy0 );
        xy_to_gui_xy( n1, y1, gx1, gy1 );
        wnd->draw_line( gx0, gy0, gx1, gy1 );
    }

    void Figure::stem(int index) {
        float gx0, gy0, gx1, gy1;
        wnd->set_color    ( line_color     );
        wnd->set_thickness( line_thickness );
        if( index<0) {
            for( int n=0; n<narr; n++ ) {
                xy_to_gui_xy( n, farr[n],  gx0, gy0 );
                xy_to_gui_xy( n, range.ly, gx1, gy1 );
                wnd->draw_line( gx0, gy0, gx1, gy1 );
            }
        } else {
            xy_to_gui_xy( index, farr[index], gx0, gy0 );
            xy_to_gui_xy( index, range.ly,    gx1, gy1 );
            wnd->draw_line( gx0, gy0, gx1, gy1 );
        }
    }

    void Figure::text(int index, string text) {
        wnd->set_color( text_color );
        float gx, gy;
        xy_to_gui_xy( index, farr[index], gx, gy );
        wnd->write( gx, gy-20, text );
    }

    void Figure::scatter(int index) {
        wnd->set_color    ( line_color     );
        wnd->set_thickness( line_thickness );
        float gx, gy;
        if( index<0 ) {
            for( int n=0; n<narr; n++ ) {
                xy_to_gui_xy( n, farr[n], gx, gy );
                wnd->draw_circle( gx, gy, 2 );
            }
        } else {
            xy_to_gui_xy( index, farr[index], gx, gy );
            wnd->draw_circle( gx, gy, 2 );
        }
    }

    void Figure::bar(bool filled) {
        float dw = (w-2*margin)/(float)narr;
        float dh = (h-2*margin)/(float)range.dy;
        int thick=line_thickness;
        if( filled ) thick*=-1;
        wnd->set_color    ( bar_color );
        wnd->set_thickness( thick     );
        float gx, gy;
        for( int n=0; n<narr; n++ ) {
            xy_to_gui_xy( n, farr[n], gx, gy );
            wnd->draw_rectangle( gx, gy, dw*0.9, farr[n]*dh );
        }
    }

}

#endif

