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

#include <kortex/opencv_extensions.h>
#include <kortex/image.h>
#include <kortex/color.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

namespace kortex {

    void draw_line(IplImage* img, int x0, int y0, int x1, int y1, Color* color, int thickness) {
        CvScalar col = cvScalar( color->b, color->g, color->r );
        cvLine( img, cvPoint(x0+.5, y0+.5), cvPoint(x1+.5, y1+.5), col, thickness, CV_AA);
    }

    void draw_polygon(IplImage* img, int* xy, int no_points, Color* color, int thickness ) {
        for( int n=0; n<no_points-1; n++ )
            draw_line(img, xy[2*n], xy[2*n+1], xy[2*n+2], xy[2*n+3], color, thickness );
        draw_line(img, xy[2*no_points-2], xy[2*no_points-1], xy[0], xy[1], color, thickness );
    }


    void draw_ray(IplImage* img, int x0, int y0, float length, float angle, Color* color, int thickness) {
        int px0 = x0+.5;
        int py0 = y0+.5;
        int py1 = py0 + length * sin(angle);
        int px1 = px0 + length * cos(angle);
        draw_line(img, px0, py0, px1, py1, color, thickness );
    }

    void draw_rectangle(IplImage* img, int x, int y, int dw, int dh, Color* color, int thickness) {
        CvScalar col = cvScalar( color->b, color->g, color->r );
        cvRectangle(img, cvPoint(x,y), cvPoint(x+dw,y+dh), col, thickness, CV_AA );
    }

    void draw_circle(IplImage* img, int x, int y, int dr, Color* color, int thickness) {
        CvScalar col = cvScalar( color->b, color->g, color->r );
        cvCircle(img, cvPoint(x, y), dr, col, thickness, CV_AA);
    }

    void draw_marker(IplImage* img, int x, int y, Color* color, int thickness) {

        if( x < 0 || x >= img->width || y < 0 || y>= img->height )
            return;

        switch( thickness ) {
        case 0:
            (img->imageData+y*img->widthStep)[x*img->nChannels  ] = color->r;
            (img->imageData+y*img->widthStep)[x*img->nChannels+1] = color->g;
            (img->imageData+y*img->widthStep)[x*img->nChannels+2] = color->b;
            break;
        default:
            draw_circle(img,x,y,2,color,thickness);
            break;
        }
    }

    void write_on_image(IplImage* img, int x, int y, string text, Color* color, CvFont* display_font) {
        CvScalar col = cvScalar( color->b, color->g, color->r );
        cvPutText(img, text.c_str(), cvPoint(x,y+10), display_font, col);
    }

    void write_on_image_cv(Image* img, const std::vector<ImageTextInfo> &info ) {
        assert_pointer( img );

        CvFont dp_font; cvInitFont(&dp_font, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1, CV_AA);
        IplImage*tmp = cvCreateImage(cvSize(img->w(), img->h()), IPL_DEPTH_8U, 3);
        copy_image_to_color_ipl(img, tmp);

        for(size_t n=0; n<info.size(); n++ ) {
            ImageTextInfo iti = info.at(n);
            Color col;
            get_color(iti.color, col.r, col.g, col.b);
            write_on_image(tmp, iti.x, iti.y, iti.text, &col, &dp_font);
        }
        copy_ipl_to_image(tmp, img);
        cvReleaseImage(&tmp);
    }

    void overlay_region(IplImage* img, int* occmap ) {
        assert_pointer( img && occmap );
        int h = img->height;
        int w = img->width;
        for( int y=0; y<h; y++ ) {
            for( int x=0; x<w; x++ ) {
                if( occmap[y*w+x] ) {
                    ( (uchar*)( img->imageData + y*img->widthStep ) ) [x*img->nChannels] = 255;
                }
            }
        }
    }

