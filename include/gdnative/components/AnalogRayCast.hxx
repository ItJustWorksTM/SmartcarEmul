#ifndef SMARTCAR_EMUL_ANALOGRAYCAST_HXX
#define SMARTCAR_EMUL_ANALOGRAYCAST_HXX

#include <Godot.hpp>
#include <Sprite3D.hpp>
#include <atomic>
#include <RayCast.hpp>

namespace godot {

class AnalogRayCast : public RayCast {
  GODOT_CLASS(AnalogRayCast, Sprite3D)

    float time_passed = 0.0;
    std::atomic_uint16_t* pin_ptr;

  public:
    int testing;
    static void _register_methods() {
        register_method("_process", &AnalogRayCast::_process);
        register_method("_ready", &AnalogRayCast::_ready);
        register_property("testing", &AnalogRayCast::testing, 100);
    }

    void _init();
    void setup(std::atomic_uint16_t* ptr);
    void _ready();
    void _process(float delta);
};

}

#endif // SMARTCAR_EMUL_ANALOGRAYCAST_HXX
