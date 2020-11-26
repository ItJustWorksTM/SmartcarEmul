#ifndef SMARTCAR_EMUL_UTIL_HXX
#define SMARTCAR_EMUL_UTIL_HXX
#include <filesystem>
#include <optional>
#include <utility>
#include <BoardConf.hxx>
#include <BoardData.hxx>
#include <BoardInfo.hxx>
#include <Godot.hpp>

template<typename T>
T* gd_cast(const godot::Object* variant) {
    return godot::Object::cast_to<T>(variant);
}


std::optional<std::pair<BoardData, BoardInfo>> make_config(const std::filesystem::path& path);

#endif // SMARTCAR_EMUL_UTIL_HXX
