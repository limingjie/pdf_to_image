#include "window.hpp"
#include "pdf.hpp"

window::window(int x, int y, int w, int h, const char *label)
	: Fl_Window(x, y, w, h, label)
{
	create_ui();
}

void window::create_ui()
{
	//Fl_Box *o = new Fl_Box(10, 10, 240, 25, "Convert PDF to images...");
	//o->box(FL_BORDER_BOX);
	//o->align(FL_ALIGN_INSIDE | FL_ALIGN_WRAP | FL_ALIGN_CENTER);
	//o->color((Fl_Color)215);
	//o->labelfont(FL_HELVETICA_BOLD);

	filename_input = new Fl_Input(10, 10, 240, 25, "");

	Fl_Button *but = new Fl_Button(260, 10, 80, 25, "Go");
	but->callback(on_pick_button_click, this);
}

void window::on_pick_button_click(Fl_Widget *sender, void *obj)
{
	window *w = (window *)obj;
	if (w->filename_input->size() == 0)
	{
		Fl_Native_File_Chooser native;
		native.title("Pick a file");
		native.type(Fl_Native_File_Chooser::BROWSE_FILE);
		native.filter("PDF\t*.pdf");

		if (native.show() == 0)
		{
			w->filename_input->value(native.filename());
		}
	}

	w->process();
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