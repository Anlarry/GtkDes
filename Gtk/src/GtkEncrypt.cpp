#include <GtkEncrypt.h>
#include <iostream>
#include <Des.h>
#include <Aes.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

using namespace std;
using namespace Gtk;
using namespace rapidjson;


Encrypt::Encrypt(Config* config_ptr, Orientation orientation , int spacing) 
: 
    Gtk::Box(orientation, spacing),
    text("明文"),
    key("秘钥"),
    cipher_text("密文"),
    buttons("加密"),
    config_ptr(config_ptr)
{
    pack_start(text);
    pack_start(key);
    pack_start(cipher_text);
    pack_start(buttons);

    buttons.but1.signal_clicked().connect(sigc::mem_fun(*this, 
        &Encrypt::crypt_process));
    buttons.but2.signal_clicked().connect(sigc::mem_fun(*this,
        &Encrypt::clear_buffer));
}

Encrypt::~Encrypt() {
}


void Encrypt::crypt_process() {
    string t = text.get_text();
    string k = key.get_text();
    // cout << t << "\n" << k << "\n";
    string res;
    string alg = config_ptr->getString("algorithm");
    if(alg == "DES")  {
        res = Des::process(t, k, 0);
    }
    else if(alg == "AES"){
        Aes aes;
        res = aes.process(t, k);
    }
    else {
        printf("\033[1;32mAlgorithm para error, %s\n\033[0m", alg.c_str());
    }
    cipher_text.set_text(res);
}

void Encrypt::clear_buffer() {
    text.clear();
    key.clear();
    cipher_text.clear();
}


