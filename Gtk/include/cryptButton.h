#ifndef __CRYPTBUTTON_H
#define __CRYPTBUTTON_H
#include <gtkmm-3.0/gtkmm.h>
#include <iostream>
using namespace std;
using namespace Gtk;

class cryptButton : public Gtk::ButtonBox 
{
public : 
    cryptButton(const string &button_name,
        Orientation orientataion = Gtk::ORIENTATION_HORIZONTAL, int spacing = 0
    );
    Button but1, but2;
};



#endif