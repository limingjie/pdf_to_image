# Check OS
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifneq (,$(findstring MINGW,$(uname_S)))
	X=.exe
endif

ifeq ($(OS),Windows_NT)
	X=.exe
endif

# Check CXX flag.
ifeq ($(CXX),clang++)
	CXX=clang++
else
	CXX=g++
endif

CXXFLAGS       = -Wall -g -O2 -std=c++0x

FLTK_CXXFLAGS  = $(shell fltk-config --cxxflags)
FLTK_LDFLAGS   = $(shell fltk-config --ldstaticflags)

MUPDF_DIR      = ~/Development/mupdf-1.7a-source/build/release
MUPDF_CXXFLAGS = -I/usr/local/include/
MUPDF_LDFLAGS  = /usr/local/lib/libmupdf.a $(MUPDF_DIR)/libz.a    \
				 $(MUPDF_DIR)/libmujs.a $(MUPDF_DIR)/libopenjpeg.a \
				 $(MUPDF_DIR)/libcurl.a $(MUPDF_DIR)/libfreetype.a \
				 $(MUPDF_DIR)/libjpeg.a $(MUPDF_DIR)/libjbig2dec.a \
				 -lcrypto

all: pdf2image$(X)

pdf2image$(X): main.o window.o pdf.o
	@echo "==>Linking pdf2image$(X)..."
	$(CXX) -o pdf2image$(X) main.o window.o pdf.o $(FLTK_LDFLAGS) $(MUPDF_LDFLAGS)

main.o: main.cpp window.hpp pdf.hpp
	@echo "==>Compiling main.cpp..."
	$(CXX) -c $(CXXFLAGS) $(FLTK_CXXFLAGS) $(MUPDF_CXXFLAGS) -o main.o main.cpp

window.o: window.cpp window.hpp pdf.hpp
	@echo "==>Compiling window.cpp..."
	$(CXX) -c $(CXXFLAGS) $(FLTK_CXXFLAGS) -o window.o window.cpp

pdf.o: pdf.cpp pdf.hpp
	@echo "==>Compiling pdf.cpp..."
	$(CXX) -c $(CXXFLAGS) $(MUPDF_CXXFLAGS) -o pdf.o pdf.cpp

clean:
	@echo "==>Cleaning..."
	rm -rf *.o
	rm -rf pdf2image$(X)
