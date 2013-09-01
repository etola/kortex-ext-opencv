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

#ifndef KORTEX_PAINT_OPENCV_H
#define KORTEX_PAINT_OPENCV_H

#include "kortex/types.h"
#include "kortex/image_text_info.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

struct _IplImage;
typedef struct _IplImage IplImage;
struct CvFont;

namespace kortex {

    class Color;
    class Image;

    void draw_ray      ( IplImage* img, int x0,  int y0, float length, float angle, Color* color, int thickness=1);
    void draw_line     ( IplImage* img, int x0,  int y0, int x1, int y1, Color* color, int thickness=1);
    void draw_circle   ( IplImage* img, int x,   int y,  int dr, Color* color, int thickness=1);
    void draw_marker   ( IplImage* img, int x,   int y,  Color* color, int thickness);
    void draw_rectangle( IplImage* img, int x,   int y, int dw, int dh, Color* color, int thickness=1);
    void draw_polygon  ( IplImage* img, int* xy, int no_points, Color* color, int thickness=1 );

    void write_on_image(IplImage* img, int x, int y, string text, Color* color, CvFont* display_font);
    void write_on_image_cv(Image* img, const vector<ImageTextInfo> &info );

    void overlay_region(IplImage* img, int* occmap);

    void copy_to_color_ipl(const IplImage* src, int x, int y, int w, IplImage* &dest);

    void copy_image_to_color_ipl(const uchar* im, int w, int h, int nc, IplImage* ipl );
    void copy_image_to_color_ipl(const Image* im, IplImage*& ipl );

    void copy_ipl_to_image( const IplImage* ipl, Image *im );

    void write_on_image( Image* im, const vector<ImageTextInfo>& tinfo );

}
#endif
