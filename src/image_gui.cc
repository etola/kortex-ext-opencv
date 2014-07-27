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
#include "kortex/image_gui.h"
#include <kortex/image.h>
#include <kortex/string.h>
#include <kortex/image_space_mapping.h>

#include <ctime>

namespace kortex {

    void display( const Image* img, int w, bool interactive, double time_out ) {
        ImageGUI g;
        g.setup( img );
        g.create( w );
        if( interactive )
            g.display( time_out );
        else
            g.display_only( time_out );
    }

    ImageGUI::ImageGUI() {
        wzoom = NULL;
        imgp = NULL;
        bhover = true;
        benable_help = false;
        benable_shadow = true;
        gx = 0;
        gy = 0;
        gw = 700;
        gh = 700;
        zsz = 101;
    }

    ImageGUI::~ImageGUI() {
        if( wzoom ) {
            wzoom->destroy();
            delete wzoom;
            wzoom = NULL;
        }
    }

    void ImageGUI::toggle_zoom_window() {
        if( wzoom ) {
            wzoom->destroy();
            delete wzoom;
            wzoom = NULL;
            return;
        }

        wzoom = new GUIWindow();
        wzoom->set_name("zoom");
        wzoom->create_display( zsz, zsz );
        wzoom->create(0);
        wzoom->resize( 3*zsz, 3*zsz );
        wzoom->move( gw, 0 );
        wzoom->show();
    }

    void ImageGUI::update_zoom_window() {
        if( !wzoom ) return;
        wzoom->set_display( &wimg, gx, gy, zsz );
        int rw = zsz/2.5;
        int px = zsz/2 - rw;
        int py = zsz/2 - rw;
        wzoom->draw_rectangle( px, py, 2*rw+1, 2*rw+1 );
    }

    void ImageGUI::create( int window_width ) {
        gw = (window_width) ? window_width : 600;
        // gw = std::min(700, w );
        gh = gw / double( imgp->w() ) * imgp->h();
        wimg.set_name("image window");

        if( imgp->ch() == 1 ) {
            ColorMap cm;
            cm.cm_type = CMT_LINEAR;
            cm.r_type  = RT_PERCENT;
            cm.minv = 5;
            cm.maxv = 90;
            cm.invert = false;
            Image tmp;
            image_map( *imgp, cm, tmp );
            wimg.set_image( &tmp );
        } else {
            wimg.set_image( imgp );
        }
        wimg.create(0);
        wimg.resize(gw,gh);
        wimg.move(0,0);
        wimg.init_mouse();
        wimg.show();
    }

    void ImageGUI::setup( const Image* img ) {
        imgp = img;
    }

    void ImageGUI::reset_mouse() {
        wimg.reset_mouse();
    }

    bool ImageGUI::catch_keyboard() {
        int c = wimg.wait(5);
        if     ( c == 'q' ) return false;
        else if( c == 'b' ) bhover = !bhover;
        else if( c == 'h' ) benable_help = !benable_help;
        else if( c == 'm' ) benable_shadow = !benable_shadow;
        else if( c == 'z' ) toggle_zoom_window();
        return true;
    }

    void ImageGUI::catch_mouse() {
        if( bhover && wimg.mouse_move_event(gx,gy) ) {
            wimg.reset_mouse();
        }
        if( !wimg.mouse_click(1,gx,gy) ) {
            return;
        }

        printf("clicked [%d %d] ", gx, gy);

        if( imgp->ch() == 1 ) {
            float v = imgp->get(gx,gy);
            printf("[val %f]\n", v);
        } else if( imgp->ch() == 3 ) {
            uchar r, g, b;
            imgp->get( gx, gy, r, g, b );
            printf("[val %d %d %d]\n", r, g, b );
        }

        wimg.reset_mouse();
    }

    void ImageGUI::display( double time_out ) {
        wimg.reset_mouse();

        time_t st;
        time_t now;
        time( &st );

        while(1) {
            reset_display();
            if( !catch_keyboard() )
                break;
            catch_mouse();
            draw_mouse_shadow();
            update_zoom_window();
            display_help();
            display_messages();
            refresh();

            if( time_out != 0.0 ) {
                time(&now);
                double elapsed = difftime( now, st );
                if( elapsed > time_out )
                    break;
            }
        }
    }


    void ImageGUI::display_only( double time_out ) {
        wimg.reset_mouse();

        time_t st;
        time_t now;
        time( &st );

        while(1) {
            if( !catch_keyboard() )
                break;
            catch_mouse();
            if( time_out != 0.0 ) {
                time(&now);
                double elapsed = difftime( now, st );
                if( elapsed > time_out )
                    break;
            }
        }
    }


    void ImageGUI::reset_display() {
        wimg.reset_display();
    }


    void ImageGUI::refresh() {
        wimg.refresh();
        if( wzoom ) wzoom->refresh();
    }

    void ImageGUI::display_help() {
        if( !benable_help ) return;
        wimg.set_color( 255, 255, 10 );
        wimg.write( 10,  20, "h: toggle help text" );
        wimg.write( 10,  40, "v: toggle verbosity" );
        wimg.write( 10,  60, "q: quit" );
        wimg.write( 10,  80, "z: toggle zoom window" );
        wimg.write( 10, 100, "m: enable mouse shadow" );
    }

    void ImageGUI::display_messages() {
        uchar cr, cg, cb;
        get_color( COLOR_YELLOW, cr, cg, cb );
        wimg.set_color( cr, cg, cb );
        wimg.write( 10, imgp->h()-20, "("+num2str(gx)+","+num2str(gy)+")" );
    }

    void ImageGUI::draw_mouse_shadow() {
        if( !benable_shadow ) return;

        wimg.set_color( 255, 255, 255 );
        wimg.mark( gx, gy, 0 );
        wimg.set_color( 0, 255, 0 );
        wimg.draw_line( gx, gy+4, gx, gy+17 );
        wimg.draw_line( gx, gy-4, gx, gy-17 );
        wimg.draw_line( gx+4, gy, gx+17, gy );
        wimg.draw_line( gx-4, gy, gx-17, gy );
        wimg.set_color( 255, 0, 0 );
        wimg.draw_line( gx+3, gy-3, gx+15, gy-15 );
        wimg.draw_line( gx+3, gy+3, gx+15, gy+15 );
        wimg.draw_line( gx-3, gy-3, gx-15, gy-15 );
        wimg.draw_line( gx-3, gy+3, gx-15, gy+15 );
        wimg.draw_circle( gx, gy, 15 );
    }

}
