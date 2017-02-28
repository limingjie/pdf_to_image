#include <stdexcept>
#include <FL/Fl_Menu_Button.H>

#include "window.hpp"

const std::string resolutions[3] = {"Web (72dpi)", "Print (300dpi)", "Large (600dpi)"};

// compare string case insensitive
static bool iequals(const std::string &str1, const std::string &str2)
{
    if (str1.size() != str2.size())
    {
        return false;
    }

    auto c1  = str1.cbegin();
    auto c2  = str2.cbegin();
    auto end = str1.cend();

    while (c1 != end)
    {
        if (tolower(*c1) != tolower(*c2))
        {
            return false;
        }

        ++c1;
        ++c2;
    }

    return true;
}

window::window(int x, int y, int w, int h, const char *label)
    : Fl_Window(x, y, w, h, label)
{
    create_ui();
    _pdf = nullptr;
}

void window::create_ui()
{
    _filename_input = new Fl_Input(10, 10, 250, 25, "");
    _filename_input->readonly(1);

    Fl_Button *pick_button = new Fl_Button(270, 10, 55, 25, "...");
    pick_button->callback(on_pick_button_click, this);
    pick_button->take_focus();

    Fl_Button *go_button = new Fl_Button(335, 10, 55, 25, "Go");
    go_button->callback(on_go_button_click, this);

    _page_check = new Fl_Check_Button(10, 45, 25, 25, "");
    _page_check->value(0);
    _page_check->callback(on_page_check_click, this);

    _from_page_spinner = new Fl_Spinner(80, 45, 50, 25, "Pages:");
    _from_page_spinner->step(1);
    _from_page_spinner->minimum(1);
    _from_page_spinner->maximum(1);
    _from_page_spinner->value(1);
    _from_page_spinner->deactivate();
    _from_page_spinner->when(FL_WHEN_CHANGED);
    _from_page_spinner->callback(on_from_page_spinner_change, this);

    _to_page_spinner = new Fl_Spinner(140, 45, 50, 25, "-");
    _to_page_spinner->step(1);
    _to_page_spinner->minimum(1);
    _to_page_spinner->maximum(1);
    _to_page_spinner->value(1);
    _to_page_spinner->deactivate();
    _to_page_spinner->when(FL_WHEN_CHANGED);
    _to_page_spinner->callback(on_to_page_spinner_change, this);

    _resolution_choice = new Fl_Input_Choice(270, 45, 120, 25, "Resolution:");
    Fl_Menu_Button *_menu_button = _resolution_choice->menubutton();
    _menu_button->add(resolutions[0].c_str());
    _menu_button->add(resolutions[1].c_str());
    _menu_button->add(resolutions[2].c_str());
    _resolution_choice->value(0);

    Fl_Button *author = new Fl_Button(10, 75, 380, 15,
        "Mingjie Li (https://github.com/limingjie/pdf_to_image)");
    author->box(FL_FLAT_BOX);
    author->align(FL_ALIGN_INSIDE | FL_ALIGN_RIGHT);
    author->labelsize(12);
    author->labelcolor(fl_rgb_color(128, 128, 128));
    author->callback(on_author_click, this);
}

void window::on_pick_button_click(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    Fl_Native_File_Chooser native;
    native.title("Open");
    native.type(Fl_Native_File_Chooser::BROWSE_FILE);
    native.filter("PDF\t*.pdf");
    if (w->_filename_input->size() != 0)
    {
        native.preset_file(w->_filename_input->value());
    }

    switch (native.show())
    {
    case -1: // failure
        fl_alert(native.errmsg());
        break;
    case 0:
        w->_filename_input->value(native.filename());
        w->open_pdf();
        break;
    case 1: // cancel
        break;
    }
}

