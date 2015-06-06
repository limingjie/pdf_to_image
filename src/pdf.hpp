//
// pdf.hpp
//
// June 5, 2015 - by Mingjie Li (limingjie@outlook.com)
// https://github.com/limingjie
//

#pragma once

extern "C" {
#include <mupdf/fitz.h>
}

#include <string>

class pdf
{
private:
    fz_context  *_ctx;
    fz_document *_doc;

    bool         _good;
    bool         _needs_password;
    char        *_image_name;
    int          _size;
    std::string  _filename;

    void generate_image_name(int page, int width);

public:
    pdf(const char *filename);
    ~pdf();

    bool needs_password();
    bool set_password(const char *password);

    int  size();
    bool good();
    bool render(int from, int to, int zoom = 100);
};
