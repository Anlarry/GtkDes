#include <cryptButton.h>
using namespace Gtk;


cryptButton::cryptButton(
    const string &button_name,
    Orientation orientation , int spacing) 
:   
    but1(button_name), but2("Clear")
{

    set_border_width(10);
    set_spacing(5);
    pack_start(but1, Gtk::PACK_SHRINK);
    pack_start(but2, Gtk::PACK_SHRINK);
    set_layout(Gtk::BUTTONBOX_END);

}