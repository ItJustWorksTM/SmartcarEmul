#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <Sprite3D.hpp>
#include <atomic>
#include <RayCast.hpp>

namespace godot {

class GDExample : public RayCast {
    GODOT_CLASS(GDExample, Sprite3D)

    float time_passed = 0.0;
    std::atomic_uint16_t* m_pin_ptr;

public:
    int testing;
    static void _register_methods() {
        register_method("_process", &GDExample::_process);
        register_method("_ready", &GDExample::_ready);
        register_property("testing", &GDExample::testing, 100);
    }

    GDExample() = default;

    void _init();
    void _ready();
    void _process(float delta);
};

}

#endif

