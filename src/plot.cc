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

#include "kortex/plot.h"
#include <kortex/minmax.h>
#include <kortex/string.h>
#include <kortex/math.h>

namespace kortex {

    Plot::Plot() {
        gw = gh = 0;
        bgrid = true;
    }
    Plot::~Plot() {
    }

    void Plot::set_params( PlotParams& gparams ) {
        params = gparams;
    }

    void Plot::create( int w, int h ) {
        gw = w;
        gh = h;
        wmain.set_name("plot");
        wmain.create_display(w,h);
        wmain.create(0);
        wmain.resize(w,h);
        wmain.move(0,0);
        wmain.init_mouse();
        wmain.show();
    }

    void Plot::display() {
        shift_x( xvals[0] );
        shift_y( yvals[0] );
        wmain.reset_mouse();
        redraw();
        while( 1 ) {
            if( !catch_keyboard() )
                break;
            redraw();
            catch_mouse();
            refresh();
        }
    }

    void Plot::redraw() {
        reset_display();
        draw_grid();
        draw_data();
        refresh();
    }

    void Plot::refresh() {
        wmain.refresh();
    }

    void Plot::reset_display() {
        wmain.reset_display();
    }

    void Plot::reset_mouse() {
        wmain.reset_mouse();
    }

    void Plot::draw_mouse_shadow() {
        mx-=5;
        my-=5;
        wmain.set_color( 255, 0, 0 );

        wmain.draw_line( mx, my+1, mx, my+31 );
        wmain.draw_line( mx, my-1, mx, my-31 );
        wmain.draw_line( mx+1, my, mx+31, my );
        wmain.draw_line( mx-1, my, mx-31, my );
        wmain.set_color( 255, 0, 0 );
        float rx, ry;
        gxy_to_rxy( mx+5, my+5, rx, ry );

        float gx, gy;
        int pid = find_closest( rx, ry );
        rxy_to_gxy( xvals[pid], yvals[pid], gx, gy );
        wmain.mark( gx, gy, 4 );

        float cx = xvals[pid];
        float cy = yvals[pid];
        wmain.write( 20, gh-20, "Mouse ("+num2str(rx,3)+","+num2str(ry,3)+") -> [ Closest Point "+num2str(pid)+ " ]" );

        float gx0, gy0;
        rxy_to_gxy( cx, 0, gx0, gy0 );
        wmain.draw_line( gx, gy, gx0, gy0 );
        wmain.write( gx0+3, gy0-12, num2str(cx,5) );

        rxy_to_gxy( 0, cy, gx0, gy0 );
        wmain.draw_line( gx, gy, gx0, gy0 );
        wmain.write( gx0+3, gy0-12, num2str(cy,5) );

    }

    bool Plot::catch_keyboard() {
        int c = wmain.wait(40);
        if     ( c == 'q' ) return false;
        else if( c == 'g' ) {
            bgrid = !bgrid;
            redraw();
        } else if( c == '0' ) {
            params.zoom_factor = 1.0;
            center_coordinate( 0, 0 );
            redraw();
        } else if( c == 65361 ) { // right
            shift_x( -x_range()/10 );
            redraw();
        } else if( c == 65363 ) { // left
            shift_x(  x_range()/10 );
            redraw();
        } else if( c == 65362 ) { // up
            shift_y( y_range()/10 );
            redraw();
        } else if( c == 65364 ) { // down
            shift_y( -y_range()/10 );
            redraw();
        } else if( c == '-' ) {
            zoom( 1.0/0.8 );
            redraw();
        } else if( c == '=' ) {
            zoom( +0.8 );
            redraw();
        }
        return true;
    }

    void Plot::shift_x( float xs ) {
        params.x_shift += xs;
    }

    void Plot::shift_y( float ys ) {
        params.y_shift += ys;
    }

