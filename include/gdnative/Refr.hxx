#ifndef SMARTCAR_EMUL_REFR_HXX
#define SMARTCAR_EMUL_REFR_HXX

#include <Godot.hpp>
#include <Sprite3D.hpp>
#include <atomic>
#include <RayCast.hpp>

namespace godot {

class Refr : public Reference {
  GODOT_CLASS(Refr, Reference)

  public:
    int testing;
    static void _register_methods() {
        register_property("testing", &Refr::testing, 100);
    }

    ~Refr() {
        Godot::print("Refr diedd!");
    }

    void _init() {

    }
};

}


#endif // SMARTCAR_EMUL_REFR_HXX
