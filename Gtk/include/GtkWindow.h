#ifndef __GTKWINDOW_H
#define __GTKWINDOW_H

#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <GtkEncrypt.h>
#include <GtkDecrypt.h>
#include <Config.h>
#include <string>
using namespace std;

class MainWindow : public Gtk::Window
{   
public :
    MainWindow (string config_path="config.json") ;
    virtual ~MainWindow();

protected:
    void on_notebook_switch_page(Gtk::Widget *page, guint page_number); 

    Gtk::Box vbox;

    Gtk::Notebook notebook;
    // Gtk::Label label1, label2;

    Encrypt page1;
    Decrypt page2;
    Config  config_page;

};   

#endif