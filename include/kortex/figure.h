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

#ifndef KORTEX_FIGURE_H
#define KORTEX_FIGURE_H

#include <kortex/color.h>
#include <kortex/rect2.h>

#include <string>
using std::string;

namespace kortex {

    class gui_window;

    class figure {
    public:
        figure();
        figure(const string& name);
        ~figure();
        void create(const string& name="");
        void destroy();

        void set_name(const string& name);

        // set/get
        void set_range(Rect2f rng)     { range = rng; }
        void set_ticks(int yt, int xt) { yticks=yt; xticks=xt; }

        void axis  (bool s) { baxes=s;   }
        void grid  (bool s) { bgrid=s;   }
        void legend(bool s) { blegend=s; }

        void set(const float* arr, const int& n);
        void set(const uchar* arr, const int& n);
        void set(const int  * arr, const int& n);

        void set_line_color(int r, int g, int b) { line_color=Color(b,g,r); }
        void set_bar_color (int r, int g, int b) { bar_color =Color(b,g,r); }
        void set_axis_color(int r, int g, int b) { axis_color=Color(b,g,r); }

        void set_thickness (int t) { line_thickness = t; }
        void set_margin    (int m) { margin = m; }

        void set_title(std::string s) { title=s; }

        void move( const int& x, const int& y );

        // display
        void mark(int n, int r=-1, int g=-1, int b=-1);
        void plot(const float* arr, int n, int r=-1, int g=-1, int b=-1);
        void line(float y0, int n0, float y1, int n1);
        void plot();
        void show();
        void refresh();
        void reset ( const int& nw=-1, const int& nh=-1 );
        void resize( const int& nw, const int& nh );
        int  wait(const int& n) const;

        void draw_axis();
        void draw_grid();
        void stem(int index=-1);
        void text(int index, std::string text);
        void scatter(int index=-1);
        void bar(bool filled=true);

    private:
        void init_();

        float* farr;
        int    narr;

        Rect2f range;
        int yticks;
        int xticks;

        bool baxes;
        bool bgrid;
        bool blegend;
        int  margin;

        int h;
        int w;

        int     line_thickness;
        Color   axis_color;
        Color   line_color;
        Color   bar_color;
        Color   text_color;
        Color   mark_color;

        string title;
        string wname;
        gui_window* wnd;

    };

}

#endif

#endif
