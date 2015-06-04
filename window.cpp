#include "window.hpp"

window::window(int x, int y, int w, int h, const char *label)
    : Fl_Window(x, y, w, h, label)
{
    create_ui();
    p = nullptr;
}

void window::create_ui()
{
    filename_input = new Fl_Input(10, 10, 250, 25, "");
    filename_input->readonly(1);

    Fl_Button *pick_button = new Fl_Button(270, 10, 55, 25, "...");
    pick_button->callback(on_pick_button_click, this);
    pick_button->take_focus();

    Fl_Button *go_button = new Fl_Button(335, 10, 55, 25, "Go");
    go_button->callback(on_go_button_click, this);

    page_check = new Fl_Check_Button(10, 45, 25, 25, "");
    page_check->value(0);
    page_check->callback(on_page_check_click, this);

    from_page_spinner = new Fl_Spinner(90, 45, 50, 25, "Pages:");
    from_page_spinner->step(1);
    from_page_spinner->minimum(1);
    from_page_spinner->maximum(1);
    from_page_spinner->value(1);
    from_page_spinner->deactivate();
    from_page_spinner->when(FL_WHEN_CHANGED);
    from_page_spinner->callback(on_from_page_spinner_change, this);

    to_page_spinner = new Fl_Spinner(150, 45, 50, 25, "-");
    to_page_spinner->step(1);
    to_page_spinner->minimum(1);
    to_page_spinner->maximum(1);
    to_page_spinner->value(1);
    to_page_spinner->deactivate();
    to_page_spinner->when(FL_WHEN_CHANGED);
    to_page_spinner->callback(on_to_page_spinner_change, this);

    quality_choice = new Fl_Choice(270, 45, 120, 25, "Quality:");
    quality_choice->add("Web (72dpi)");
    quality_choice->add("Print (300dpi)");
    quality_choice->value(0);
}

void window::on_pick_button_click(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    Fl_Native_File_Chooser native;
    native.title("Pick a document");
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
        // convert from page
        from = w->from_page_spinner->value();
        to = w->to_page_spinner->value();

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
        w->from_page_spinner->deactivate();
        w->to_page_spinner->deactivate();
    }
    else
    {
        w->from_page_spinner->activate();
        w->to_page_spinner->activate();
    }
}

void window::on_from_page_spinner_change(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->from_page_spinner->value() > w->to_page_spinner->value())
    {
        w->to_page_spinner->value(w->from_page_spinner->value());
    }
}

void window::on_to_page_spinner_change(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->from_page_spinner->value() > w->to_page_spinner->value())
    {
        w->from_page_spinner->value(w->to_page_spinner->value());
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

    if (p->good() && p->size() != 0)
    {
        int page = p->size();
        from_page_spinner->minimum(1);
        from_page_spinner->maximum(page);
        from_page_spinner->value(1);

        to_page_spinner->minimum(1);
        to_page_spinner->maximum(page);
        to_page_spinner->value(page);
    }
    else
    {
        from_page_spinner->minimum(1);
        from_page_spinner->maximum(1);
        from_page_spinner->value(1);

        to_page_spinner->minimum(1);
        to_page_spinner->maximum(1);
        to_page_spinner->value(1);
    }
}
