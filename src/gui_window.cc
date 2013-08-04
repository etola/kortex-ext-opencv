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

#include <kortex/gui_window.h>
#include <kortex/opencv_extensions.h>
#include <kortex/image.h>
#include <kortex/string.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

namespace kortex {
    callback_info g_mouse_callback;
    void mouse_callback(int event, int x, int y, int flags, void* param) {
        if(event == CV_EVENT_LBUTTONDOWN || event == CV_EVENT_RBUTTONDOWN || event == CV_EVENT_MOUSEMOVE) {
            if(x > g_mouse_callback.xstart && y > g_mouse_callback.ystart &&
               x < g_mouse_callback.xend && y < g_mouse_callback.yend) {
                g_mouse_callback.x = x;
                g_mouse_callback.y = y;
                g_mouse_callback.event = event;
            }
        }
    }

    void gui_window::init_() {
        display          = NULL;
        original_display = NULL;
        dp_font          = NULL;
        dh = dw = 0;
        py = px = 0;
        // cvInitFont (&dp_font, CV_FONT_VECTOR0, 0.4, 0.4, 0.0f, 1, CV_AA);
        dp_font = new CvFont;
        cvInitFont( dp_font, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1, CV_AA );
        dp_color = Color(255,0,0);
        dp_thickness = 1;
        margin = 50;
    }

    gui_window::gui_window() {
        wname = "float window";
        init_();
    }
    gui_window::gui_window(const string& name) {
        wname = name;
        init_();
    }

    void gui_window::set_name(const string& name) {
        wname=name;
    }

    void gui_window::create(const int& fixed) {
        cvNamedWindow(wname.c_str(), fixed);
    }

    void gui_window::destroy() {
        cvDestroyWindow(wname.c_str());
    }

    gui_window::~gui_window() {
        reset();
    }

    void gui_window::init(const int& w, const int& h, const int& nc) {
        if( original_display ) cvReleaseImage( &original_display );
        dh=h;
        dw=w;
        original_display = cvCreateImage(cvSize(dw,dh), IPL_DEPTH_8U, 3);
        memset(original_display->imageData, 0, sizeof(uchar)*(original_display->widthStep*h));
        reset_display();
    }

    void gui_window::create_display(const int& w, const int& h) {
        if( original_display ) cvReleaseImage( &original_display );
        dh = h;
        dw = w;
        original_display = cvCreateImage( cvSize(dw,dh), IPL_DEPTH_8U, 3);
        cvSet(original_display, cvScalar(0,0,0));
        reset_display();
    }

    void gui_window::set_image( const string& imname ) {
        if( original_display ) cvReleaseImage( &original_display );
        Image tmp;
        tmp.load(imname.c_str());

        if     ( tmp.ch() == 3 ) { tmp.convert( IT_U_PRGB ); }
        else if( tmp.ch() == 1 ) { tmp.convert( IT_U_GRAY ); }
        set_image( tmp.get_row_u(0) , tmp.w(), tmp.h(), tmp.ch() );
    }

    void gui_window::set_image( const uchar* im, const int& w, const int& h, const int& nc ) {
        if( original_display ) cvReleaseImage( &original_display );
        dh = h;
        dw = w;
        original_display = cvCreateImageHeader( cvSize(dw,dh), IPL_DEPTH_8U, 3);
        cvCreateData(original_display);
        copy_image_to_color_ipl(im, dw, dh, nc, original_display);
        reset_display();
    }
    void gui_window::set_image( const Image *im ) {
        im->passert_type( IT_U_GRAY | IT_U_PRGB );
        if( original_display ) cvReleaseImage( &original_display );
        dh = im->h();
        dw = im->w();
        original_display = cvCreateImageHeader( cvSize(dw,dh), IPL_DEPTH_8U, 3);

        cvCreateData(original_display);
        copy_image_to_color_ipl(im, original_display);
        reset_display();
    }

    void gui_window::save_screen( const string& file ) const {
        cvSaveImage( file.c_str(), display );
    }

    void gui_window::zoom_to_point( const int& x, const int& y, const int& wsz) {
        Image tmp_img;
        copy_ipl_to_image(display, &tmp_img);
        int x0 = x-wsz/2; if( x0<0 ) x0 = 0;
        int y0 = y-wsz/2; if( y0<0 ) y0 = 0;
        Image zoomed_image;
        zoomed_image.copy_from_region( &tmp_img, x0, y0, wsz, wsz, 0, 0 );
        copy_image_to_color_ipl(&zoomed_image, display);
    }

    const IplImage* gui_window::get_original_display() const {
        return original_display;
    }

    const IplImage* gui_window::get_display() const {
        return display;
    }

    void gui_window::set_display(const gui_window* src_wnd, const int& x, const int& y, const int& wsz) {
        const IplImage* sdisplay = src_wnd->get_display();
        copy_to_color_ipl(sdisplay, x, y, wsz, display);
    }

