#include <GtkDecrypt.h>
#include <Des.h>
#include <Aes.h>
using namespace std;
using namespace Gtk;



Decrypt::Decrypt(Config* config_ptr, Orientation orientation, int spacing) 
:
    Gtk::Box(orientation, spacing),
    text("明文"),
    key("秘钥"),
    cipher_text("密文"),
    buttons("解密"),
    config_ptr(config_ptr)
{
    pack_start(cipher_text);
    pack_start(key);
    pack_start(text);
    pack_start(buttons);
    // show_all_children();

    buttons.but1.signal_clicked().connect(sigc::mem_fun(*this, 
        &Decrypt::crypt_process));
    buttons.but2.signal_clicked().connect(sigc::mem_fun(*this,
        &Decrypt::clear_buffer));
}

Decrypt::~Decrypt() {

}

int Decrypt::char2int(char c) {
    if(isdigit(c)) {
        return c - '0';
    }
    else if(c <= 'f' and c >= 'a') {
        return c - 'a' + 10;
    }
    else if(c >= 'A' and c <= 'F') {
        return c - 'A' + 10;
    }
    else {
        throw "not 0-9 | a-z | A-Z ";
    }
}

void Decrypt::crypt_process() {
    string num16 = cipher_text.get_text();
    string k = key.get_text();
    string alg = config_ptr->getString("algorithm");
    if(alg == "DES") {
        string t = "";
        for(int i = 0; i < num16.size(); ) {
            char c = 0;
            char cc = 0;
            try{
                c = char2int(num16[i++]);
                c <<= 4;
                c += char2int(num16[i++]);
                for(int i = 0; i < 8; i++) {
                    cc = (cc << 1) | (c & 1);
                    c >>= 1;
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            t += cc;
        }
        string res = Des::process(t, k, -1);
        string show = "";
        for(auto s : res) {
            if(s > 0) show += s;
            else break;
        }
        text.set_text(show);
    }
    else if(alg == "AES") {
        Aes aes;
        string res = aes.processInv(num16, k);
        string show = "";
        for(auto s : res) {
            if(s > 0) show += s;
            else break;
        }
        text.set_text(show);
    }
    else {
        printf("\033[1;32mAlgorithm para error, %s\n\033[0m", alg.c_str());
    }
}

void Decrypt::clear_buffer() {
    text.clear();
    key.clear();
    cipher_text.clear();
}



