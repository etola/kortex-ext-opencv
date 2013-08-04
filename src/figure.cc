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

#include <kortex/figure.h>
#include <kortex/gui_window.h>
#include <kortex/mem_manager.h>
#include <kortex/minmax.h>
#include <kortex/string.h>

#include <cstring>

using namespace std;

namespace kortex {

    figure::figure() {
        init_();
    }

    figure::figure( const string& name ) {
        init_();
        wname = name;
    }

    void figure::set_name( const string& name ) {
        wname = name;
    }

    void figure::init_() {
        yticks =  4;
        xticks = 10;

        farr   = NULL;
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

    figure::~figure() {
        deallocate(farr);
        delete wnd;  wnd  = NULL;
    }

    void figure::create(const string& name) {
        if( name != "" )
            wname = name;
        wnd = new gui_window(wname);
        wnd->init(w,h,3);
        wnd->create();
    }

    void figure::destroy() {
        if( wnd ) wnd->destroy();
        delete wnd;
        wnd = NULL;
    }

    void figure::reset( const int& nw, const int& nh ) {
        wnd->reset_display();
    }

    void figure::resize( const int& nw, const int& nh ) {
        h = nh;
        w = nw;
        wnd->init(w,h,3);
    }

    void figure::show() {
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

    void figure::refresh() {
        wnd->refresh();
    }

    void figure::set( const float* arr, const int& n ) {
        if( narr != n ) {
            deallocate(farr);
            allocate(farr,n);
        }
        memcpy(farr, arr, sizeof(*arr)*n );
        narr = n;
        if(range.dy==0) {
            float mn,mx;
            min(farr, narr, mn);
            max(farr, narr, mx);
            range = Rect2f(mn,mx,0,narr);
        }
    }

    void figure::set(const int* arr, const int& n) {
        if( narr != n ) {
            deallocate(farr);
            allocate(farr,n);
        }
        for(int i=0;i<n; i++)
            farr[i]=arr[i];
        narr = n;
    }

    void figure::set(const uchar* arr, const int& n) {
        if( narr != n ) {
            deallocate(farr);
            allocate(farr,n);
        }
        for(int i=0;i<n; i++)
            farr[i]=arr[i];
        narr = n;
    }


    void figure::plot( const float* arr, int n, int r, int g, int b ) {
        if( r!=-1 ) set_line_color(r,g,b);
        set(arr,n);
        plot();
    }

    void figure::move( const int& x, const int& y ) {
        wnd->move(x,y);
    }

    void figure::mark(int n, int r, int g, int b) {
        if( !farr ) return;
        Color tmp = line_color;
        if( r==-1 ) line_color = mark_color;
        else        line_color = Color(r,g,b);
        scatter(n);
        stem(n);
        line_color = tmp;
    }

    int figure::wait(const int& n) const {
        return wnd->wait(n);
    }

    void figure::draw_axis() {
        wnd->set_color( axis_color );
        wnd->set_thickness( line_thickness );
        wnd->draw_line( margin, h-margin,   margin,   margin );
        wnd->draw_line( margin, h-margin, w-margin, h-margin );

        float dw    = (w-2*margin)/(xticks);
        float dh    = (h-2*margin)/(yticks);
        float ystep = range.dy/(yticks);
        float xstep = range.dx/(xticks);
        int   tl    = dh/10;
        // xticks
        for( int i=0; i<=xticks; i++ ) {
            wnd->draw_line( margin+i*dw, h-margin-tl, margin+i*dw,  h-margin+tl );
            wnd->write( margin+(i-0.2)*dw, h-margin+10, num2str((int)(range.lx+i*xstep)) );
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

    void figure::draw_grid() {
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

    void figure::plot() {
        if( !farr ) return;
        float dw = (w-2*margin)/(float)narr;
        float dh = (h-2*margin)/(float)range.dy;
        wnd->set_color    ( line_color     );
        wnd->set_thickness( line_thickness );
        for( int n=0; n<narr-1; n++ ) {
            wnd->draw_line( margin+n*dw, h-margin-(farr[n]-range.ly)*dh, margin+n*dw+dw,  h-margin-(farr[n+1]-range.ly)*dh );
        }
    }

    void figure::line(float y0, int n0, float y1, int n1) {
        if( !farr ) return;
        float dw = (w-2*margin)/(float)narr;
        float dh = (h-2*margin)/(float)range.dy;
        wnd->set_color    ( line_color     );
        wnd->set_thickness( line_thickness );
        wnd->draw_line( margin+n0*dw, h-margin-(y0-range.ly)*dh, margin+n1*dw, h-margin-(y1-range.ly)*dh );
    }

    void figure::stem(int index) {
        if( !farr ) return;
        float dw = (w-2*margin)/(float)narr;
        float dh = (h-2*margin)/(float)range.dy;

        wnd->set_color    ( line_color     );
        wnd->set_thickness( line_thickness );
        if( index<0) {
            for( int n=0; n<narr; n++ )
                wnd->draw_line( margin+n*dw, h-margin-(farr[n]-range.ly)*dh, margin+n*dw, h-margin );
        } else {
            wnd->draw_line( margin+index*dw, h-margin-(farr[index]-range.ly)*dh, margin+index*dw, h-margin );
        }
    }

    void figure::text(int index, string text) {
        if( !farr ) return;
        float dw = (w-2*margin)/(float)narr;
        float dh = (h-2*margin)/(float)range.dy;
        wnd->set_color( text_color );
        wnd->write( margin+index*dw, h-margin-(farr[index]-range.ly)*dh-20, text );
    }

    void figure::scatter(int index) {
        if( !farr ) return;
        float dw = (w-2*margin)/(float)narr;
        float dh = (h-2*margin)/(float)range.dy;
        wnd->set_color    ( line_color     );
        wnd->set_thickness( line_thickness );
        if( index<0 ) {
            for( int n=0; n<narr; n++ )
                wnd->draw_circle( margin+n*dw, h-margin-(farr[n]-range.ly)*dh, 2 );
        } else {
            wnd->draw_circle( margin+index*dw, h-margin-(farr[index]-range.ly)*dh, 2 );
        }
    }

    void figure::bar(bool filled) {
        if( !farr ) return;
        float dw = (w-2*margin)/(float)narr;
        float dh = (h-2*margin)/(float)range.dy;

        int thick=line_thickness;
        if( filled ) thick*=-1;
        wnd->set_color    ( bar_color );
        wnd->set_thickness( thick     );
        for( int n=0; n<narr; n++ ) {
            wnd->draw_rectangle( margin+n*dw, h-margin-(farr[n]-range.ly)*dh, dw*0.9, farr[n]*dh );
        }
    }
}

#endif