    void copy_image_to_color_ipl( const uchar* data, int w, int h, int nc, IplImage* ipl ) {
        assert_pointer( data && ipl );
        passert_statement( ipl->height == h, "dimension mismatch" );
        passert_statement( ipl->width  == w, "dimension mismatch" );
        passert_statement( ipl->nChannels  == 3, "dimension mismatch" );

        for( int y=0; y<h; y++ ) {
            for( int x=0; x<w; x++ ) {
                int ind = nc * ( y*w+x );
                if( nc == 1 ) {
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+0] = data[ind];
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+1] = data[ind];
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+2] = data[ind];
                } else  {
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+0] = data[ind+2];
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+1] = data[ind+1];
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+2] = data[ind  ];
                }
            }
        }
    }

    void copy_to_color_ipl(const IplImage* src, int x, int y, int w, IplImage* &dest) {
        assert_pointer( src && dest );
        if( dest->height != w || dest->width != w ) {
            cvReleaseImage(&dest);
            dest = cvCreateImage( cvSize(w,w), IPL_DEPTH_8U, 3 );
        }
        cvSet(dest, cvScalar(0,0,0));

        int w2 = (w-1)/2;
        int xs = x-w2; if( xs < 0 ) xs=0;
        int xe = x+w2; if( xe > src->width ) xe = src->width;
        int ys = y-w2; if( ys < 0 ) ys=0;
        int ye = y+w2; if( ye > src->height ) ye = src->height;

        for( int yu=-w2; yu<=w2; yu++ ) {
            int yy = y+yu;
            int dy = yu+w2;
            if( yy<0 || yy>=src->height ) continue;
            assert( dy < w );
            for( int xu=-w2; xu<=w2; xu++ ) {
                int xx = x+xu;
                int dx = xu+w2;
                assert( dx < w );
                if( xx<0 || xx>=src->width ) {
                    (dest->imageData+dy*dest->widthStep)[dx*dest->nChannels  ] = 0;
                    (dest->imageData+dy*dest->widthStep)[dx*dest->nChannels+1] = 0;
                    (dest->imageData+dy*dest->widthStep)[dx*dest->nChannels+2] = 0;
                    continue;
                }
                for(int c=0; c<3; c++)
                    (dest->imageData+dy*dest->widthStep)[dx*dest->nChannels+c] =
                        (src->imageData+yy*src->widthStep)[xx*src->nChannels+c];
            }
        }
    }

    void copy_image_to_color_ipl( const Image* im, IplImage*& ipl ) {
        assert_pointer( im && ipl );
        passert_statement( im->precision() == TYPE_UCHAR, "invalid image precision" );

        int w  = im->w();
        int h  = im->h();
        int nc = im->ch();

        if( ipl->height != h || ipl->width != w ) {
            cvReleaseImage( &ipl );
            ipl = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
        }
        assert( ipl->height == (int)h );
        assert( ipl->width  == (int)w );
        assert( ipl->nChannels >= 3 );

        uchar r, g, b;

        for( int y=0; y<h; y++ ) {
            for( int x=0; x<w; x++ ) {
                if( nc == 1 ) {
                    r = im->getu(x,y);
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+0] = r;
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+1] = r;
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+2] = r;
                } else {
                    im->get(x,y,r,g,b);
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+0] = b;
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+1] = g;
                    (ipl->imageData+y*ipl->widthStep)[x*ipl->nChannels+2] = r;
                }
            }
        }
    }

    void copy_ipl_to_image( const IplImage* ipl, Image *im ) {
        assert_pointer( ipl );
        assert_pointer( im );

        int w  = ipl->width;
        int h  = ipl->height;
        int nc = ipl->nChannels;

        if     ( nc == 1 ) im->create(w, h, IT_U_GRAY);
        else               im->create(w, h, IT_U_PRGB);

        switch( nc ) {
        case 1: {
            for( int y=0; y<h; y++ ) {
                for( int x=0; x<w; x++ ) {
                    const uchar& v = (ipl->imageData + y*ipl->widthStep)[x];
                    im->set(x, y, v);
                }
            }
        } break;
        case 3: {
            for( int y=0; y<h; y++ ) {
                for( int x=0; x<w; x++ ) {
                    const uchar& b = (ipl->imageData + y*ipl->widthStep)[x*ipl->nChannels  ];
                    const uchar& g = (ipl->imageData + y*ipl->widthStep)[x*ipl->nChannels+1];
                    const uchar& r = (ipl->imageData + y*ipl->widthStep)[x*ipl->nChannels+2];
                    im->set(x, y, r, g, b);
                }
            }
        } break;
        default: logman_fatal("invalid channel number");
        }
    }

    void write_on_image( Image* im, const vector<ImageTextInfo>& tinfo ) {
        write_on_image_cv( im, tinfo );
    }


}

#endif
