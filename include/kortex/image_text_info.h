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
#ifndef KORTEX_IMAGE_TEXT_INFO_H
#define KORTEX_IMAGE_TEXT_INFO_H

#include "kortex/color.h"
#include <string>

namespace kortex {

    struct ImageTextInfo {
        int x;
        int y;
        std::string text;
        ColorName color;
        float font_size;
        float font_thickness;
        ImageTextInfo() {
            x = 20;
            y = 20;
            text = "";
            color = COLOR_RED;
            font_size = 1.0f;
            font_thickness = 1.0f;
        }
    };

}

#endif