    void Plot::zoom( float zf ) {
        float zx, zy, rx, ry;

        gxy_to_rxy(mx, my, rx, ry);
        int pid = find_closest(rx,ry);
        zx = xvals[pid];
        zy = yvals[pid];
        params.zoom_factor *= zf;
        if( params.zoom_factor <= 1.0/128.0 ) {
            params.zoom_factor = 1.0/128.0;
        }
        center_coordinate( zx, zy );
    }

    void Plot::gxy_to_cxy( float gx, float gy, float& cx, float& cy ) const {
        cx = gx - params.x_margin;
        cy = gh - gy - params.y_margin;
    }
    void Plot::cxy_to_gxy( float cx, float cy, float& gx, float& gy ) const {
        gx = cx + params.x_margin;
        gy = gh - params.y_margin - cy;
    }

    void Plot::gxy_to_rxy( float gx, float gy, float& rx, float& ry ) const {
        float cx, cy;
        gxy_to_cxy( gx, gy, cx, cy );
        float sx = (params.x_max-params.x_min) / (gw-2*params.x_margin) * params.zoom_factor;
        float sy = (params.y_max-params.y_min) / (gh-2*params.y_margin) * params.zoom_factor;
        rx = sx * cx + params.x_shift;
        ry = sy * cy + params.y_shift;
    }

    void Plot::rxy_to_cxy( float rx, float ry, float& cx, float& cy ) const {
        float sx = (params.x_max-params.x_min) / (gw-2*params.x_margin) * params.zoom_factor;
        float sy = (params.y_max-params.y_min) / (gh-2*params.y_margin) * params.zoom_factor;
        cx = (rx - params.x_shift)/sx;
        cy = (ry - params.y_shift)/sy;
    }
    void Plot::cxy_to_rxy( float cx, float cy, float& rx, float& ry ) const {
        float sx = (params.x_max-params.x_min) / (gw-2*params.x_margin) * params.zoom_factor;
        float sy = (params.y_max-params.y_min) / (gh-2*params.y_margin) * params.zoom_factor;
        rx = cx * sx + params.x_shift;
        ry = cy * sy + params.y_shift;
    }

    bool Plot::is_visible( float gx, float gy ) const {
        if( gx < params.x_margin || gx >= gw-params.x_margin ) return false;
        if( gy < params.y_margin || gy >= gh-params.y_margin ) return false;
        return true;
    }

    bool Plot::is_visible_x( float gx ) const {
        if( gx < params.x_margin || gx >= gw-params.x_margin ) return false;
        return true;
    }
    bool Plot::is_visible_y( float gy ) const {
        if( gy < params.y_margin || gy >= gh-params.y_margin ) return false;
        return true;
    }


    void Plot::rxy_to_gxy( float rx, float ry, float& gx, float& gy ) const {
        float sx = (params.x_max-params.x_min) / (gw-2*params.x_margin) * params.zoom_factor;
        float sy = (params.y_max-params.y_min) / (gh-2*params.y_margin) * params.zoom_factor;
        cxy_to_gxy( (rx - params.x_shift)/sx, (ry-params.y_shift)/sy, gx, gy );
    }

    void Plot::find_shift_to_center( float tx, float ty, float& xs, float& ys ) const {
        float qx, qy;
        gxy_to_rxy( gw/2.0, gh/2.0, qx, qy );
        xs = -tx + qx;
        ys = -ty + qy;
    }

    void Plot::center_coordinate( float tx, float ty ) {
        float xs, ys;
        find_shift_to_center( tx, ty, xs, ys );
        params.x_shift -= xs;
        params.y_shift -= ys;
    }

    void Plot::set( const vector<float>& xs, const vector<float>& ys ) {
        yvals = ys;
        xvals = xs;
        kortex::min( xvals, params.x_min );
        kortex::max( xvals, params.x_max );
        kortex::min( yvals, params.y_min );
        kortex::max( yvals, params.y_max );
        // printf( "xr %f %f\n", params.x_min, params.x_max );
        // printf( "yr %f %f\n", params.y_min, params.y_max );
    }

