#include "gdnative/EmulGlue.hxx"
#include <Ref.hpp>

namespace godot {
void EmulGlue::_init() { Godot::print("EmulGlue initted"); }

void EmulGlue::_ready() { Godot::print("EmulGlue ready"); }

// TODO: move out of class
void EmulGlue::compile(const smce::SketchSource& ino_path, const std::filesystem::path& smce_home) {
    compile_tr = std::async([&, ino_path, smce_home]() {
        auto ret = smce::compile_sketch(ino_path, smce_home);
        compile_done = true;
        return ret;
    });
}

void EmulGlue::_process(float delta) {
    if (compile_done) {
        auto compile_result = compile_tr->get();
        auto err = std::visit(Visitor{
                                  [&](smce::SketchObject& obb) -> std::optional<std::runtime_error> {
                                      ino_runtime.set_sketch_and_car(obb, board.first, board.second);
                                      if (!ino_runtime.start())
                                          return std::runtime_error{"Sketch could not be run"};
                                      Godot::print("compile completed");
                                      return std::nullopt;
                                  },
                                  [&](std::runtime_error& err) -> std::optional<std::runtime_error> { return err; },
                              },
                              compile_result);
        if (err) {
            Godot::print(fmt::format("Compile failed: {}", err->what()).c_str());
            emit_signal("compile_done", false, fmt::format("Compile failed: {}", err->what()).c_str());
        } else {
            emit_signal("compile_done", true, "Something happened!");
        }

        compile_done = false;
        compile_tr.reset();
    }
}

bool EmulGlue::start_compile(const String _ino_path) {

    constexpr auto paths_exist = [](auto... t) { return (... && std::filesystem::exists(t)); };
    if (compile_tr) {
        Godot::print("Already compiling!");
        return false;
    }

    auto ino_path = std::string{_ino_path.utf8().get_data()};

    std::filesystem::path smce_home = "/home/ruthgerd/Sources/SmartcarEmul/cmake-build-debug";
    std::filesystem::path b_conf_path = "/home/ruthgerd/board_config.json";

    if (!paths_exist(smce_home, b_conf_path, ino_path)) {
        Godot::print("paths dont exist");
        return false;
    }

    auto result = make_config(b_conf_path);
    if (!result) {
        Godot::print("config failed");
        return false;
    }

    board = std::move(*result);
    compile_tr = std::async([&, ino_path, smce_home]() {
      auto ret = smce::compile_sketch({ino_path}, smce_home);
      compile_done = true;
      return ret;
    });



    return true;
}
} // namespace godot
