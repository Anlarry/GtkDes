#include <Gtk.h>
#include <Algorithm.h>
#include <bits/stdc++.h>
#include <gtkmm-3.0/gtkmm/application.h>
using namespace std;

int main(int argc, char *argv[])
{
// auto app = Gtk::Application::create(argc, argv, "DES");
//
// MainWindow window;
//
// //Shows the window and returns when it is closed.
// return app->run(window);
    BigNum a = 10;
    BigNum b = 253;
    cout << BigNum(2).powerMod((BigNum)10, 10000000) << "\n";
    auto c = a * b;
    cout << a * b << "\n";
    Prime prime_cls = Prime();
    // BigNum q = prime_cls.genPrime(16, Prime::Option::MR_Random);
    // cout << q << "\n";
    BigNum p = prime_cls.genPrime(64, Prime::Option::MR_Random);
    cout << p << "\n";
    // for(BigNum i = 2; i * i <= p; i = i+ 1) {
    //     if(p % i == 0) {
    //         cout << "false: " << i <<  "\n";
    //     }
    //     cout << i << "\r";
    // }
    return 0;
}