#include "pdf.hpp"

#include <sstream>
#include <algorithm>
#include <cmath>

pdf::pdf(const char *filename)
{
    _filename = filename;

    // Create a context to hold the exception stack and various caches.
    ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);

    // Register the default file types.
    fz_register_document_handlers(ctx);

    // Open the PDF, XPS or CBZ document.
    fz_try(ctx)
    {
        doc = fz_open_document(ctx, filename);
        _good = true;
    }
    fz_catch(ctx)
    {
        _good = false;
        return;
    }

    // Retrieve the number of pages (not used in this example).
    pagecount = fz_count_pages(ctx, doc);
}

pdf::~pdf()
{
    // Clean up.
    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
}

int pdf::size()
{
    return pagecount;
}

bool pdf::good()
{
    return _good;
}

bool pdf::render(int from, int to, int zoom)
{
    if (!_good) return false;

    std::stringstream ss;
    std::string filename;
    char *writable;
    int width = std::log10(to) + 1;

    // Calculate a transform to use when rendering. This transform
    // contains the scale and rotation. Convert zoom percentage to a
    // scaling factor. Without scaling the resolution is 72 dpi.
    fz_matrix transform;
    fz_rotate(&transform, 0);
    fz_pre_scale(&transform, zoom / 100.0f, zoom / 100.0f);

    for (int i = from; i <= to; ++i)
    {
        // generate filename
        ss.str("");
        ss.clear();
        ss << _filename << '.';
        ss.width(width);
        ss.fill('0');
        ss << std::right << i;
        ss << ".png";
        filename = ss.str();
        writable = new char[filename.size() + 1];
        std::copy(filename.begin(), filename.end(), writable);
        writable[filename.size()] = '\0';

        fz_try(ctx)
        {
            fz_page *page;
            fz_rect bounds;
            fz_irect bbox;
            fz_pixmap *pix;
            fz_device *dev;

            // Load the page we want. Page numbering starts from zero.
            page = fz_load_page(ctx, doc, i - 1);

            // Take the page bounds and transform them by the same matrix that
            // we will use to render the page.
            fz_bound_page(ctx, page, &bounds);
            fz_transform_rect(&bounds, &transform);

            // Create a blank pixmap to hold the result of rendering. The
            // pixmap bounds used here are the same as the transformed page
            // bounds, so it will contain the entire page. The page coordinate
            // space has the origin at the top left corner and the x axis
            // extends to the right and the y axis extends down.

            fz_round_rect(&bbox, &bounds);
            pix = fz_new_pixmap_with_bbox(ctx, fz_device_rgb(ctx), &bbox);
            fz_clear_pixmap_with_value(ctx, pix, 0xff);

            // A page consists of a series of objects (text, line art, images,
            // gradients). These objects are passed to a device when the
            // interpreter runs the page. There are several devices, used for
            // different purposes:
            //
            //  draw device -- renders objects to a target pixmap.
            //
            //  text device -- extracts the text in reading order with styling
            //  information. This text can be used to provide text search.
            //
            //  list device -- records the graphic objects in a list that can
            //  be played back through another device. This is useful if you
            //  need to run the same page through multiple devices, without
            //  the overhead of parsing the page each time.

            // Create a draw device with the pixmap as its target.
            // Run the page with the transform.
            dev = fz_new_draw_device(ctx, pix);
            fz_run_page(ctx, page, dev, &transform, NULL);
            fz_drop_device(ctx, dev);

            // Save the pixmap to a file.
            fz_write_png(ctx, pix, writable, 0);
            delete[] writable;

            // Clean up.
            fz_drop_pixmap(ctx, pix);
            fz_drop_page(ctx, page);
        }
        fz_catch(ctx)
        {
            return false;
        }
    }

    return true;
}
