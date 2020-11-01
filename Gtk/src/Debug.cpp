#include <Debug.h>
using namespace Gtk;
using namespace std;

DebugWindow::DebugWindow () : text("123") {

    set_border_width(10);
    set_default_size(500, 500);

    add(text);
    // add(view);
    // auto buffer = view.get_buffer();
    // auto iter = buffer->get_iter_at_offset(0);
    // buffer->insert(iter, "aaa");

    show_all_children();
}

DebugWindow::~DebugWindow() {

}

