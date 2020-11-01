#ifndef __GTKENCRYPT_H
#define __GTKENCRYPT_H

#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <TextFrame.h>
#include <cryptButton.h>

using namespace Gtk;



class Encrypt : public Gtk::Box 
{
public:
    Encrypt (Orientation orientation = ORIENTATION_HORIZONTAL, int spacing = 0);
    virtual  ~Encrypt ();
protected:
    TextFrame text, key, cipher_text;
    cryptButton buttons;

    void crypt_process();
    void clear_buffer();

};

#endif