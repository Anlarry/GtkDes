#ifndef __DEBUG_H
#define __DEBUG_H

#include <gtkmm-3.0/gtkmm.h>
#include <TextFrame.h>
class DebugWindow : public Gtk::Window 
{
public :
    DebugWindow();
    ~DebugWindow();
protected:
    TextFrame text;
    Gtk::TextView view;
};

#endif