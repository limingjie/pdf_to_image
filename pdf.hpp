extern "C" {
#include <mupdf/fitz.h>
}

#include <string>

class pdf
{
private:
    fz_context  *ctx;
    fz_document *doc;

    bool         _good;
    char        *_image_name;
    int          _size;
    std::string  _filename;

    void generate_image_name(int page, int width);

public:
    pdf(const char *filename);
    ~pdf();

    int size();
    bool good();

    bool render(int from, int to, int zoom = 100);
};