    void gui_window::set_original_display(const gui_window* src_wnd, const int& x, const int& y, const int& wsz) {
        const IplImage* sdisplay = src_wnd->get_original_display();
        copy_to_color_ipl(sdisplay, x, y, wsz, original_display);
        reset_display();
    }

    void gui_window::draw_line( int x0, int y0, int x1, int y1) {
        kortex::draw_line( display, x0, y0, x1, y1, &dp_color, dp_thickness);
    }
    void gui_window::draw_rectangle( int x, int y, int dw, int dh) {
        kortex::draw_rectangle( display, x, y, dw, dh, &dp_color, dp_thickness);
    }
    void gui_window::draw_circle( int x, int y, int dr ) {
        kortex::draw_circle( display, x, y, dr, &dp_color, dp_thickness);
    }
    void gui_window::draw_polygon(int* xy, int no_points ) {
        kortex::draw_polygon( display, xy, no_points, &dp_color, dp_thickness);
    }
    void gui_window::draw_ray( int x0, int y0, float length, float angle) {
        kortex::draw_ray( display, x0, y0, length, angle, &dp_color, dp_thickness);
    }
    void gui_window::mark( int x, int y, int thickness ) {
        if( thickness == -1 ) kortex::draw_marker( display, x, y, &dp_color, dp_thickness );
        else                  kortex::draw_marker( display, x, y, &dp_color, thickness );
    }
    void gui_window::mark_region( int* mark, bool permanent ) {
        if( permanent ) {
            overlay_region(original_display, mark);
            reset_display();
        } else {
            overlay_region( display, mark );
        }
    }
    void gui_window::write(int x, int y, const string& text) {
        write_on_image(display, x, y, text, &dp_color, dp_font);
    }
    void gui_window::write(int x, int y, int num) {
        string text = num2str(num);
        write_on_image(display, x, y, text, &dp_color, dp_font);
    }
    void gui_window::write(int x, int y, float num) {
        string text = num2str(num,8);
        write_on_image(display, x, y, text, &dp_color, dp_font);
    }
    void gui_window::write(int x, int y, double num) {
        string text = num2str(num,8);
        write_on_image(display, x, y, text, &dp_color, dp_font);
    }
    void gui_window::reset_display() {
        if( display ) cvReleaseImage(&display);
        display = cvCloneImage(original_display);
    }

    void gui_window::reset() {
        if( display          ) cvReleaseImage(&display);
        if( original_display ) cvReleaseImage(&original_display);
        if( dp_font          ) { delete dp_font; dp_font = NULL; }
        init_();
    }

    void gui_window::reset_mouse() {
        g_mouse_callback.xstart = 0;
        g_mouse_callback.ystart = 0;
        g_mouse_callback.xend = dw;
        g_mouse_callback.yend = dh;
        g_mouse_callback.x = -1;
        g_mouse_callback.y = -1;
        g_mouse_callback.event = -1;
    }
    void gui_window::init_mouse() {
        reset_mouse();
        cvSetMouseCallback(wname.c_str(), mouse_callback, 0 );
    }
    int gui_window::wait( const int& ms ) const {
        return ((cvWaitKey(ms)) & 0xffff);
    }
    bool gui_window::mouse_click( const int& button, int &x, int &y ) const {
        if( g_mouse_callback.x > 0 && g_mouse_callback.y > 0 && g_mouse_callback.event == button ) {
            x = g_mouse_callback.x;
            y = g_mouse_callback.y;
            return true;
        } else {
            return false;
        }
    }
    bool gui_window::mouse_move_event(int &x, int &y ) const {
        if( g_mouse_callback.x > 0 && g_mouse_callback.y > 0 && g_mouse_callback.event == CV_EVENT_MOUSEMOVE ) {
            x = g_mouse_callback.x;
            y = g_mouse_callback.y;
            return true;
        } else {
            return false;
        }
    }

    void gui_window::move( const int& x, const int& y ) {
        py = y;
        px = x;
        cvMoveWindow(wname.c_str(), px, py);
    }
    void gui_window::show() {
        assert( wname != "" );
        cvShowImage( wname.c_str(), display);
    }
    void gui_window::refresh() {
        cvShowImage( wname.c_str(), display);
    }
    void gui_window::resize(const int& nw, const int& nh) {
        cvResizeWindow(wname.c_str(), nw, nh);
    }
    void gui_window::set_thickness(const int& t) {
        dp_thickness = t;
    }

    void gui_window::set_color( const uchar& r, const uchar& g, const uchar& b ) {
        dp_color = Color(r,g,b);
    }

    void gui_window::set_color( const Color& col ) {
        dp_color = col;
    }

    void gui_window::set_font( float hs, float vs, float shear, float thickness ) {
        cvInitFont(dp_font, CV_FONT_HERSHEY_PLAIN, hs, vs, shear, thickness, CV_AA);
    }
}
#endif
