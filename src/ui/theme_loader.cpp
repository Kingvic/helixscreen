// Copyright (C) 2025-2026 356C LLC
// SPDX-License-Identifier: GPL-3.0-or-later

#include "theme_loader.h"

#include <spdlog/spdlog.h>

#include <stdexcept>

namespace helix {

const std::array<const char*, 16>& ThemePalette::color_names() {
    static const std::array<const char*, 16> names = {
        "bg_darkest",     "bg_dark",          "bg_dark_highlight", "border_muted",
        "text_light",     "bg_light",         "bg_lightest",       "accent_highlight",
        "accent_primary", "accent_secondary", "accent_tertiary",   "status_error",
        "status_danger",  "status_warning",   "status_success",    "status_special"};
    return names;
}

const std::string& ThemePalette::at(size_t index) const {
    switch (index) {
    case 0:
        return bg_darkest;
    case 1:
        return bg_dark;
    case 2:
        return bg_dark_highlight;
    case 3:
        return border_muted;
    case 4:
        return text_light;
    case 5:
        return bg_light;
    case 6:
        return bg_lightest;
    case 7:
        return accent_highlight;
    case 8:
        return accent_primary;
    case 9:
        return accent_secondary;
    case 10:
        return accent_tertiary;
    case 11:
        return status_error;
    case 12:
        return status_danger;
    case 13:
        return status_warning;
    case 14:
        return status_success;
    case 15:
        return status_special;
    default:
        throw std::out_of_range("ThemePalette index out of range");
    }
}

std::string& ThemePalette::at(size_t index) {
    return const_cast<std::string&>(static_cast<const ThemePalette*>(this)->at(index));
}

bool ThemeData::is_valid() const {
    // Check all colors are non-empty and start with #
    for (size_t i = 0; i < 16; ++i) {
        const auto& color = colors.at(i);
        if (color.empty() || color[0] != '#' || color.size() != 7) {
            return false;
        }
    }
    return !name.empty();
}

} // namespace helix
