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

    void launch_compile(const smce::SketchSource& ino_path, const std::filesystem::path& smce_home);

  public:
    std::pair<BoardData, BoardInfo> board;
    enum class CompileResult { SUCCESS, FAILURE };
    static void _register_methods() {
        register_method("_process", &EmulGlue::_process);
        register_method("_ready", &EmulGlue::_ready);
        register_method("has_board", &EmulGlue::has_board);
        register_method("compile", &EmulGlue::compile);
        register_method("write_uart", &EmulGlue::write_uart);
        register_method("write_uart_n", &EmulGlue::write_uart_n);
        register_method("get_uart_buf_n", &EmulGlue::get_uart_buf_n);


        register_signal<EmulGlue>("compile_done", "successful", GODOT_VARIANT_TYPE_BOOL, "message", GODOT_VARIANT_TYPE_STRING);
        register_signal<EmulGlue>("board_started", Dictionary());
        register_signal<EmulGlue>("board_resumed", Dictionary());
        register_signal<EmulGlue>("board_paused", Dictionary());
        register_signal<EmulGlue>("board_stopped", Dictionary());


    }

    void _init();
    void _ready();
    void _process(float delta);
    bool compile(const String ino_path);

    ~EmulGlue() { Godot::print("glue died"); }

    bool has_board() const { return ino_runtime.is_running(); }
    bool bus_exists(size_t bus) const { return has_board() && bus > board.first.uart_buses.size(); }

    bool write_uart(String msg) { return write_uart_n(msg, 0); }
    bool write_uart_n(String msg, unsigned int bus);

    String get_uart_buf_n(unsigned int bus);
    void pause_board() { ino_runtime.pause_now(); }
    void resume_board() { ino_runtime.resume(); }
    void reset_board() {}
};

} // namespace godot
#endif // SMARTCAR_EMUL_EMULGLUE_HPP
