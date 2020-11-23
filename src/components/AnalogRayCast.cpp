#include <Label.hpp>
#include "gdnative/components/AnalogRayCast.hxx"
#include "gdnative/util.hxx"
#include "gdnative/EmulGlue.hxx"

using namespace godot;

void AnalogRayCast::_init() {
    Godot::print("AnalogRayCast initted");
}

void AnalogRayCast::setup(std::atomic_uint16_t* ptr) {
    pin_ptr = ptr;
}

void AnalogRayCast::_ready() {

}

void AnalogRayCast::_process(float delta) {
    if (is_colliding()) {
        auto* collider = gd_cast<Spatial>(get_collider());

        if (pin_ptr && collider) {
            uint16_t dist = get_global_transform().origin.distance_to(collider->get_global_transform().origin);
            pin_ptr->store(dist);
        }

    }
}

