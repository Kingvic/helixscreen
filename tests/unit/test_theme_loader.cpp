// Copyright (C) 2025-2026 356C LLC
// SPDX-License-Identifier: GPL-3.0-or-later

#include "theme_loader.h"

#include <cstdio>
#include <sys/stat.h>

#include "../catch_amalgamated.hpp"

using namespace helix;

TEST_CASE("ModePalette index access", "[theme]") {
    ModePalette palette;
    palette.app_bg = "#2e3440";
    palette.focus = "#8fbcbb";

    REQUIRE(palette.at(0) == "#2e3440");
    REQUIRE(palette.at(15) == "#8fbcbb");
}

TEST_CASE("ModePalette color_names returns all 16 names", "[theme]") {
    auto& names = ModePalette::color_names();
    REQUIRE(names.size() == 16);
    REQUIRE(std::string(names[0]) == "app_bg");
    REQUIRE(std::string(names[15]) == "focus");
}

TEST_CASE("ThemeData::is_valid checks palettes and name", "[theme]") {
    ThemeData theme;
    theme.name = "Test";

    // Set all 16 colors in dark palette to valid hex
    for (size_t i = 0; i < 16; ++i) {
        theme.dark.at(i) = "#aabbcc";
    }

    REQUIRE(theme.is_valid());

    // Empty name should be invalid
    theme.name = "";
    REQUIRE_FALSE(theme.is_valid());
    theme.name = "Test";

    // Invalid color format should fail
    theme.dark.app_bg = "invalid";
    REQUIRE_FALSE(theme.is_valid());

    // Short hex should fail
    theme.dark.app_bg = "#abc";
    REQUIRE_FALSE(theme.is_valid());
}

TEST_CASE("ModePalette::at throws on invalid index", "[theme]") {
    ModePalette palette;
    REQUIRE_THROWS_AS(palette.at(16), std::out_of_range);
    REQUIRE_THROWS_AS(palette.at(100), std::out_of_range);
}

TEST_CASE("parse_theme_json parses valid dual-palette theme", "[theme]") {
    const char* json = R"({
        "name": "Test Theme",
        "dark": {
            "app_bg": "#2e3440",
            "panel_bg": "#3b4252",
            "card_bg": "#434c5e",
            "card_alt": "#4c566a",
            "border": "#616e88",
            "text": "#eceff4",
            "text_muted": "#d8dee9",
            "text_subtle": "#b8c2d1",
            "primary": "#88c0d0",
            "secondary": "#81a1c1",
            "tertiary": "#5e81ac",
            "info": "#b48ead",
            "success": "#a3be8c",
            "warning": "#ebcb8b",
            "danger": "#bf616a",
            "focus": "#8fbcbb"
        },
        "border_radius": 8,
        "border_width": 2,
        "border_opacity": 50,
        "shadow_intensity": 10
    })";

    auto theme = helix::parse_theme_json(json, "test.json");

    REQUIRE(theme.name == "Test Theme");
    REQUIRE(theme.dark.app_bg == "#2e3440");
    REQUIRE(theme.dark.focus == "#8fbcbb");
    REQUIRE(theme.properties.border_radius == 8);
    REQUIRE(theme.properties.shadow_intensity == 10);
    REQUIRE(theme.is_valid());
}

TEST_CASE("get_default_nord_theme returns valid theme", "[theme]") {
    auto theme = helix::get_default_nord_theme();

    REQUIRE(theme.name == "Nord");
    REQUIRE(theme.is_valid());
    REQUIRE(theme.dark.app_bg == "#2e3440");
    REQUIRE(theme.light.app_bg == "#eceff4");
}

TEST_CASE("parse_theme_json returns Nord on invalid JSON", "[theme]") {
    auto theme = helix::parse_theme_json("{ invalid json", "bad.json");

    REQUIRE(theme.name == "Nord");
    REQUIRE(theme.is_valid());
}

TEST_CASE("parse_theme_json returns Nord when missing palettes", "[theme]") {
    // JSON with no dark or light palette should fall back to Nord
    const char* json = R"({
        "name": "Invalid Theme",
        "border_radius": 8
    })";

    auto theme = helix::parse_theme_json(json, "missing_palettes.json");

    REQUIRE(theme.name == "Nord");
    REQUIRE(theme.is_valid());
}

TEST_CASE("save_theme_to_file and load_theme_from_file roundtrip", "[theme]") {
    auto original = helix::get_default_nord_theme();
    original.name = "Roundtrip Test";
    original.properties.border_radius = 20;

    std::string path = "/tmp/test_theme_roundtrip.json";
    REQUIRE(helix::save_theme_to_file(original, path));

    auto loaded = helix::load_theme_from_file(path);

    REQUIRE(loaded.name == "Roundtrip Test");
    REQUIRE(loaded.properties.border_radius == 20);
    REQUIRE(loaded.dark.app_bg == original.dark.app_bg);
    REQUIRE(loaded.light.app_bg == original.light.app_bg);
    REQUIRE(loaded.is_valid());

    // Cleanup
    std::remove(path.c_str());
}

// ============================================================================
// Two-Tier Theme Loading Tests
// ============================================================================

