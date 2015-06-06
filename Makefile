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

pdf2image$(X): obj/main.o obj/window.o obj/pdf.o
	@echo "==>Linking pdf2image$(X)..."
	$(CXX) -o pdf2image$(X) obj/main.o obj/window.o obj/pdf.o $(FLTK_LDFLAGS) $(MUPDF_LDFLAGS)

obj/main.o: src/main.cpp src/window.hpp src/pdf.hpp
	@echo "==>Compiling main.cpp..."
	$(CXX) -c $(CXXFLAGS) $(FLTK_CXXFLAGS) $(MUPDF_CXXFLAGS) -o obj/main.o src/main.cpp

obj/window.o: src/window.cpp src/window.hpp src/pdf.hpp
	@echo "==>Compiling window.cpp..."
	$(CXX) -c $(CXXFLAGS) $(FLTK_CXXFLAGS) -o obj/window.o src/window.cpp

obj/pdf.o: src/pdf.cpp src/pdf.hpp
	@echo "==>Compiling pdf.cpp..."
	$(CXX) -c $(CXXFLAGS) $(MUPDF_CXXFLAGS) -o obj/pdf.o src/pdf.cpp

clean:
	@echo "==>Cleaning..."
	rm -rf obj/*.o
	rm -rf pdf2image$(X)
