#pragma once

#include <array>
#include <string_view>

enum class Color
{
    White,
    Black
};

constexpr std::array<std::string_view, 2> colorNames = {
    "White",
    "Black"
}; // permanent array of strings, indexed by Color enum values (the order must always match the enum)