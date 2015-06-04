#include "window.hpp"

int main(int argc, char *argv[])
{
    Fl::get_system_colors();
    window *w = new window(100, 100, 380, 80, "PDF to Image");
    w->show();
    return Fl::run();
}
