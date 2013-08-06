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
#ifndef KORTEX_IMAGE_GUI_H
#define KORTEX_IMAGE_GUI_H

#include <kortex/gui_window.h>

namespace kortex {

    class Image;
    void display( const Image* img );

    class ImageGUI {
    public:
        ImageGUI();
        void setup( const Image* img );
        void create();
        void display();
    private:
        gui_window  wimg;
        gui_window* wzoom;
        bool        bhover;
        bool        benable_help;
        bool        benable_shadow;
        int         gx, gy, gw, gh;
        int         zsz;
        const Image* imgp;

        void reset_display();
        void refresh();
        void display_help();
        void display_messages();
        void draw_mouse_shadow();

        void toggle_zoom_window();
        void update_zoom_window();

        void reset_mouse();
        void catch_mouse();
        void catch_keyboard();
    };


}

#endif
