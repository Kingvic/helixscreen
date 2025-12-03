# SPDX-License-Identifier: GPL-3.0-or-later
#
# HelixScreen - Cross-Compilation Module
# Handles cross-compilation for embedded ARM targets
#
# Usage:
#   make                       # Native build (SDL)
#   make PLATFORM_TARGET=pi    # Cross-compile for Raspberry Pi (aarch64)
#   make PLATFORM_TARGET=ad5m  # Cross-compile for Adventurer 5M (armv7-a)
#   make pi-docker             # Docker-based Pi build
#   make ad5m-docker           # Docker-based AD5M build

# =============================================================================
# Target Platform Definitions
# =============================================================================

# Note: We use PLATFORM_TARGET to avoid collision with Makefile's TARGET (binary path)
PLATFORM_TARGET ?= native

ifeq ($(PLATFORM_TARGET),pi)
    # -------------------------------------------------------------------------
    # Raspberry Pi (Mainsail OS) - aarch64 / ARM64
    # -------------------------------------------------------------------------
    CROSS_COMPILE ?= aarch64-linux-gnu-
    TARGET_ARCH := aarch64
    TARGET_TRIPLE := aarch64-linux-gnu
    # Include paths for cross-compilation:
    # - /usr/aarch64-linux-gnu/include: arm64 sysroot headers
    # - /usr/include/libdrm: drm.h (needed by xf86drmMode.h)
    TARGET_CFLAGS := -march=armv8-a -I/usr/aarch64-linux-gnu/include -I/usr/include/libdrm
    DISPLAY_BACKEND := drm
    ENABLE_SDL := no
    ENABLE_TINYGL_3D := yes
    ENABLE_EVDEV := yes
    BUILD_SUBDIR := pi
    # Strip binary for size - embedded targets don't need debug symbols
    STRIP_BINARY := yes

else ifeq ($(PLATFORM_TARGET),ad5m)
    # -------------------------------------------------------------------------
    # Flashforge Adventurer 5M - Cortex-A7 (armv7-a hard-float)
    # Specs: 800x480 display, 110MB RAM, glibc 2.25
    # -------------------------------------------------------------------------
    CROSS_COMPILE ?= arm-linux-gnueabihf-
    TARGET_ARCH := armv7-a
    TARGET_TRIPLE := arm-linux-gnueabihf
    TARGET_CFLAGS := -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard -mtune=cortex-a7
    # GCC 8 requires -lstdc++fs for std::filesystem support (GCC 9+ includes it by default)
    TARGET_LDFLAGS := -lstdc++fs
    DISPLAY_BACKEND := fbdev
    ENABLE_SDL := no
    ENABLE_TINYGL_3D := yes
    ENABLE_EVDEV := yes
    BUILD_SUBDIR := ad5m
    # Strip binary for size on memory-constrained device
    STRIP_BINARY := yes

else ifeq ($(PLATFORM_TARGET),native)
    # -------------------------------------------------------------------------
    # Native desktop build (macOS / Linux)
    # -------------------------------------------------------------------------
    CROSS_COMPILE :=
    TARGET_ARCH := $(shell uname -m)
    TARGET_TRIPLE :=
    TARGET_CFLAGS :=
    DISPLAY_BACKEND := sdl
    ENABLE_SDL := yes
    # TinyGL controlled by main Makefile default
    ENABLE_EVDEV := no
    BUILD_SUBDIR :=

else
    $(error Unknown PLATFORM_TARGET: $(PLATFORM_TARGET). Valid options: native, pi, ad5m)
endif

# =============================================================================
# Cross-Compiler Configuration
# =============================================================================

ifneq ($(CROSS_COMPILE),)
    # Override compilers for cross-compilation
    CC := $(CROSS_COMPILE)gcc
    CXX := $(CROSS_COMPILE)g++
    AR := $(CROSS_COMPILE)ar
    STRIP := $(CROSS_COMPILE)strip
    RANLIB := $(CROSS_COMPILE)ranlib
    LD := $(CROSS_COMPILE)ld

    # Override build directories for cross-compilation
    ifneq ($(BUILD_SUBDIR),)
        BUILD_DIR := build/$(BUILD_SUBDIR)
        BIN_DIR := $(BUILD_DIR)/bin
        OBJ_DIR := $(BUILD_DIR)/obj
    endif

    # Print cross-compilation info
    $(info )
    $(info ========================================)
    $(info Cross-compiling for: $(PLATFORM_TARGET))
    $(info Architecture: $(TARGET_ARCH))
    $(info Compiler: $(CC))
    $(info Output: $(BUILD_DIR))
    $(info ========================================)
    $(info )
endif

# =============================================================================
# Target-Specific Flags
# =============================================================================

ifdef TARGET_CFLAGS
    CFLAGS += $(TARGET_CFLAGS)
    CXXFLAGS += $(TARGET_CFLAGS)
    SUBMODULE_CFLAGS += $(TARGET_CFLAGS)
    SUBMODULE_CXXFLAGS += $(TARGET_CFLAGS)
endif

ifdef TARGET_LDFLAGS
    LDFLAGS += $(TARGET_LDFLAGS)
