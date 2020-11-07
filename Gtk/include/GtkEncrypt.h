#ifndef __GTKENCRYPT_H
#define __GTKENCRYPT_H

#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <TextFrame.h>
#include <cryptButton.h>
#include <Config.h>
using namespace Gtk;



class Encrypt : public Gtk::Box 
{
public:
    Encrypt(Config* config_ptr,Orientation orientation = ORIENTATION_HORIZONTAL, int spacing = 0);
    virtual  ~Encrypt ();
protected:
    TextFrame text, key, cipher_text;
    cryptButton buttons;

    Config* config_ptr;

    void crypt_process();
    void clear_buffer();

};

#endif