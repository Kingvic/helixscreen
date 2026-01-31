// SPDX-License-Identifier: GPL-3.0-or-later
#include "theme_manager.h"

// Forward declarations for style configure functions
namespace style_configs {
void configure_card(lv_style_t* s, const ThemePalette& p);
void configure_dialog(lv_style_t* s, const ThemePalette& p);
void configure_obj_base(lv_style_t* s, const ThemePalette& p);
void configure_input_bg(lv_style_t* s, const ThemePalette& p);
void configure_disabled(lv_style_t* s, const ThemePalette& p);
void configure_pressed(lv_style_t* s, const ThemePalette& p);
void configure_focused(lv_style_t* s, const ThemePalette& p);
void configure_text_primary(lv_style_t* s, const ThemePalette& p);
void configure_text_muted(lv_style_t* s, const ThemePalette& p);
void configure_text_subtle(lv_style_t* s, const ThemePalette& p);
void configure_icon_text(lv_style_t* s, const ThemePalette& p);
void configure_icon_primary(lv_style_t* s, const ThemePalette& p);
void configure_icon_secondary(lv_style_t* s, const ThemePalette& p);
void configure_icon_tertiary(lv_style_t* s, const ThemePalette& p);
void configure_icon_info(lv_style_t* s, const ThemePalette& p);
void configure_icon_success(lv_style_t* s, const ThemePalette& p);
void configure_icon_warning(lv_style_t* s, const ThemePalette& p);
void configure_icon_danger(lv_style_t* s, const ThemePalette& p);
void configure_button(lv_style_t* s, const ThemePalette& p);
void configure_button_primary(lv_style_t* s, const ThemePalette& p);
void configure_button_secondary(lv_style_t* s, const ThemePalette& p);
void configure_button_tertiary(lv_style_t* s, const ThemePalette& p);
void configure_button_danger(lv_style_t* s, const ThemePalette& p);
void configure_button_ghost(lv_style_t* s, const ThemePalette& p);
void configure_button_disabled(lv_style_t* s, const ThemePalette& p);
void configure_button_pressed(lv_style_t* s, const ThemePalette& p);
void configure_severity_info(lv_style_t* s, const ThemePalette& p);
void configure_severity_success(lv_style_t* s, const ThemePalette& p);
void configure_severity_warning(lv_style_t* s, const ThemePalette& p);
void configure_severity_danger(lv_style_t* s, const ThemePalette& p);
void configure_dropdown(lv_style_t* s, const ThemePalette& p);
void configure_checkbox(lv_style_t* s, const ThemePalette& p);
void configure_switch(lv_style_t* s, const ThemePalette& p);
void configure_slider(lv_style_t* s, const ThemePalette& p);
void configure_spinner(lv_style_t* s, const ThemePalette& p);
void configure_arc(lv_style_t* s, const ThemePalette& p);
} // namespace style_configs

ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

void ThemeManager::init() {
    if (initialized_)
        return;

    register_style_configs();

    // Default Nord-ish palette for testing
    current_palette_.screen_bg = lv_color_hex(0x1a1a2e);
    current_palette_.overlay_bg = lv_color_hex(0x2E3440);
    current_palette_.card_bg = lv_color_hex(0x2E3440);
    current_palette_.elevated_bg = lv_color_hex(0x3B4252);
    current_palette_.border = lv_color_hex(0x4C566A);

    current_palette_.text = lv_color_hex(0xECEFF4);
    current_palette_.text_muted = lv_color_hex(0xD8DEE9);
    current_palette_.text_subtle = lv_color_hex(0x8FBCBB);

    current_palette_.primary = lv_color_hex(0x88C0D0);
    current_palette_.secondary = lv_color_hex(0x81A1C1);
    current_palette_.tertiary = lv_color_hex(0x5E81AC);
    current_palette_.info = lv_color_hex(0x88C0D0);
    current_palette_.success = lv_color_hex(0xA3BE8C);
    current_palette_.warning = lv_color_hex(0xEBCB8B);
    current_palette_.danger = lv_color_hex(0xBF616A);
    current_palette_.focus = lv_color_hex(0x88C0D0);

    current_palette_.border_radius = 8;
    current_palette_.border_width = 1;
    current_palette_.border_opacity = 40;

    apply_palette(current_palette_);
    initialized_ = true;
}

void ThemeManager::shutdown() {
    for (auto& entry : styles_) {
        lv_style_reset(&entry.style);
    }
    initialized_ = false;
}

