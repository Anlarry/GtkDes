#include <iostream>
#include <GtkWindow.h>
using namespace std;

MainWindow::MainWindow()
    :vbox(Gtk::ORIENTATION_VERTICAL), page1(Gtk::ORIENTATION_VERTICAL), 
                                      page2(Gtk::ORIENTATION_VERTICAL)
{
    set_title("DES");
    set_border_width(10);
    set_default_size(700,  500);

    add(vbox);

    notebook.set_border_width(0);
    vbox.pack_start(notebook);
    
    notebook.append_page(page1, "加密");
    notebook.append_page(page2, "解密");

    notebook.signal_switch_page().connect(sigc::mem_fun(*this,
        &MainWindow::on_notebook_switch_page));

    show_all_children();
}

MainWindow::~MainWindow() {

}

void MainWindow::on_notebook_switch_page(Gtk::Widget*, guint page_num) {
    
}