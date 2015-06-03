//
// window.hpp
//
// June 20, 2015 - by Mingjie Li (limingjie@outlook.com)
// https://github.com/limingjie
//

#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H> // fl_alert
#include <FL/Fl_Input.H>

class window : public Fl_Window
{
private:
	Fl_Input *filename_input;

	void create_ui();
	static void on_pick_button_click(Fl_Widget *sender, void *obj);

	void process();

public:
	window(int x, int y, int w, int h, const char *label = 0);
};
