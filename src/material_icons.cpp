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

#include "material_icons.h"

#include "lvgl/lvgl.h"
#include "lvgl/src/xml/lv_xml.h"

#include <spdlog/spdlog.h>

void material_icons_register() {
    spdlog::trace("Registering Material Design icons (77 total)...");

    // Navigation & Movement
    lv_xml_register_image(NULL, "mat_home_img", &home);
    lv_xml_register_image(NULL, "mat_home_printer_img", &home_printer);
    lv_xml_register_image(NULL, "mat_arrow_up_img", &arrow_up);
    lv_xml_register_image(NULL, "mat_arrow_down_img", &arrow_down);
    lv_xml_register_image(NULL, "mat_arrow_left_img", &arrow_left);
    lv_xml_register_image(NULL, "mat_arrow_right_img", &arrow_right);
    lv_xml_register_image(NULL, "mat_back_img", &back);
    lv_xml_register_image(NULL, "mat_move_img", &move);
    lv_xml_register_image(NULL, "mat_z_closer_img", &z_closer);
    lv_xml_register_image(NULL, "mat_z_farther_img", &z_farther);
    lv_xml_register_image(NULL, "mat_home_z_img", &home_z);

    // Print & Files
    lv_xml_register_image(NULL, "mat_print_img", &print);
    lv_xml_register_image(NULL, "mat_list_img", &list);
    lv_xml_register_image(NULL, "mat_grid_view_img", &grid_view);
    lv_xml_register_image(NULL, "mat_folder_img", &folder);
    lv_xml_register_image(NULL, "mat_folder_up_img", &folder_up);
    lv_xml_register_image(NULL, "mat_pause_img", &pause_img);
    lv_xml_register_image(NULL, "mat_resume_img", &resume);
    lv_xml_register_image(NULL, "mat_cancel_img", &cancel);
    lv_xml_register_image(NULL, "mat_prohibited_img", &prohibited);
    lv_xml_register_image(NULL, "mat_sd_img", &sd_img);
    lv_xml_register_image(NULL, "mat_refresh_img", &refresh_img);

    // Temperature & Heating
    lv_xml_register_image(NULL, "mat_bed_img", &bed);
    lv_xml_register_image(NULL, "mat_heater_img", &heater);
    lv_xml_register_image(NULL, "mat_cooldown_img", &cooldown_img);

    // Extrusion & Filament
    lv_xml_register_image(NULL, "mat_extruder_img", &extruder);
    lv_xml_register_image(NULL, "mat_extrude_img", &extrude);
    lv_xml_register_image(NULL, "mat_extrude_img", &extrude_img);
    lv_xml_register_image(NULL, "mat_filament_img", &filament_img);
    lv_xml_register_image(NULL, "mat_load_filament_img", &load_filament_img);
    lv_xml_register_image(NULL, "mat_unload_filament_img", &unload_filament_img);
    lv_xml_register_image(NULL, "mat_retract_img", &retract_img);

    // Fan & Cooling
    lv_xml_register_image(NULL, "mat_fan_img", &fan);
    lv_xml_register_image(NULL, "mat_fan_on_img", &fan_on);
    lv_xml_register_image(NULL, "mat_fan_off_img", &fan_off_img);

    // Lighting
    lv_xml_register_image(NULL, "mat_light_img", &light_img);
    lv_xml_register_image(NULL, "mat_light_off_img", &light_off);

    // Network & Communication
    lv_xml_register_image(NULL, "mat_network_img", &network_img);
    lv_xml_register_image(NULL, "mat_lan_img", &lan);
    lv_xml_register_image(NULL, "mat_router_img", &router);
    lv_xml_register_image(NULL, "mat_printer_3d_img", &printer_3d);

    // Tuning & Adjustments
    lv_xml_register_image(NULL, "mat_fine_tune_img", &fine_tune_img);
    lv_xml_register_image(NULL, "mat_flow_down_img", &flow_down_img);
    lv_xml_register_image(NULL, "mat_flow_up_img", &flow_up_img);
    lv_xml_register_image(NULL, "mat_speed_down_img", &speed_down_img);
    lv_xml_register_image(NULL, "mat_speed_up_img", &speed_up_img);
    lv_xml_register_image(NULL, "mat_pa_minus_img", &pa_minus_img);
    lv_xml_register_image(NULL, "mat_pa_plus_img", &pa_plus_img);

    // Calibration & Advanced
    lv_xml_register_image(NULL, "mat_bedmesh_img", &bedmesh_img);
    lv_xml_register_image(NULL, "mat_belts_calibration_img", &belts_calibration_img);
    lv_xml_register_image(NULL, "mat_inputshaper_img", &inputshaper_img);
    lv_xml_register_image(NULL, "mat_limit_img", &limit_img);

    // System & Info
    lv_xml_register_image(NULL, "mat_info_img", &info_img);
    lv_xml_register_image(NULL, "mat_sysinfo_img", &sysinfo_img);
    lv_xml_register_image(NULL, "mat_settings_img", &settings);
    lv_xml_register_image(NULL, "mat_power_devices_img", &power_devices_img);
    lv_xml_register_image(NULL, "mat_motor_img", &motor_img);
    lv_xml_register_image(NULL, "mat_motor_off_img", &motor_off_img);
    lv_xml_register_image(NULL, "mat_update_img", &update_img);
    lv_xml_register_image(NULL, "mat_emergency_img", &emergency);
    lv_xml_register_image(NULL, "mat_delete_img", &delete_img);
    lv_xml_register_image(NULL, "mat_build_img", &build);
    lv_xml_register_image(NULL, "mat_dashboard_img", &dashboard);
    lv_xml_register_image(NULL, "mat_speed_img", &speed);
    lv_xml_register_image(NULL, "mat_inventory_img", &inventory);
    lv_xml_register_image(NULL, "mat_code_img", &code);
    lv_xml_register_image(NULL, "mat_terminal_img", &terminal);

    // Monitoring & Display
    lv_xml_register_image(NULL, "mat_chart_img", &chart_img);
    lv_xml_register_image(NULL, "mat_layers_img", &layers_img);
    lv_xml_register_image(NULL, "mat_clock_img", &clock_img);
    lv_xml_register_image(NULL, "mat_hourglass_img", &hourglass);

    // Misc
    lv_xml_register_image(NULL, "mat_spoolman_img", &spoolman_img);
    lv_xml_register_image(NULL, "mat_printer_3d_img", &printer_3d);

    // WiFi & Network
    spdlog::debug("Registering WiFi icons...");
    lv_xml_register_image(NULL, "mat_wifi_img", &wifi);
    lv_xml_register_image(NULL, "mat_wifi_off_img", &wifi_off);
    lv_xml_register_image(NULL, "mat_wifi_lock_img", &wifi_lock);
    lv_xml_register_image(NULL, "mat_wifi_check_img", &wifi_check);
    lv_xml_register_image(NULL, "mat_wifi_alert_img", &wifi_alert);
    lv_xml_register_image(NULL, "mat_wifi_strength_1_img", &wifi_strength_1);
    lv_xml_register_image(NULL, "mat_wifi_strength_2_img", &wifi_strength_2);
    lv_xml_register_image(NULL, "mat_wifi_strength_3_img", &wifi_strength_3);
    lv_xml_register_image(NULL, "mat_wifi_strength_4_img", &wifi_strength_4);
    lv_xml_register_image(NULL, "mat_wifi_strength_1_lock_img", &wifi_strength_1_lock);
    lv_xml_register_image(NULL, "mat_wifi_strength_2_lock_img", &wifi_strength_2_lock);
    lv_xml_register_image(NULL, "mat_wifi_strength_3_lock_img", &wifi_strength_3_lock);
    lv_xml_register_image(NULL, "mat_wifi_strength_4_lock_img", &wifi_strength_4_lock);
    spdlog::trace("WiFi icons registered (13 total)");

    spdlog::trace("Material Design icons registered successfully");
    lv_xml_register_image(NULL, "mat_notifications_img", &notifications);
}