lv_style_t* ThemeManager::get_style(StyleRole role) {
    auto idx = static_cast<size_t>(role);
    if (idx >= styles_.size())
        return nullptr;
    return &styles_[idx].style;
}

void ThemeManager::register_style_configs() {
    using namespace style_configs;

    // Initialize all styles
    for (size_t i = 0; i < styles_.size(); ++i) {
        styles_[i].role = static_cast<StyleRole>(i);
        lv_style_init(&styles_[i].style);
    }

    // Register configure functions
    styles_[static_cast<size_t>(StyleRole::Card)].configure = configure_card;
    styles_[static_cast<size_t>(StyleRole::Dialog)].configure = configure_dialog;
    styles_[static_cast<size_t>(StyleRole::ObjBase)].configure = configure_obj_base;
    styles_[static_cast<size_t>(StyleRole::InputBg)].configure = configure_input_bg;
    styles_[static_cast<size_t>(StyleRole::Disabled)].configure = configure_disabled;
    styles_[static_cast<size_t>(StyleRole::Pressed)].configure = configure_pressed;
    styles_[static_cast<size_t>(StyleRole::Focused)].configure = configure_focused;

    styles_[static_cast<size_t>(StyleRole::TextPrimary)].configure = configure_text_primary;
    styles_[static_cast<size_t>(StyleRole::TextMuted)].configure = configure_text_muted;
    styles_[static_cast<size_t>(StyleRole::TextSubtle)].configure = configure_text_subtle;

    styles_[static_cast<size_t>(StyleRole::IconText)].configure = configure_icon_text;
    styles_[static_cast<size_t>(StyleRole::IconPrimary)].configure = configure_icon_primary;
    styles_[static_cast<size_t>(StyleRole::IconSecondary)].configure = configure_icon_secondary;
    styles_[static_cast<size_t>(StyleRole::IconTertiary)].configure = configure_icon_tertiary;
    styles_[static_cast<size_t>(StyleRole::IconInfo)].configure = configure_icon_info;
    styles_[static_cast<size_t>(StyleRole::IconSuccess)].configure = configure_icon_success;
    styles_[static_cast<size_t>(StyleRole::IconWarning)].configure = configure_icon_warning;
    styles_[static_cast<size_t>(StyleRole::IconDanger)].configure = configure_icon_danger;

    styles_[static_cast<size_t>(StyleRole::Button)].configure = configure_button;
    styles_[static_cast<size_t>(StyleRole::ButtonPrimary)].configure = configure_button_primary;
    styles_[static_cast<size_t>(StyleRole::ButtonSecondary)].configure = configure_button_secondary;
    styles_[static_cast<size_t>(StyleRole::ButtonTertiary)].configure = configure_button_tertiary;
    styles_[static_cast<size_t>(StyleRole::ButtonDanger)].configure = configure_button_danger;
    styles_[static_cast<size_t>(StyleRole::ButtonGhost)].configure = configure_button_ghost;
    styles_[static_cast<size_t>(StyleRole::ButtonDisabled)].configure = configure_button_disabled;
    styles_[static_cast<size_t>(StyleRole::ButtonPressed)].configure = configure_button_pressed;

    styles_[static_cast<size_t>(StyleRole::SeverityInfo)].configure = configure_severity_info;
    styles_[static_cast<size_t>(StyleRole::SeveritySuccess)].configure = configure_severity_success;
    styles_[static_cast<size_t>(StyleRole::SeverityWarning)].configure = configure_severity_warning;
    styles_[static_cast<size_t>(StyleRole::SeverityDanger)].configure = configure_severity_danger;

    styles_[static_cast<size_t>(StyleRole::Dropdown)].configure = configure_dropdown;
    styles_[static_cast<size_t>(StyleRole::Checkbox)].configure = configure_checkbox;
    styles_[static_cast<size_t>(StyleRole::Switch)].configure = configure_switch;
    styles_[static_cast<size_t>(StyleRole::Slider)].configure = configure_slider;
    styles_[static_cast<size_t>(StyleRole::Spinner)].configure = configure_spinner;
    styles_[static_cast<size_t>(StyleRole::Arc)].configure = configure_arc;
}

void ThemeManager::apply_palette(const ThemePalette& palette) {
    current_palette_ = palette;
    for (auto& entry : styles_) {
        if (entry.configure) {
            lv_style_reset(&entry.style);
            entry.configure(&entry.style, palette);
        }
    }
}
