#include "GtkWindow.h"
#include <Debug.h>
#include <bits/stdc++.h>
// #include <gtkmm/application.h>

// #ifdef DEBUG
//   #define AppWindow DebugWindow
// #else
//   #define AppWindow MainWindow
// #endif

#include <gtkmm-3.0/gtkmm/application.h>
#include <Des.h>

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "DES");

  MainWindow window;
  
  //Shows the window and returns when it is closed.
  return app->run(window);
}