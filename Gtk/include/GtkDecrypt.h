#ifndef __GTKDECRYPT_H
#define __GTKDECRYPT_H

#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <TextFrame.h>
#include <cryptButton.h>
#include <Config.h>

using namespace Gtk;

class Decrypt : public Gtk::Box 
{
public:
    Decrypt (Config* config_ptr, Orientation orientation = ORIENTATION_HORIZONTAL, int spacing = 0);
    virtual  ~Decrypt ();
protected:
    TextFrame text, key, cipher_text;
    cryptButton buttons;

    void crypt_process();
    void clear_buffer();
    int char2int(char c);
    Config* config_ptr;
};

#endif