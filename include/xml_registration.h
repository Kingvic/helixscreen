// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file xml_registration.h
 * @brief XML component and font registration in correct dependency order
 *
 * @pattern Two-phase: register_fonts_and_images() -> register_xml_components()
 * @threading Main thread only; must complete before UI creation
 * @gotchas Fonts need BOTH lv_conf.h enable AND lv_xml_register_font() - missing either silently
 * falls back
 */

#pragma once

namespace helix {

/**
 * @brief Register fonts and images for XML component system
 *
 * IMPORTANT: Fonts require TWO steps to work:
 *   1. Enable in lv_conf.h: #define LV_FONT_MONTSERRAT_XX 1
 *   2. Register here with lv_xml_register_font()
 *
 * If either step is missing, LVGL will silently fall back to a different font.
 *
 * @see docs/LVGL9_XML_GUIDE.md "Typography - Semantic Text Components"
 */
void register_fonts_and_images();

/**
 * @brief Register XML components from ui_xml/ directory
 *
 * Registers all XML component definitions in dependency order.
 * Must be called after register_fonts_and_images() and theme init.
 */
void register_xml_components();

} // namespace helix
