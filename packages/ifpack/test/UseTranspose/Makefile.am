#@HEADER
# ************************************************************************
# 
#       Ifpack: Object-Oriented Algebraic Preconditioner Package
#                 Copyright (2002) Sandia Corporation
# 
# Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
# license for use of this work by or on behalf of the U.S. Government.
# 
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#  
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#  
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA
# Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
# 
# ************************************************************************
#@HEADER

include $(top_builddir)/Makefile.export.ifpack

if HAVE_IFPACK_GALERI
include $(top_builddir)/../galeri/Makefile.export.galeri
EXAMPLE_LIBS     = $(IFPACK_LIBS)      $(GALERI_LIBS)
EXAMPLE_INCLUDES = $(IFPACK_INCLUDES)  $(GALERI_INCLUDES)
else
EXAMPLE_LIBS     = $(IFPACK_LIBS)
EXAMPLE_INCLUDES = $(IFPACK_INCLUDES)
endif

EXEEXT = .exe

noinst_PROGRAMS = TestUseTranspose

TestUseTranspose_SOURCES      = $(srcdir)/cxx_main.cpp
TestUseTranspose_DEPENDENCIES = $(top_builddir)/src/libifpack.a 
TestUseTranspose_CXXFLAGS     = $(EXAMPLE_INCLUDES)
TestUseTranspose_LDADD        = $(EXAMPLE_LIBS)