void window::on_go_button_click(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->_filename_input->size() == 0)
    {
        fl_alert("Select a PDF document.");
        return;
    }

    if (!w->_pdf->good())
    {
        fl_alert("Cannot open the PDF document.");
        return;
    }

    int from;
    int to;
    if (w->_page_check->value() == 1)
    {
        // convert from page
        from = w->_from_page_spinner->value();
        to   = w->_to_page_spinner->value();

        // check to page range
        if (to < from)
        {
            fl_alert("To page should be great than or equal to start page.");
            return;
        }
    }
    else
    {
        from = 1;
        to = w->_pdf->size();
    }

    int zoom;
	std::string resolution = w->_resolution_choice->value();
    if (iequals(resolution, resolutions[0]))
    {
        zoom = 72;
    }
    else if (iequals(resolution, resolutions[1]))
    {
        zoom = 300;
    }
    else if (iequals(resolution, resolutions[2]))
    {
        zoom = 600;
    }
    else
    {
        bool converted = true;
        try {
            zoom = std::stoi(resolution);
            if (zoom < 72 || zoom > 1000)
            {
                converted = false;
            }
        }
        catch (const std::invalid_argument &invalid) {
            converted = false;
        }
        catch (const std::out_of_range &out) {
            converted = false;
        }

        if (!converted)
        {
            fl_alert("Invalid resolution, enter a value between 72 and 1000.");
            return;
        }
    }

    // select export directory
    Fl_Native_File_Chooser native;
    native.title("Choose directory");
    native.options(Fl_Native_File_Chooser::NEW_FOLDER);
    native.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
    switch (native.show())
    {
    case -1: // failure
        fl_alert(native.errmsg());
        return;
    case 0:
        break;
    case 1: // cancel
        return;
    }

    std::string path(native.filename());
    path += '/';
    path += fl_filename_name(w->_filename_input->value());

    if (w->_pdf->render(path.c_str(), from, to, zoom))
    {
        fl_alert("Done.");
    }
    else
    {
        fl_alert("Failed to render document.");
    }
}

void window::on_page_check_click(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->_page_check->value() == 0) // unchecked
    {
        w->_from_page_spinner->deactivate();
        w->_to_page_spinner->deactivate();
    }
    else
    {
        w->_from_page_spinner->activate();
        w->_to_page_spinner->activate();
    }
}

void window::on_from_page_spinner_change(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->_from_page_spinner->value() > w->_to_page_spinner->value())
    {
        w->_to_page_spinner->value(w->_from_page_spinner->value());
    }
}

void window::on_to_page_spinner_change(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->_from_page_spinner->value() > w->_to_page_spinner->value())
    {
        w->_from_page_spinner->value(w->_to_page_spinner->value());
    }
}

void window::on_author_click(Fl_Widget *sender, void *obj)
{
    char errmsg[512];
    fl_open_uri("https://github.com/limingjie/pdf_to_image",
        errmsg, sizeof(errmsg));
}

void window::open_pdf()
{
    if (_pdf)
    {
        delete _pdf;
        _pdf = nullptr;
    }

    _pdf = new pdf(_filename_input->value());

    int count = 1;
    const char *password;
    while (_pdf->needs_password())
    {
        password = fl_password("Enter password for %s. (%d/3)", "",
                               _filename_input->value(), count);

        if (password == NULL) // canceled
        {
            fl_alert("Failed to open %s.", _filename_input->value());
            _filename_input->value("");
            return;
        }

        // check password
        if (_pdf->set_password(password))
        {
            break;
        }

        // exceed maximum attempts
        if (++count > 3)
        {
            fl_alert("Exceed maximum password attempts.");
            _filename_input->value("");
            return;
        }
    }

    if (_pdf->good() && _pdf->size() != 0)
    {
        int page = _pdf->size();
        _from_page_spinner->minimum(1);
        _from_page_spinner->maximum(page);
        _from_page_spinner->value(1);

        _to_page_spinner->minimum(1);
        _to_page_spinner->maximum(page);
        _to_page_spinner->value(page);
    }
    else
    {
        _from_page_spinner->minimum(1);
        _from_page_spinner->maximum(1);
        _from_page_spinner->value(1);

        _to_page_spinner->minimum(1);
        _to_page_spinner->maximum(1);
        _to_page_spinner->value(1);
    }
}