TEST_CASE("get_default_themes_directory returns defaults path", "[theme]") {
    auto path = helix::get_default_themes_directory();
    REQUIRE(path.find("/defaults") != std::string::npos);
}

TEST_CASE("load_theme_from_file falls back to defaults directory", "[theme]") {
    // Setup: Theme exists only in defaults/, not in themes/
    // The "nord" theme should exist in defaults/ but not themes/
    std::string themes_dir = helix::get_themes_directory();
    std::string defaults_dir = helix::get_default_themes_directory();

    // Ensure nord doesn't exist in user themes dir
    std::string user_nord = themes_dir + "/nord.json";
    std::remove(user_nord.c_str());

    // Load should fall back to defaults
    auto theme = helix::load_theme_from_file("nord");

    REQUIRE(theme.is_valid());
    REQUIRE(theme.name == "Nord");
}

TEST_CASE("user theme overrides default theme with same name", "[theme]") {
    std::string themes_dir = helix::get_themes_directory();

    // Create a user theme with the same name as a default theme
    helix::ThemeData user_theme = helix::get_default_nord_theme();
    user_theme.name = "User Nord Override";
    user_theme.dark.app_bg = "#111111"; // Different color to identify it

    std::string user_path = themes_dir + "/nord.json";
    REQUIRE(helix::save_theme_to_file(user_theme, user_path));

    // Load should return user version
    auto loaded = helix::load_theme_from_file("nord");

    REQUIRE(loaded.name == "User Nord Override");
    REQUIRE(loaded.dark.app_bg == "#111111");

    // Cleanup
    std::remove(user_path.c_str());
}

TEST_CASE("discover_themes merges user and default themes", "[theme]") {
    std::string themes_dir = helix::get_themes_directory();

    // Create a user-only theme
    helix::ThemeData user_only = helix::get_default_nord_theme();
    user_only.name = "User Only Theme";
    user_only.filename = "user_only_test";

    std::string user_path = themes_dir + "/user_only_test.json";
    REQUIRE(helix::save_theme_to_file(user_only, user_path));

    // Discover themes - should include both defaults and user themes
    auto themes = helix::discover_themes(themes_dir);

    // Check that we have some themes (defaults + user)
    REQUIRE(themes.size() > 1);

    // Check that user-only theme is included
    bool found_user_only = false;
    for (const auto& info : themes) {
        if (info.filename == "user_only_test") {
            found_user_only = true;
            REQUIRE(info.display_name == "User Only Theme");
            break;
        }
    }
    REQUIRE(found_user_only);

    // Check that a default theme is also included (e.g., nord)
    bool found_default = false;
    for (const auto& info : themes) {
        if (info.filename == "nord") {
            found_default = true;
            break;
        }
    }
    REQUIRE(found_default);

    // Cleanup
    std::remove(user_path.c_str());
}

TEST_CASE("has_default_theme returns true for bundled themes", "[theme]") {
    // "nord" is a shipped default theme
    REQUIRE(helix::has_default_theme("nord") == true);
    REQUIRE(helix::has_default_theme("gruvbox") == true);
    REQUIRE(helix::has_default_theme("catppuccin") == true);
}

TEST_CASE("has_default_theme returns false for user-created themes", "[theme]") {
    // Non-existent themes
    REQUIRE(helix::has_default_theme("my-custom-theme") == false);
    REQUIRE(helix::has_default_theme("nonexistent") == false);
}

TEST_CASE("reset_theme_to_default deletes user file and returns default", "[theme]") {
    std::string themes_dir = helix::get_themes_directory();

    // Create a user override for nord
    helix::ThemeData user_override = helix::get_default_nord_theme();
    user_override.name = "Modified Nord";
    user_override.dark.app_bg = "#222222";

    std::string user_path = themes_dir + "/nord.json";
    REQUIRE(helix::save_theme_to_file(user_override, user_path));

    // Verify user file exists
    struct stat st;
    REQUIRE(stat(user_path.c_str(), &st) == 0);

    // Reset to default
    auto result = helix::reset_theme_to_default("nord");

    REQUIRE(result.has_value());
    REQUIRE(result->name == "Nord");           // Should be original default
    REQUIRE(result->dark.app_bg != "#222222"); // Should not be user override

    // User file should be deleted
    REQUIRE(stat(user_path.c_str(), &st) != 0);
}

TEST_CASE("reset_theme_to_default returns nullopt for user-created themes", "[theme]") {
    std::string themes_dir = helix::get_themes_directory();

    // Create a user-only theme (no default exists)
    helix::ThemeData user_theme = helix::get_default_nord_theme();
    user_theme.name = "My Custom Theme";
    user_theme.filename = "my_custom_test";

    std::string user_path = themes_dir + "/my_custom_test.json";
    REQUIRE(helix::save_theme_to_file(user_theme, user_path));

    // Try to reset - should return nullopt since there's no default
    auto result = helix::reset_theme_to_default("my_custom_test");

    REQUIRE_FALSE(result.has_value());

    // User file should still exist
    struct stat st;
    REQUIRE(stat(user_path.c_str(), &st) == 0);

    // Cleanup
    std::remove(user_path.c_str());
}