endif

# Display backend defines (used by display_backend.cpp and lv_conf.h for conditional compilation)
# Must be added to SUBMODULE_*FLAGS as well for LVGL driver compilation
ifeq ($(DISPLAY_BACKEND),drm)
    CFLAGS += -DHELIX_DISPLAY_DRM -DHELIX_DISPLAY_FBDEV
    CXXFLAGS += -DHELIX_DISPLAY_DRM -DHELIX_DISPLAY_FBDEV
    SUBMODULE_CFLAGS += -DHELIX_DISPLAY_DRM -DHELIX_DISPLAY_FBDEV
    SUBMODULE_CXXFLAGS += -DHELIX_DISPLAY_DRM -DHELIX_DISPLAY_FBDEV
    # DRM backend linker flags are added in Makefile's cross-compile section
else ifeq ($(DISPLAY_BACKEND),fbdev)
    CFLAGS += -DHELIX_DISPLAY_FBDEV
    CXXFLAGS += -DHELIX_DISPLAY_FBDEV
    SUBMODULE_CFLAGS += -DHELIX_DISPLAY_FBDEV
    SUBMODULE_CXXFLAGS += -DHELIX_DISPLAY_FBDEV
else ifeq ($(DISPLAY_BACKEND),sdl)
    CFLAGS += -DHELIX_DISPLAY_SDL
    CXXFLAGS += -DHELIX_DISPLAY_SDL
    SUBMODULE_CFLAGS += -DHELIX_DISPLAY_SDL
    SUBMODULE_CXXFLAGS += -DHELIX_DISPLAY_SDL
endif

# Evdev input support
ifeq ($(ENABLE_EVDEV),yes)
    CFLAGS += -DHELIX_INPUT_EVDEV
    CXXFLAGS += -DHELIX_INPUT_EVDEV
    SUBMODULE_CFLAGS += -DHELIX_INPUT_EVDEV
    SUBMODULE_CXXFLAGS += -DHELIX_INPUT_EVDEV
endif

# =============================================================================
# Cross-Compilation Build Targets
# =============================================================================

.PHONY: pi ad5m pi-docker ad5m-docker docker-toolchains cross-info

# Direct cross-compilation (requires toolchain installed)
pi:
	@echo "$(CYAN)$(BOLD)Cross-compiling for Raspberry Pi (aarch64)...$(RESET)"
	$(Q)$(MAKE) PLATFORM_TARGET=pi -j$(NPROC) all

ad5m:
	@echo "$(CYAN)$(BOLD)Cross-compiling for Adventurer 5M (armv7-a)...$(RESET)"
	$(Q)$(MAKE) PLATFORM_TARGET=ad5m -j$(NPROC) all

# Docker-based cross-compilation (recommended)
# SKIP_OPTIONAL_DEPS=1 skips npm, clang-format, python venv, and other development tools
pi-docker:
	@echo "$(CYAN)$(BOLD)Cross-compiling for Raspberry Pi via Docker...$(RESET)"
	@if ! docker image inspect helixscreen/toolchain-pi >/dev/null 2>&1; then \
		echo "$(YELLOW)Docker image not found. Building toolchain first...$(RESET)"; \
		$(MAKE) docker-toolchain-pi; \
	fi
	$(Q)docker run --rm -v "$(PWD)":/src -w /src helixscreen/toolchain-pi \
		make PLATFORM_TARGET=pi SKIP_OPTIONAL_DEPS=1 -j$$(nproc)

ad5m-docker:
	@echo "$(CYAN)$(BOLD)Cross-compiling for Adventurer 5M via Docker...$(RESET)"
	@if ! docker image inspect helixscreen/toolchain-ad5m >/dev/null 2>&1; then \
		echo "$(YELLOW)Docker image not found. Building toolchain first...$(RESET)"; \
		$(MAKE) docker-toolchain-ad5m; \
	fi
	$(Q)docker run --rm -v "$(PWD)":/src -w /src helixscreen/toolchain-ad5m \
		make PLATFORM_TARGET=ad5m SKIP_OPTIONAL_DEPS=1 -j$$(nproc)

# Build Docker toolchain images
docker-toolchains: docker-toolchain-pi docker-toolchain-ad5m
	@echo "$(GREEN)$(BOLD)All Docker toolchains built successfully$(RESET)"

docker-toolchain-pi:
	@echo "$(CYAN)Building Raspberry Pi toolchain Docker image...$(RESET)"
	$(Q)docker build -t helixscreen/toolchain-pi -f docker/Dockerfile.pi docker/

docker-toolchain-ad5m:
	@echo "$(CYAN)Building Adventurer 5M toolchain Docker image...$(RESET)"
	$(Q)docker build -t helixscreen/toolchain-ad5m -f docker/Dockerfile.ad5m docker/

# Display cross-compilation info (alias for help-cross)
cross-info: help-cross

