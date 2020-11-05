#include <Config.h>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <iostream>
using namespace rapidjson;
using namespace std;

Config::Config(string config_path,
        Orientation orientation, int spacing) :
    Gtk::Box(orientation, spacing),
    config_path(config_path),
    alg_label("Algorithem")
{

    alg_combo.append("DES");
    alg_combo.append("AES");
    set_border_width(5);
    ifstream fin(config_path);
    Document d;
    if(fin.good()) {
        IStreamWrapper isw(fin);
        d.ParseStream(isw);
        assert(d.HasMember("algorithm"));
        if(d["algorithm"] == "DES") {
            alg_combo.set_active(1);
        }
        else if(d["algorithm"] == "AES"){
            alg_combo.set_active(2);
        }
    }
    else {
        d.SetObject();
        Value v;
        v.SetString("DES");
        d.AddMember("algorithm", v, d.GetAllocator());
        ofstream ofs(config_path);
        OStreamWrapper osw(ofs);
        Writer<OStreamWrapper> writer(osw);
        d.Accept(writer);
        alg_combo.set_active(1);
    }
    grid.set_column_homogeneous(true);
    add(grid);
    grid.set_border_width(5);
    grid.attach(alg_label, 1, 1, 1, 1);
    grid.attach(alg_combo, 6, 1, 1, 1);

    alg_combo.signal_changed().connect(sigc::mem_fun(*this, 
        &Config::on_combo_changed));
}

void Config::on_combo_changed() {
    string text = alg_combo.get_active_text();
    if(!text.empty()) {
        cout << "algorithm change to " << text << "\n";
        Document d;
        ifstream fin(config_path);
        IStreamWrapper isw(fin);
        d.ParseStream(isw);
        Value &s = d["algorithm"];
        s.SetString(text.c_str(), text.size());
        ofstream ofs(config_path);
        OStreamWrapper osw(ofs);
        Writer<OStreamWrapper> writer(osw);
        d.Accept(writer);
    }
}

string Config::getString(const string& s) {
    Document d;
    ifstream fin(config_path);
    IStreamWrapper isw(fin);
    d.ParseStream(isw);
    Value& res = d[s.c_str()];
    return string(res.GetString());
}   