#include "gdnative/EmulGlue.hxx"
#include "gdnative/components/AnalogRayCast.hxx"

using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    Godot::gdnative_terminate(o);
}

template<class ...T>
void register_classes() { (register_class<T>(), ...); };

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);
    register_classes<EmulGlue, AnalogRayCast>();
}

