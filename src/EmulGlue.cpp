#include "gdnative/EmulGlue.hxx"
#include <Ref.hpp>

namespace godot {
void EmulGlue::_init() { Godot::print("EmulGlue initted"); }

void EmulGlue::_ready() { Godot::print("EmulGlue ready"); }

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

bool EmulGlue::compile(const String _ino_path) {

    if (compile_tr) {
        Godot::print("Already compiling!");
        return false;
    }

    auto ino_path = std::string{_ino_path.utf8().get_data()};

    std::filesystem::path smce_home = "/home/ruthgerd/Sources/SmartcarEmul/cmake-build-debug";
    std::filesystem::path b_conf_path = "/home/ruthgerd/board_config.json";

    constexpr auto paths_exist = [](auto... t) { return (... && std::filesystem::exists(t)); };
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

bool EmulGlue::write_uart_n(String msg, unsigned int bus) {
    if (!bus_exists(bus))
        return false;

    auto& ubus = board.first.uart_buses[bus];
    std::scoped_lock lk{ubus.rx_mutex};

    auto buf = msg.utf8();
    auto view = std::string_view{buf.get_data(), static_cast<size_t>(buf.length())};

    auto size = ubus.rx.size();
    ubus.rx.resize(ubus.rx.size() + view.size());

    std::memcpy(&*ubus.rx.begin() + size, view.data(), view.size());
    return true;
}

String EmulGlue::get_uart_buf_n(unsigned int bus) {
    if (!bus_exists(bus))
        return {};
    auto ret = std::string();
    auto& ubus = board.first.uart_buses[bus];
    std::scoped_lock lk{ubus.rx_mutex};
    ret.resize(ubus.rx.size());
    std::memcpy(ret.data(), ubus.rx.data(), ubus.rx.size());

    return ret.c_str();
}
} // namespace godot
