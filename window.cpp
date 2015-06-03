#include "window.hpp"
#include "pdf.hpp"

window::window(int x, int y, int w, int h, const char *label)
    : Fl_Window(x, y, w, h, label)
{
    create_ui();
}

void window::create_ui()
{
    filename_input = new Fl_Input(10, 10, 290, 25, "");

    Fl_Button *pick_button = new Fl_Button(310, 10, 60, 25, "...");
    pick_button->callback(on_pick_button_click, this);

    Fl_Button *go_button = new Fl_Button(380, 10, 60, 25, "Go");
    go_button->callback(on_go_button_click, this);

    Fl_Box *from_label = new Fl_Box(10, 45, 45, 25, "From:");
    from_page_input = new Fl_Int_Input(55, 45, 40, 25, "");

    Fl_Box *to_label = new Fl_Box(100, 45, 25, 25, "to:");
    to_page_input = new Fl_Int_Input(125, 45, 40, 25, "");

    Fl_Box *quality_label = new Fl_Box(170, 45, 70, 25, "Quality:");

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
        break;
    case 1:
        break;
    }
}

void window::on_go_button_click(Fl_Widget *sender, void *obj)
{
    window *w = (window *)obj;

    if (w->filename_input->size() != 0)
    {
        w->process();
    }
}

void window::process()
{
    pdf p(filename_input->value());
    if (p.good())
    {
        p.render(1, 10);
    }
    else
    {
        fl_alert("Cannot open document!");
    }
}
