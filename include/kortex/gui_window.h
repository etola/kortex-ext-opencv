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

#ifndef KORTEX_GUI_WINDOW_H
#define KORTEX_GUI_WINDOW_H

#include <kortex/color.h>
#include <string>

#define MOUSE_LCLICK 1
#define MOUSE_RCLICK 2

using std::string;

struct _IplImage;
typedef struct _IplImage IplImage;
struct CvFont;

namespace kortex {

    class Image;

    struct callback_info {
        int xstart;
        int ystart;
        int xend;
        int yend;
        int event;
        int x;
        int y;
    };
    extern callback_info g_mouse_callback;
    void mouse_callback(int event, int x, int y, int flags, void* param);

    class gui_window {
    public:
        gui_window();
        gui_window( const string& name);
        ~gui_window();

        void create( const int& fixed=1 );
        void destroy();
        void set_name( const string& name );

        void init( const int& w, const int& h, const int& nc );

        void set_image( const Image *im );
        void set_image( const uchar *im, const int& w, const int& h, const int& nc );
        void set_image( const string& imname );

        void      create_display( const int& w, const int& h );

        const IplImage* get_display() const;
        const IplImage* get_original_display() const;

        void  set_display( const gui_window* src_wnd, const int& x, const int& y, const int& wsz );
        void  set_original_display( const gui_window* src_wnd, const int& x, const int& y, const int& wsz );

        void set_margin( const int& m ) { margin=m; }

        void save_screen( const string& file ) const;

        // window paint operations
        void draw_line     ( int x0, int y0, int x1, int y1 );
        void draw_ray      ( int x0, int y0, float length, float angle );
        void draw_rectangle( int x, int y, int dw, int dh );
        void draw_circle   ( int x, int y, int dr );
        void draw_polygon  ( int* xy, int no_points );
        void zoom_to_point ( const int& x, const int& y, const int& wsz );

        void mark( int x, int y, int thickness=-1 );
        void mark_region( int* mark, bool permanent );

        void write( int x, int y, const string& text );
        void write( int x, int y, float  num );
        void write( int x, int y, double num );
        void write( int x, int y, int    num );
        //

        void reset_display();
        void reset();

        void init_mouse();
        void reset_mouse();
        int wait( const int& ms ) const;

        bool mouse_click( const int& button, int &x, int &y ) const;
        bool mouse_move_event( int &x, int &y ) const;

        void show();
        void refresh();
        void resize( const int& nw, const int& nh );
        void move  ( const int& x,  const int& y  );

        // brush settings
        void set_thickness( const int& t );
        void set_color( const uchar& r, const uchar& g, const uchar& b );
        void set_color( const Color& col );

        void set_font( float hs=1, float vs=1, float shear=0, float thickness=1 );
        //
        int  get_thickness() const { return dp_thickness; }

    private:

        void init_();

        IplImage* display;
        IplImage* original_display;

        string wname;

        int margin;

        int dh; // window size
        int dw;
        int py; // position of the window
        int px;

        int      dp_thickness; // brush settings
        Color    dp_color;
        CvFont*  dp_font;
    };
}
#endif // KORTEX_GUI_WINDOW_H

#endif // if KORTEX_WITH_OPENCV
