#include "gdnative/util.hxx"

std::optional<std::pair<BoardData, BoardInfo>> make_config(const std::filesystem::path& path) {
    auto vehicle_conf_opt = smce::load(path);
    if (!vehicle_conf_opt)
        return std::nullopt;

    auto bdata = smce::as_data(*vehicle_conf_opt);
    auto binfo = smce::as_info(*vehicle_conf_opt);

    bdata.write_byte = +[](unsigned char c) {
      godot::Godot::print(std::string(reinterpret_cast<const char*>(&c), 1).c_str());
      return true;
    };

    bdata.write_buf = +[](const unsigned char* buf, std::size_t len) {
      godot::Godot::print(std::string(reinterpret_cast<const char*>(buf), len).c_str());
      return len;
    };

    return std::make_pair(std::move(bdata), std::move(binfo));
}
