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

#ifndef KORTEX_IMAGE_TEXT_INFO_H
#define KORTEX_IMAGE_TEXT_INFO_H

#include <kortex/color.h>
#include <string>

namespace kortex {

    struct ImageTextInfo {
        int x;
        int y;
        std::string text;
        ColorName color;
    };

}

#endif
