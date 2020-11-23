#include <catch2/catch.hpp>
#include "VehicleConf.hxx"

TEST_CASE("Read a valid vehicle config", "[VehicleConf]") {
    rapidjson::Document doc;
    doc.Parse(R"(
{
    "hull_model_file": "/home/dave/Urho3D-models/car_hull.mdl",
    "parts": {
        "wheel": {
            "position": {"x": 0.0, "y": 4, "z": -1},
            "rotation": {"x": 0, "y": 90, "z": 0},
            "model_file": "/home/dave/Urho3D-models/car_wheel.mdl"
        },
        "fan": {
            "position": {"x": -0.76, "y": 0.1, "z": 0.4},
            "rotation": {"x": 0, "y": 90, "z": 180},
            "model_file": "/home/dave/Urho3D-models/car_fan.mdl",
            "model_position_offset": {"x": 0, "y": 0.1, "z": 0},
            "model_rotation_offset": {"x": 45, "y": 90, "z": 0}
        }
    },
    "attachments": {
        "ServoMotor": {
            "pin": 4,
            "rotation": "continuous",
            "max_speed": 450
        }
    }
}
)");
    REQUIRE(!doc.HasParseError());
    const auto conf_opt = smce::VehicleConfig::load(std::move(doc));
    REQUIRE(conf_opt);
    const auto& conf = *conf_opt;

    REQUIRE(conf.attachments.size() == 1);
    REQUIRE(conf.attachments[0].first == "ServoMotor");
    REQUIRE(conf.attachments[0].second->IsObject());
}

TEST_CASE("Report parsing failure", "[VehicleConf]") {
    rapidjson::Document doc;
    doc.Parse("{}");
    REQUIRE(!doc.HasParseError());
    const auto conf_opt = smce::VehicleConfig::load(std::move(doc));
    REQUIRE(!conf_opt);
}