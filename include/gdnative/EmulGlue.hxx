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
#include "BoardConf.hxx"
#include "Toolchain.hxx"
#include "VehicleConf.hxx"
#include "gdnative/util.hxx"

namespace godot {



class EmulGlue : public Node {
    GODOT_CLASS(EmulGlue, Node);

    std::atomic<bool> compile_done = false;
    smce::SketchRuntime ino_runtime{};
    std::optional<std::future<smce::CompilationResults>> compile_tr{};


    void compile(const smce::SketchSource& ino_path, const std::filesystem::path& smce_home);
  public:
    std::pair<BoardData, BoardInfo> board;
    enum class CompileResult {
        SUCCESS,
        FAILURE
    };
    static void _register_methods() {
        register_method("_process", &EmulGlue::_process);
        register_method("_ready", &EmulGlue::_ready);
        register_method("start_compile", &EmulGlue::start_compile);
        register_signal<EmulGlue>("compile_done", "successful", GODOT_VARIANT_TYPE_BOOL, "message",
                                  GODOT_VARIANT_TYPE_STRING);
    }

    void _init();
    void _ready();
    void _process(float delta);
    bool start_compile(const String ino_path);

    void talk() { Godot::print("hello from the singleton!"); }

};

} // namespace godot
#endif // SMARTCAR_EMUL_EMULGLUE_HPP
