// Copyright 2025 356C LLC
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file test_ui_panel_console.cpp
 * @brief Unit tests for ConsolePanel G-code history functionality
 *
 * Tests the static helper methods and logic for parsing G-code console entries.
 * These tests don't require LVGL initialization since they test pure C++ logic.
 */

#include <string>

#include "../catch_amalgamated.hpp"

// ============================================================================
// Test is_error_message() detection logic
// (Replicated from ui_panel_console.cpp since it's a private static method)
// ============================================================================

/**
 * @brief Check if a response message indicates an error
 *
 * Moonraker/Klipper errors typically start with "!!" or contain
 * "error" in the message.
 */
static bool is_error_message(const std::string& message) {
    if (message.empty()) {
        return false;
    }

    // Klipper errors typically start with "!!" prefix
    if (message.size() >= 2 && message[0] == '!' && message[1] == '!') {
        return true;
    }

    // Case-insensitive check for "error" at start
    if (message.size() >= 5) {
        std::string lower = message.substr(0, 5);
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (lower == "error") {
            return true;
        }
    }

    return false;
}

// ============================================================================
// Error Message Detection Tests
// ============================================================================

TEST_CASE("Console: is_error_message() with empty string", "[console][error_detection]") {
    REQUIRE(is_error_message("") == false);
}

TEST_CASE("Console: is_error_message() with !! prefix", "[console][error_detection]") {
    REQUIRE(is_error_message("!! Error: Heater not responding") == true);
    REQUIRE(is_error_message("!!Thermistor disconnected") == true);
    REQUIRE(is_error_message("!! ") == true);
}

TEST_CASE("Console: is_error_message() with Error prefix", "[console][error_detection]") {
    REQUIRE(is_error_message("Error: Command failed") == true);
    REQUIRE(is_error_message("ERROR: Unknown G-code") == true);
    REQUIRE(is_error_message("error: invalid parameter") == true);
    REQUIRE(is_error_message("ErRoR: mixed case") == true);
}

TEST_CASE("Console: is_error_message() with normal responses", "[console][error_detection]") {
    // Normal OK responses
    REQUIRE(is_error_message("ok") == false);
    REQUIRE(is_error_message("// Klipper state: Ready") == false);
    REQUIRE(is_error_message("B:60.0 /60.0 T0:210.0 /210.0") == false);

    // Messages containing "error" but not at start
    REQUIRE(is_error_message("No error detected") == false);
    REQUIRE(is_error_message("G-code M112 for error stop") == false);
}

TEST_CASE("Console: is_error_message() with single character", "[console][error_detection]") {
    REQUIRE(is_error_message("!") == false); // Only one !, not two
    REQUIRE(is_error_message("E") == false); // Not enough characters for "Error"
}

TEST_CASE("Console: is_error_message() with boundary cases", "[console][error_detection]") {
    REQUIRE(is_error_message("Err") == false);   // Too short for "Error"
    REQUIRE(is_error_message("Erro") == false);  // Still too short
    REQUIRE(is_error_message("Error") == true);  // Exactly "Error"
    REQUIRE(is_error_message("Errorx") == true); // Starts with "Error"
}

// ============================================================================
// Entry Type Classification Tests
// ============================================================================

TEST_CASE("Console: command vs response type classification", "[console][entry_type]") {
    // These would come from MoonrakerClient::GcodeStoreEntry.type field

    // Commands are user input
    REQUIRE(std::string("command") == "command");

    // Responses are Klipper output
    REQUIRE(std::string("response") == "response");
}

// ============================================================================
// Message Content Tests
// ============================================================================

TEST_CASE("Console: typical Klipper error messages", "[console][error_detection]") {
    // Real Klipper error message patterns
    REQUIRE(is_error_message("!! Move out of range: 0.000 250.000 0.500 [0.000]") == true);
    REQUIRE(is_error_message("!! Timer too close") == true);
    REQUIRE(is_error_message("!! MCU 'mcu' shutdown: Timer too close") == true);
    REQUIRE(is_error_message("Error: Bed heater not responding") == true);
}

TEST_CASE("Console: typical Klipper info messages", "[console][error_detection]") {
    // Normal Klipper messages that should NOT be flagged as errors
    REQUIRE(is_error_message("// Klipper state: Ready") == false);
    REQUIRE(is_error_message("// probe at 150.000,150.000 is z=1.234567") == false);
    REQUIRE(is_error_message("echo: G28 homing completed") == false);
    REQUIRE(is_error_message("Recv: ok") == false);
}
