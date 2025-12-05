// Copyright 2025 356C LLC
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file test_thumbnail_selection.cpp
 * @brief Unit tests for ThumbnailInfo and FileMetadata::get_largest_thumbnail()
 *
 * Tests the thumbnail selection logic that picks the largest available
 * thumbnail by pixel count for best display quality.
 */

#include "../../include/moonraker_api.h"

#include "../catch_amalgamated.hpp"

// ============================================================================
// ThumbnailInfo Tests
// ============================================================================

TEST_CASE("ThumbnailInfo pixel_count calculation", "[thumbnail]") {
    SECTION("Calculates correct pixel count for standard dimensions") {
        ThumbnailInfo info;
        info.width = 300;
        info.height = 300;
        REQUIRE(info.pixel_count() == 90000);
    }

    SECTION("Calculates correct pixel count for rectangular thumbnails") {
        ThumbnailInfo info;
        info.width = 400;
        info.height = 300;
        REQUIRE(info.pixel_count() == 120000);
    }

    SECTION("Returns zero for uninitialized thumbnail") {
        ThumbnailInfo info;
        REQUIRE(info.pixel_count() == 0);
    }

    SECTION("Handles small thumbnails") {
        ThumbnailInfo info;
        info.width = 32;
        info.height = 32;
        REQUIRE(info.pixel_count() == 1024);
    }
}

// ============================================================================
// FileMetadata::get_largest_thumbnail Tests
// ============================================================================

TEST_CASE("FileMetadata get_largest_thumbnail", "[thumbnail]") {
    SECTION("Returns empty string when no thumbnails") {
        FileMetadata metadata;
        REQUIRE(metadata.get_largest_thumbnail().empty());
    }

    SECTION("Returns only thumbnail when one available") {
        FileMetadata metadata;
        ThumbnailInfo thumb;
        thumb.relative_path = ".thumbnails/test-300x300.png";
        thumb.width = 300;
        thumb.height = 300;
        metadata.thumbnails.push_back(thumb);

        REQUIRE(metadata.get_largest_thumbnail() == ".thumbnails/test-300x300.png");
    }

    SECTION("Selects largest thumbnail by pixel count") {
        FileMetadata metadata;

        // Small thumbnail (32x32 = 1024 pixels)
        ThumbnailInfo small;
        small.relative_path = ".thumbnails/test-32x32.png";
        small.width = 32;
        small.height = 32;
        metadata.thumbnails.push_back(small);

        // Medium thumbnail (150x150 = 22500 pixels)
        ThumbnailInfo medium;
        medium.relative_path = ".thumbnails/test-150x150.png";
        medium.width = 150;
        medium.height = 150;
        metadata.thumbnails.push_back(medium);

        // Large thumbnail (300x300 = 90000 pixels)
        ThumbnailInfo large;
        large.relative_path = ".thumbnails/test-300x300.png";
        large.width = 300;
        large.height = 300;
        metadata.thumbnails.push_back(large);

        REQUIRE(metadata.get_largest_thumbnail() == ".thumbnails/test-300x300.png");
    }

    SECTION("Handles thumbnails in any order") {
        FileMetadata metadata;

        // Add largest first
        ThumbnailInfo large;
        large.relative_path = ".thumbnails/test-300x300.png";
        large.width = 300;
        large.height = 300;
        metadata.thumbnails.push_back(large);

        // Add smallest last
        ThumbnailInfo small;
        small.relative_path = ".thumbnails/test-32x32.png";
        small.width = 32;
        small.height = 32;
        metadata.thumbnails.push_back(small);

        REQUIRE(metadata.get_largest_thumbnail() == ".thumbnails/test-300x300.png");
    }

    SECTION("Handles rectangular thumbnails correctly") {
        FileMetadata metadata;

        // 400x300 = 120000 pixels
        ThumbnailInfo rect;
        rect.relative_path = ".thumbnails/test-400x300.png";
        rect.width = 400;
        rect.height = 300;
        metadata.thumbnails.push_back(rect);

        // 300x300 = 90000 pixels (smaller even though same height)
        ThumbnailInfo square;
        square.relative_path = ".thumbnails/test-300x300.png";
        square.width = 300;
        square.height = 300;
        metadata.thumbnails.push_back(square);

        REQUIRE(metadata.get_largest_thumbnail() == ".thumbnails/test-400x300.png");
    }

    SECTION("Falls back to first thumbnail when dimensions are zero") {
        FileMetadata metadata;

        // First thumbnail with no dimensions
        ThumbnailInfo first;
        first.relative_path = ".thumbnails/test-first.png";
        first.width = 0;
        first.height = 0;
        metadata.thumbnails.push_back(first);

        // Second thumbnail with no dimensions
        ThumbnailInfo second;
        second.relative_path = ".thumbnails/test-second.png";
        second.width = 0;
        second.height = 0;
        metadata.thumbnails.push_back(second);

        // When all have 0 pixels, returns first (stable selection)
        REQUIRE(metadata.get_largest_thumbnail() == ".thumbnails/test-first.png");
    }

    SECTION("Prefers thumbnail with dimensions over ones without") {
        FileMetadata metadata;

        // Thumbnail without dimensions
        ThumbnailInfo no_dims;
        no_dims.relative_path = ".thumbnails/test-unknown.png";
        no_dims.width = 0;
        no_dims.height = 0;
        metadata.thumbnails.push_back(no_dims);

        // Thumbnail with dimensions
        ThumbnailInfo with_dims;
        with_dims.relative_path = ".thumbnails/test-300x300.png";
        with_dims.width = 300;
        with_dims.height = 300;
        metadata.thumbnails.push_back(with_dims);

        REQUIRE(metadata.get_largest_thumbnail() == ".thumbnails/test-300x300.png");
    }
}