    void Plot::set( const vector<float>& ys ) {
        yvals = ys;
        xvals.resize( yvals.size() );
        for( int i=0; i<(int)yvals.size(); i++ )
            xvals[i] = i;
        kortex::min( xvals, params.x_min );
        kortex::max( xvals, params.x_max );
        kortex::min( yvals, params.y_min );
        kortex::max( yvals, params.y_max );
        // printf( "xr %f %f\n", params.x_min, params.x_max );
        // printf( "yr %f %f\n", params.y_min, params.y_max );
    }

    void Plot::catch_mouse() {
        if( wmain.mouse_move_event(mx,my) ) {
            draw_mouse_shadow();
            refresh();
            return;
        }
        float cx, cy, rx, ry;
        if( wmain.mouse_click(2, mx, my) ) { // right button
            mx-=5;
            my-=5;
            gxy_to_cxy( mx, my, cx, cy );
            gxy_to_rxy( mx, my, rx, ry );
            int pid = find_closest( rx, ry );
            center_coordinate( xvals[pid], yvals[pid] );
            redraw();
        } else if( wmain.mouse_click(1, mx, my) ) { // left button
            mx-=5;
            my-=5;
            gxy_to_cxy( mx, my, cx, cy );
            gxy_to_rxy( mx, my, rx, ry );
            redraw();
            wmain.set_color(255,0,0);
            int psz = int(2.0/params.zoom_factor+0.5);
            if( psz <  2 ) psz = 2;
            if( psz > 10 ) psz = 10;
            wmain.mark(mx, my, psz);
            refresh();
            printf( "clicked coordinate : [xy %d %d] -> [rx %4.2f %4.2f]\n", mx, my, rx, ry );
        }
        wmain.reset_mouse();
    }

    void Plot::draw_grid() {
        Color lc( 100, 100, 150 );
        Color tc( 250, 0, 0 );

        wmain.set_color( lc.r, lc.g, lc.b );
        wmain.set_thickness( 1 );

        float gx0, gy0, gx1, gy1, gx2, gy2, gx3, gy3;


        cxy_to_gxy( 0, 0, gx0, gy0 );
        cxy_to_gxy( gw-2*params.x_margin, 0, gx1, gy1 );
        wmain.draw_line( gx0, gy0, gx1, gy1 );

        cxy_to_gxy( gw-2*params.x_margin, gh-2*params.y_margin, gx2, gy2 );
        wmain.draw_line( gx1, gy1, gx2, gy2 );

        cxy_to_gxy( 0, gh-2*params.y_margin, gx3, gy3 );
        wmain.draw_line( gx0, gy0, gx3, gy3 );
        wmain.draw_line( gx2, gy2, gx3, gy3 );

        // gxy_to_rxy( gw/2, gh/2, midx, midy );
        wmain.mark( gw/2, gh/2, 0 );

        int n_ticks = 10;
        float xj = (gw-2*params.x_margin)/n_ticks;
        float yj = (gh-2*params.y_margin)/n_ticks;
        if( xj*n_ticks > gw-2*params.x_margin )
            xj *= 0.9999999f;
        if( yj*n_ticks > gh-2*params.y_margin )
            yj *= 0.9999999f;


        for( float x=0; x<=gw-2*params.x_margin; x+=xj ) {
            float jp = (gh-2*params.y_margin)/(4*n_ticks);
            float y = 0;
            while( y < gh-2*params.y_margin ) {
                cxy_to_gxy( x, y, gx0, gy0 );
                wmain.mark( gx0, gy0, 0 );
                y += jp;
            }
            cxy_to_gxy( x,  0, gx0, gy0 );
            cxy_to_gxy( x, -10, gx1, gy1 );
            wmain.draw_line(gx0, gy0, gx1, gy1-2);
            float rx, ry;
            gxy_to_rxy(gx0, gy0, rx, ry );
            wmain.write( gx1-15, gy1, num2str(rx,2) );
        }

        for( float y=0; y<=gh-2*params.y_margin; y+=yj ) {
            float jp = (gw-2*params.x_margin)/(4*n_ticks);
            float x = 0;
            while( x < gw-2*params.x_margin ) {
                cxy_to_gxy( x,       y, gx0, gy0 );
                wmain.mark( gx0, gy0, 0 );
                x += jp;
            }

            cxy_to_gxy(-10, y, gx0, gy0 );
            cxy_to_gxy(  0, y, gx1, gy1 );
            wmain.draw_line(gx0, gy0, gx1, gy1);
            float rx, ry;
            gxy_to_rxy(gx1, gy1, rx, ry );
            if( ry>0 )
                wmain.write( 30, gy1-5, num2str(ry,2) );
            else
                wmain.write( 17, gy1-5, num2str(ry,2) );
        }

        rxy_to_gxy( 0, 0, gx0, gy0 );
        wmain.set_color( 255, 0, 0 );
        wmain.mark( gx0, gy0, 5 );
        wmain.set_color( 125, 125, 110 );
        if( is_visible_x( gx0 ) )
            wmain.draw_line( gx0, params.y_margin, gx0, gh-params.y_margin );
        if( is_visible_y( gy0 ) )
            wmain.draw_line( params.x_margin, gy0, gw-params.x_margin, gy0 );

    }

