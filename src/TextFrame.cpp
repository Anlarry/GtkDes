#include<TextFrame.h>
#include <gtkmm-3.0/gtkmm.h>
#include <iostream>
using namespace std;
using namespace Gtk;

Text::Text() {
    set_border_width(0);
    set_size_request(300, 100);
    add(textview);
}

Text::~Text() {

}

void Text::insert(const string & str) {
    auto buffer = this->textview.get_buffer();
    auto iter = buffer->get_iter_at_offset(0);
    buffer->insert(iter, str);
}

void Text::clear() {
    auto buffer = this->textview.get_buffer();
    buffer->set_text("");
}

string Text::get_text() {
    auto buffer = this->textview.get_buffer();
    return buffer->get_text();
}

void Text::set_text(const string &s) {
    auto buffer = this->textview.get_buffer();
    buffer->set_text(s);
}

TextFrame::TextFrame(const string &label) {
    this->set_label(label);
    this->set_label_align(ALIGN_START, ALIGN_START);
    this->set_shadow_type(SHADOW_ETCHED_IN);

    Gtk::Box * box = nullptr;
    set_border_width(5);
    box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);

    box->set_border_width(5);
    add(*box);
    box->add(text);
    // box->add(button);
    // show_all_children();
}

TextFrame::~TextFrame() {

}

void TextFrame::insert(const string &str) {
    this->text.insert(str);
}

void TextFrame::clear() {
    this->text.clear();
}

string TextFrame::get_text() {
    return this->text.get_text();
}

void TextFrame::set_text(const string &s) {
    this->text.set_text(s);
}