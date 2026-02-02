// SPDX-License-Identifier: GPL-3.0-or-later
//
// Stub implementations for LVGL translation API
// These are placeholders until a proper translation system is implemented.
// TODO: Remove when real translation system is built

#pragma once

#include <spdlog/spdlog.h>

#include <lvgl.h>

// Stub: Register translation file
// In production, this would parse XML translation data
inline lv_result_t lv_xml_register_translation_from_file(const char* path) {
    spdlog::debug("[Translation Stub] Would load translations from: {}", path);
    return LV_RESULT_OK;
}

// Stub: Set active language
// In production, this would trigger hot-reload of all translatable text
inline void lv_translation_set_language(const char* lang) {
    spdlog::debug("[Translation Stub] Would set language to: {}", lang);
}