    void Plot::draw_points() {
        int psz = int(2.0/params.zoom_factor+0.5);
        wmain.set_color( 10, 255, 0 );
        if( psz <  2 ) psz = 2;
        if( psz > 10 ) psz = 10;
        for( int i=0; i<(int)yvals.size(); i++ ) {
            float x = xvals[i];
            float y = yvals[i];
            float gx, gy;
            rxy_to_gxy( x, y, gx, gy );
            // printf(" %f %f -> %f %f \n", x, y, gx, gy );
            wmain.mark( gx, gy, psz );
        }
    }

    void Plot::draw_line() {
        // printf("line\n");
        wmain.set_color( 255, 255, 0 );
        float gx0, gy0, gx1, gy1;
        for( int i=0; i<(int)yvals.size()-1; i++ ) {
            rxy_to_gxy( xvals[i], yvals[i], gx0, gy0 );
            rxy_to_gxy( xvals[i+1], yvals[i+1], gx1, gy1 );
            wmain.draw_line( gx0, gy0, gx1, gy1 );
        }
    }

    void Plot::draw_data() {
        // printf( "type %d\n", params.ptype );
        if( params.ptype & PT_DOT  ) draw_points();
        if( params.ptype & PT_LINE ) draw_line();
    }

    float Plot::y_range() const {
        float gx0, gy0, gx1, gy1;
        cxy_to_rxy( 0, 0, gx0, gy0 );
        cxy_to_rxy( 0, gh-2*params.y_margin, gx1, gy1 );
        return gy1-gy0;
    }

    float Plot::x_range() const {
        float gx0, gy0, gx1, gy1;
        cxy_to_rxy( 0, 0, gx0, gy0 );
        cxy_to_rxy( gw-2*params.x_margin, 0, gx1, gy1 );
        return gx1-gx0;
    }

    int Plot::find_closest( float rx, float ry ) const {
        float dist = 1e20;
        int id = 0;

        float tx, ty, qx, qy;
        rxy_to_gxy(rx, ry, tx, ty );

        for( int i=0; i<(int)yvals.size(); i++ ) {
            rxy_to_gxy( xvals[i], yvals[i], qx, qy );
            float d = sq( qy-ty ) + sq( qx-tx );
            if( d < dist ) {
                id = i;
                dist = d;
            }
        }

        return id;
    }

//
//
//

    void plot( vector<float>& xs, vector<float>& ys, int type ) {
        Plot pt;
        pt.create( 1024, 512 );
        pt.set( xs, ys );
        pt.set_type( type );
        pt.display();
    }

    void plot( vector<float>& ys, int type ) {
        Plot pt;
        pt.create( 1024, 512 );
        pt.set( ys );
        pt.set_type( type );
        pt.display();
    }



}
