/*
 *  utility.hxx
 *  Copyright 2020 AeroStun
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef SMARTCAR_EMUL_INCLUDE_UTILITY_HXX
#define SMARTCAR_EMUL_INCLUDE_UTILITY_HXX

#define M_PI 3.14159265358979323846

#include <algorithm>
#include <cmath>
#include <concepts>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

using DynamicBitset = std::vector<bool>;

template <class T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <class CharT = char>
[[nodiscard]] std::basic_string<CharT> slurp_file(const std::filesystem::path& filepath) {
    std::basic_ifstream<CharT> file_stream{filepath};
    std::basic_string<CharT> ret;
    file_stream.seekg(0, std::ios::end);
    ret.resize(static_cast<long>(file_stream.tellg()) + 1);
    file_stream.seekg(0, std::ios::beg);
    std::copy(std::istream_iterator<CharT>{file_stream}, std::istream_iterator<CharT>{}, ret.begin());
    return ret;
}

template <Numeric From, Numeric To>
[[nodiscard]] To math_map(From old_value, From old_bottom, From old_top, To new_bottom, To new_top) {
    if constexpr (std::is_floating_point_v<To>)
        return  (static_cast<To>(old_value) - old_bottom) / (old_top - old_bottom) * (new_top - new_bottom) + new_bottom;
    return  (old_value - old_bottom) / (old_top - old_bottom) * (new_top - new_bottom) + new_bottom;
}

template <std::floating_point F>
constexpr F deg2rad(F deg) noexcept { return deg * M_PI / 180.0f; }

#endif // SMARTCAR_EMUL_INCLUDE_UTILITY_HXX