# Cross-compilation help
.PHONY: help-cross
help-cross:
	@if [ -t 1 ] && [ -n "$(TERM)" ] && [ "$(TERM)" != "dumb" ]; then \
		B='$(BOLD)'; G='$(GREEN)'; Y='$(YELLOW)'; C='$(CYAN)'; X='$(RESET)'; \
	else \
		B=''; G=''; Y=''; C=''; X=''; \
	fi; \
	echo "$${B}Cross-Compilation & Deployment$${X}"; \
	echo ""; \
	echo "$${C}Docker Cross-Compilation (recommended):$${X}"; \
	echo "  $${G}pi-docker$${X}            - Build for Raspberry Pi (aarch64) via Docker"; \
	echo "  $${G}ad5m-docker$${X}          - Build for Adventurer 5M (armv7-a) via Docker"; \
	echo "  $${G}docker-toolchains$${X}    - Build all Docker toolchain images"; \
	echo "  $${G}docker-toolchain-pi$${X}  - Build Pi toolchain image only"; \
	echo "  $${G}docker-toolchain-ad5m$${X} - Build AD5M toolchain image only"; \
	echo ""; \
	echo "$${C}Direct Cross-Compilation (requires local toolchain):$${X}"; \
	echo "  $${G}pi$${X}                   - Cross-compile for Raspberry Pi"; \
	echo "  $${G}ad5m$${X}                 - Cross-compile for Adventurer 5M"; \
	echo ""; \
	echo "$${C}Pi Deployment:$${X}"; \
	echo "  $${G}deploy-pi$${X}            - Deploy binaries + assets to Pi via rsync"; \
	echo "  $${G}deploy-pi-run$${X}        - Deploy and run in foreground"; \
	echo "  $${G}pi-test$${X}              - Full cycle: build + deploy + run"; \
	echo "  $${G}pi-ssh$${X}               - SSH into the Pi"; \
	echo ""; \
	echo "$${C}Deployment Options:$${X}"; \
	echo "  $${Y}PI_HOST$${X}=hostname     - Pi hostname (default: helixpi.local)"; \
	echo "  $${Y}PI_USER$${X}=user         - Pi username (default: from SSH config)"; \
	echo "  $${Y}PI_DEPLOY_DIR$${X}=path   - Deployment directory (default: ~/helixscreen)"; \
	echo ""; \
	echo "$${C}Current Configuration:$${X}"; \
	echo "  Platform target: $(PLATFORM_TARGET)"; \
	echo "  Display backend: $(DISPLAY_BACKEND)"; \
	echo "  SDL enabled: $(ENABLE_SDL)"

# =============================================================================
# Pi Deployment Configuration
# =============================================================================

# Pi deployment settings (can override via environment or command line)
# Example: make deploy-pi PI_HOST=192.168.1.50 PI_USER=pi
# PI_USER defaults to empty (uses SSH config or current user)
# PI_DEPLOY_DIR defaults to ~/helixscreen (full app directory)
PI_HOST ?= helixpi.local
PI_USER ?=
PI_DEPLOY_DIR ?= ~/helixscreen

# Build SSH target: user@host or just host if no user specified
ifdef PI_USER
    PI_SSH_TARGET := $(PI_USER)@$(PI_HOST)
else
    PI_SSH_TARGET := $(PI_HOST)
endif

# =============================================================================
# Pi Deployment Targets
# =============================================================================

.PHONY: deploy-pi deploy-pi-run pi-ssh pi-test

# Deploy full application to Pi using rsync (binary + assets + config + XML)
# Uses rsync for efficient delta transfers - only changed files are sent
deploy-pi:
	@test -f build/pi/bin/helix-screen || { echo "$(RED)Error: build/pi/bin/helix-screen not found. Run 'make pi-docker' first.$(RESET)"; exit 1; }
	@test -f build/pi/bin/helix-splash || { echo "$(RED)Error: build/pi/bin/helix-splash not found. Run 'make pi-docker' first.$(RESET)"; exit 1; }
	@echo "$(CYAN)Deploying HelixScreen to $(PI_SSH_TARGET):$(PI_DEPLOY_DIR)...$(RESET)"
	@echo "  Binaries: helix-screen, helix-splash"
	@echo "  Assets: ui_xml/, assets/, config/"
	ssh $(PI_SSH_TARGET) "mkdir -p $(PI_DEPLOY_DIR)"
	rsync -avz --progress \
		build/pi/bin/helix-screen \
		build/pi/bin/helix-splash \
		ui_xml \
		assets \
		config \
		$(PI_SSH_TARGET):$(PI_DEPLOY_DIR)/
	@echo "$(GREEN)✓ Deployed to $(PI_HOST):$(PI_DEPLOY_DIR)$(RESET)"

# Deploy and run in foreground (kills any existing instance first)
deploy-pi-run: deploy-pi
	@echo "$(CYAN)Starting helix-screen on $(PI_HOST)...$(RESET)"
	ssh -t $(PI_SSH_TARGET) "cd $(PI_DEPLOY_DIR) && killall helix-screen 2>/dev/null || true; ./helix-screen"

# Convenience: SSH into the Pi
pi-ssh:
	ssh $(PI_SSH_TARGET)

# Full cycle: build + deploy + run
pi-test: pi-docker deploy-pi-run
