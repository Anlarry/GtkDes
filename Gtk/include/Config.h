#ifndef __CONFIG_H
#define __CONFIG_H

#include <gtkmm-3.0/gtkmm.h>
#include <string>
#include <rapidjson/document.h>
using namespace std;


using namespace Gtk;

class Config : public Gtk::Box 
{
public:
    Config(string config_path="config.json",
        Orientation orientation = ORIENTATION_HORIZONTAL,int spacing=0
    );
    string getString(const string &s);
protected:
    void on_combo_changed();
    string config_path;
    Gtk::Label alg_label;
    Gtk::ComboBoxText alg_combo;
    Gtk::Grid grid;
};

#endif