#
# package & author info
#
packagename := kortex-ext-opencv
description := opencv gui extensions of the kortex vision library developed by Aurvis R&D
major_version := 0
minor_version := 1
tiny_version  := 0
# version := major_version . minor_version # depracated
author := Engin Tola
licence := see licence.txt
#
# add your cpp cc files here
#
sources := \
opencv_extensions.cc \
gui_window.cc \
image_gui.cc \
figure.cc

headers := \
opencv_extensions.h \
gui_window.h \
image_gui.h \
figure.h

#
# output info
#
installdir := /home/tola/usr/local/kortex/
external_sources :=
external_libraries := opencv kortex
libdir := lib
srcdir := src
includedir:= include
#
# custom flags
#
define_flags :=
custom_ld_flags :=
custom_cflags := -std=c++0x
# -std=c++0x : to include file cstdint in types.h

#
# optimization & parallelization ?
#
optimize ?= false
parallelize ?= true
boost-thread ?= false
f77 ?= false
sse ?= true
multi-threading ?= false
profile ?= false
#........................................
specialize := true
platform := native
#........................................
compiler := g++
#........................................
include $(MAKEFILE_HEAVEN)/static-variables.makefile
include $(MAKEFILE_HEAVEN)/flags.makefile
include $(MAKEFILE_HEAVEN)/rules.makefile
