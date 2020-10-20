#ifndef __TEXTFRAME_H
#define __TEXTFRAME_H

#include <gtkmm-3.0/gtkmm.h>
#include <iostream>
#include <sstream>
using namespace Gtk;
using namespace std;

class Text : public Gtk::ScrolledWindow 
{
public: 
    Text() ;
    ~Text() ;
    void insert(const string &str);
    void clear();
    string get_text();
    void set_text(const string &s);
protected:
    Gtk::TextView textview;
};

class TextFrame : public Gtk::Frame 
{
public:
    TextFrame(const string &label) ;
    ~TextFrame();
    void clear();
    void insert(const string &str) ;
    string get_text();
    void set_text(const string &s);
protected:
    Text text;
};
#endif