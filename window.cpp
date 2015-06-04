#include "window.hpp"

#include <string>

window::window(int x, int y, int w, int h, const char *label)
    : Fl_Window(x, y, w, h, label)
{
    create_ui();
    p = nullptr;
}

void window::create_ui()
{
    filename_input = new Fl_Input(10, 10, 240, 25, "");

    Fl_Button *pick_button = new Fl_Button(260, 10, 40, 25, "...");
    pick_button->callback(on_pick_button_click, this);

    Fl_Button *go_button = new Fl_Button(310, 10, 60, 25, "Go");
    go_button->callback(on_go_button_click, this);

    page_check = new Fl_Check_Button(10, 45, 25, 25, "");
    page_check->value(0);
    page_check->callback(on_page_check_click, this);

    from_page_input = new Fl_Int_Input(90, 45, 40, 25, "Pages:");
    from_page_input->deactivate();

    to_page_input = new Fl_Int_Input(140, 45, 40, 25, "-");
    to_page_input->deactivate();

    quality_choice = new Fl_Choice(250, 45, 120, 25, "Quality:");
    quality_choice->add("Web (72dpi)");
    quality_choice->add("Print (300dpi)");
    quality_choice->value(0);
}

void window::on_pick_button_click(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    Fl_Native_File_Chooser native;
    native.title("Pick a file");
    native.type(Fl_Native_File_Chooser::BROWSE_FILE);
    native.filter("PDF\t*.pdf");
    if (w->filename_input->size() != 0)
    {
        native.preset_file(w->filename_input->value());
    }

    switch (native.show())
    {
    case -1:
        fl_alert(native.errmsg());
        break;
    case 0:
        w->filename_input->value(native.filename());
        w->open_pdf();
        break;
    case 1:
        break;
    }
}

void window::on_go_button_click(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->filename_input->size() == 0)
    {
        fl_alert("Please pick a document.");
        return;
    }

    if (!w->p->good())
    {
        fl_alert("Cannot open document.");
        return;
    }

    int from;
    int to;
    if (w->page_check->value() == 1)
    {
        // check from page empty
        if (w->from_page_input->size() == 0)
        {
            fl_alert("From page cannot be empty.");
            return;
        }

        // check to page empty
        if (w->to_page_input->size() == 0)
        {
            fl_alert("To page cannot be empty.");
            return;
        }

        // convert from page
        try
        {
            from = std::stoi(w->from_page_input->value());
        }
        catch (...)
        {
            from = 1;
        }

        // check from page range
        if (from < 1)
        {
            fl_alert("From page should start from 1.");
            return;
        }

        if (from > w->p->size())
        {
            fl_alert("From page too big.");
            return;
        }

        // convert to page
        try
        {
            to = std::stoi(w->to_page_input->value());
        }
        catch (...)
        {
            to = w->p->size();
        }

        // check to page range
        if (to < from)
        {
            fl_alert("To page should be great than or equal to start page.");
            return;
        }

        if (to > w->p->size())
        {
            fl_alert("To page too large.");
            return;
        }
    }
    else
    {
        from = 1;
        to = w->p->size();
    }

    int zoom;
    if (w->quality_choice->value() == 0)
    {
        zoom = 100; // 72dpi
    }
    else
    {
        zoom = 417; // 300dpi
    }

    w->p->render(from, to, zoom);
}

void window::on_page_check_click(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->page_check->value() == 0)
    {
        w->from_page_input->deactivate();
        w->to_page_input->deactivate();
    }
    else
    {
        w->from_page_input->activate();
        w->to_page_input->activate();
    }
}

void window::open_pdf()
{
    if (p)
    {
        delete p;
        p = nullptr;
    }

    p = new pdf(filename_input->value());

    if (p->good())
    {
        from_page_input->value("1");
        to_page_input->value(std::to_string(p->size()).c_str());
    }
    else
    {
        from_page_input->value("");
        to_page_input->value("");
    }
}
