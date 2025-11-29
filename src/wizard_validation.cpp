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
 */

#include "wizard_validation.h"

#include <cctype>

bool is_valid_ip_or_hostname(const std::string& host) {
    if (host.empty()) {
        return false;
    }

    // RFC 1035: Total hostname max 253 characters
    if (host.length() > 253) {
        return false;
    }

    // First check if it looks like an IP address (contains only digits and dots)
    bool looks_like_ip = true;
    for (char c : host) {
        if (!std::isdigit(c) && c != '.') {
            looks_like_ip = false;
            break;
        }
    }

    // If it looks like an IP, validate as IPv4
    if (looks_like_ip) {
        int dot_count = 0;
        size_t last_dot = 0;
        bool valid_ip = true;

        for (size_t i = 0; i < host.length(); i++) {
            if (host[i] == '.') {
                // Check segment between dots
                if (i == last_dot) { // Empty segment (e.g., "192..1.1")
                    valid_ip = false;
                    break;
                }
                std::string segment = host.substr(last_dot, i - last_dot);
                if (segment.empty() || segment.length() > 3) {
                    valid_ip = false;
                    break;
                }
                try {
                    int num = std::stoi(segment);
                    if (num < 0 || num > 255) {
                        valid_ip = false;
                        break;
                    }
                } catch (...) {
                    valid_ip = false;
                    break;
                }
                dot_count++;
                last_dot = i + 1;
            }
        }

        if (valid_ip && dot_count == 3) {
            // Check last segment
            std::string last_segment = host.substr(last_dot);
            if (!last_segment.empty() && last_segment.length() <= 3) {
                try {
                    int num = std::stoi(last_segment);
                    if (num >= 0 && num <= 255) {
                        return true;
                    }
                } catch (...) {
                }
            }
        }
        // If it looks like an IP but isn't valid, reject it
        return false;
    }

    // Otherwise, check if it's a valid hostname
    // RFC 1035: Must start with alphanumeric, cannot end with hyphen
    // Allowed: alphanumeric, hyphens, dots (no underscores per RFC)
    if (!std::isalnum(host[0])) {
        return false;
    }

    // Cannot end with hyphen or dot
    if (host.back() == '-' || host.back() == '.') {
        return false;
    }

    // Cannot start with dot
    if (host[0] == '.') {
        return false;
    }

    // Validate each label (segment between dots)
    size_t label_start = 0;
    for (size_t i = 0; i <= host.length(); i++) {
        if (i == host.length() || host[i] == '.') {
            size_t label_len = i - label_start;

            // RFC 1035: Each label max 63 characters
            if (label_len > 63 || label_len == 0) {
                return false;
            }

            // Label cannot start or end with hyphen
            if (host[label_start] == '-' || (i > 0 && host[i - 1] == '-')) {
                return false;
            }

            label_start = i + 1;
        } else {
            char c = host[i];
            // Alphanumeric, hyphen, and underscore allowed
            // (underscores not RFC-compliant but common in internal networks)
            if (!std::isalnum(c) && c != '-' && c != '_') {
                return false;
            }
        }
    }

    return true;
}

bool is_valid_port(const std::string& port_str) {
    if (port_str.empty()) {
        return false;
    }

    // Check all characters are digits
    for (char c : port_str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    // Reject leading zeros (could be confused with octal notation)
    // Exception: "0" alone is handled by range check below
    if (port_str.length() > 1 && port_str[0] == '0') {
        return false;
    }

    // Parse and validate range
    try {
        int port = std::stoi(port_str);
        return port > 0 && port <= 65535;
    } catch (...) {
        return false;
    }
}
