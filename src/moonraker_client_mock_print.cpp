// Copyright 2025 HelixScreen
// SPDX-License-Identifier: GPL-3.0-or-later

#include "moonraker_client_mock_internal.h"

#include <spdlog/spdlog.h>

namespace mock_internal {

void register_print_handlers(std::unordered_map<std::string, MethodHandler>& registry) {
    // printer.gcode.script - Execute G-code script
    registry["printer.gcode.script"] =
        [](MoonrakerClientMock* self, const json& params, std::function<void(json)> success_cb,
           std::function<void(const MoonrakerError&)> error_cb) -> bool {
        (void)error_cb;
        std::string script;
        if (params.contains("script")) {
            script = params["script"].get<std::string>();
        }
        self->gcode_script(script); // Process G-code (updates LED state, etc.)
        if (success_cb) {
            success_cb(json::object()); // Return empty success response
        }
        return true;
    };

    // printer.print.start - Start a print job
    registry["printer.print.start"] =
        [](MoonrakerClientMock* self, const json& params, std::function<void(json)> success_cb,
           std::function<void(const MoonrakerError&)> error_cb) -> bool {
        std::string filename;
        if (params.contains("filename")) {
            filename = params["filename"].get<std::string>();
        }
        if (!filename.empty()) {
            if (self->start_print_internal(filename)) {
                if (success_cb) {
                    success_cb(json::object());
                }
            } else if (error_cb) {
                MoonrakerError err;
                err.type = MoonrakerErrorType::VALIDATION_ERROR;
                err.message = "Failed to start print";
                err.method = "printer.print.start";
                error_cb(err);
            }
        } else if (error_cb) {
            MoonrakerError err;
            err.type = MoonrakerErrorType::VALIDATION_ERROR;
            err.message = "Missing filename parameter";
            err.method = "printer.print.start";
            error_cb(err);
        }
        return true;
    };

    // printer.print.pause - Pause current print
    registry["printer.print.pause"] =
        [](MoonrakerClientMock* self, const json& params, std::function<void(json)> success_cb,
           std::function<void(const MoonrakerError&)> error_cb) -> bool {
        (void)params;
        if (self->pause_print_internal()) {
            if (success_cb) {
                success_cb(json::object());
            }
        } else if (error_cb) {
            MoonrakerError err;
            err.type = MoonrakerErrorType::VALIDATION_ERROR;
            err.message = "Cannot pause - not currently printing";
            err.method = "printer.print.pause";
            error_cb(err);
        }
        return true;
    };

    // printer.print.resume - Resume paused print
    registry["printer.print.resume"] =
        [](MoonrakerClientMock* self, const json& params, std::function<void(json)> success_cb,
           std::function<void(const MoonrakerError&)> error_cb) -> bool {
        (void)params;
        if (self->resume_print_internal()) {
            if (success_cb) {
                success_cb(json::object());
            }
        } else if (error_cb) {
            MoonrakerError err;
            err.type = MoonrakerErrorType::VALIDATION_ERROR;
            err.message = "Cannot resume - not currently paused";
            err.method = "printer.print.resume";
            error_cb(err);
        }
        return true;
    };

    // printer.print.cancel - Cancel current print
    registry["printer.print.cancel"] =
        [](MoonrakerClientMock* self, const json& params, std::function<void(json)> success_cb,
           std::function<void(const MoonrakerError&)> error_cb) -> bool {
        (void)params;
        if (self->cancel_print_internal()) {
            if (success_cb) {
                success_cb(json::object());
            }
        } else if (error_cb) {
            MoonrakerError err;
            err.type = MoonrakerErrorType::VALIDATION_ERROR;
            err.message = "Cannot cancel - no active print";
            err.method = "printer.print.cancel";
            error_cb(err);
        }
        return true;
    };
}

} // namespace mock_internal
