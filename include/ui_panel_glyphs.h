// Copyright 2025 HelixScreen
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * Copyright (C) 2025 356C LLC
 * Author: Preston Brown <pbrown@brown-house.net>
 *
 * This file is part of HelixScreen.
 *
 * HelixScreen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HelixScreen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HelixScreen. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef UI_PANEL_GLYPHS_H
#define UI_PANEL_GLYPHS_H

#include <lvgl/lvgl.h>

/**
 * @brief Create and initialize the glyphs panel
 *
 * Creates a panel displaying all LVGL symbol glyphs with their names.
 * The panel features:
 * - Scrollable vertical list of all symbols
 * - Each entry shows icon + symbolic name (e.g., "LV_SYMBOL_AUDIO")
 * - Count of total symbols in header
 * - Proper theming via globals.xml constants
 *
 * @param parent Parent object to attach panel to
 * @return lv_obj_t* The created panel object
 */
lv_obj_t* ui_panel_glyphs_create(lv_obj_t* parent);

#endif // UI_PANEL_GLYPHS_H
