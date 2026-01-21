// Copyright (C) 2025-2026 356C LLC
// SPDX-License-Identifier: GPL-3.0-or-later

#include "theme_loader.h"

#include "../catch_amalgamated.hpp"

using namespace helix;

TEST_CASE("ThemePalette index access", "[theme]") {
    ThemePalette palette;
    palette.bg_darkest = "#2e3440";
    palette.status_special = "#b48ead";

    REQUIRE(palette.at(0) == "#2e3440");
    REQUIRE(palette.at(15) == "#b48ead");
}

TEST_CASE("ThemePalette color_names returns all 16 names", "[theme]") {
    auto& names = ThemePalette::color_names();
    REQUIRE(names.size() == 16);
    REQUIRE(std::string(names[0]) == "bg_darkest");
    REQUIRE(std::string(names[15]) == "status_special");
}
