#include <Label.hpp>
#include "gdnative/gdexample.hxx"
#include "gdnative/util.hxx"
#include "gdnative/EmulGlue.hxx"

using namespace godot;
void GDExample::_init() {
    Godot::print("GDExample initted");
}

void GDExample::_ready() {
    auto* single = gd_cast<EmulGlue>(get_node_or_null("/root/EmulGlue"));
    if (single) {
        single->talk();
    }
}

void GDExample::_process(float delta) {
    time_passed += delta;
}

