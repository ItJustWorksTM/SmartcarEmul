#ifndef SMARTCAR_EMUL_EMULGLUE_HPP
#define SMARTCAR_EMUL_EMULGLUE_HPP
#include <atomic>
#include <filesystem>
#include <string>
#include <BoardData.hxx>
#include <BoardInfo.hxx>
#include <Godot.hpp>
#include <Label.hpp>
#include <Runtime.hxx>
#include <Source.hxx>
#include <VehicleConf.hxx>
#include <fmt/format.h>
#include <utility.hxx>
#include "gdnative/util.hxx"
#include "BoardConf.hxx"
#include "Toolchain.hxx"
#include "VehicleConf.hxx"

namespace godot {

class EmulGlue : public Node {
    GODOT_CLASS(EmulGlue, Node);

    std::atomic<bool> compile_done = false;
    smce::SketchRuntime ino_runtime{};
    std::optional<std::future<smce::CompilationResults>> compile_tr{};

    void compile(const smce::SketchSource& ino_path, const std::filesystem::path& smce_home);

  public:
    std::pair<BoardData, BoardInfo> board;
    enum class CompileResult { SUCCESS, FAILURE };
    static void _register_methods() {
        register_method("_process", &EmulGlue::_process);
        register_method("_ready", &EmulGlue::_ready);
        register_method("is_board_available", &EmulGlue::is_board_available);
        register_method("start_compile", &EmulGlue::start_compile);
        register_method("write_uart", &EmulGlue::write_uart);
        register_method("write_uart_n", &EmulGlue::write_uart_n);
        register_method("get_uart_buf_n", &EmulGlue::get_uart_buf_n);


        register_signal<EmulGlue>("compile_done", "successful", GODOT_VARIANT_TYPE_BOOL, "message", GODOT_VARIANT_TYPE_STRING);
    }

    void _init();
    void _ready();
    void _process(float delta);
    bool start_compile(const String ino_path);

    ~EmulGlue() { Godot::print("glue died"); }

    bool is_board_available() { return ino_runtime.is_running(); }

    bool write_uart(String msg) { return write_uart_n(msg, 0); }


    bool bus_exists(size_t bus) {
        if (!is_board_available()) {
            Godot::print_warning("Written to uart without active board", "EmulGlue::write_uart", "EmulGlue.hxx", 0);
        } else if (bus > board.first.uart_buses.size()) {
            Godot::print_warning(
                fmt::format("Written to non existant uart bus {}, max bus available: {}", bus, board.first.uart_buses.size()).c_str(),
                "EmulGlue::write_uart", "EmulGlue.hxx", 0);
        } else {
            return true;
        }
        return false;
    }

    bool write_uart_n(String msg, size_t bus) {
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

    // TODO: use PoolByteArray somehow (You can't get a ptr to its data for memcpy)
    String get_uart_buf_n(size_t bus) {
        if (!bus_exists(bus))
            return {};
        auto ret = std::string();
        auto& ubus = board.first.uart_buses[bus];
        std::scoped_lock lk{ubus.rx_mutex};
        ret.resize(ubus.rx.size());
        std::memcpy(ret.data(), ubus.rx.data(), ubus.rx.size());

        return ret.c_str();
    }
};

} // namespace godot
#endif // SMARTCAR_EMUL_EMULGLUE_HPP
