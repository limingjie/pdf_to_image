extern "C" {
#include <mupdf/fitz.h>
}

#include <string>

class pdf
{
private:
	fz_context *ctx;
	fz_document *doc;
	int pagecount;
	std::string _filename;
	bool _good;

public:
	pdf(const char *filename);
	~pdf();

	int size();
	bool good();

	void render(int from, int to, int zoom = 100);
};
