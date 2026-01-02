# Features

Detailed feature breakdown and comparison with other Klipper touchscreen interfaces.

## Feature Comparison

| Feature | HelixScreen | GuppyScreen | KlipperScreen |
|---------|-------------|-------------|---------------|
| **UI Framework** | LVGL 9 XML | LVGL 8 C | GTK 3 (Python) |
| **Declarative UI** | Full XML | C only | Python only |
| **No Recompile Changes** | XML edits | Need rebuild | Python edits |
| **Memory Footprint** | ~50-80MB | ~60-80MB | ~150-200MB |
| **Reactive Data Binding** | Built-in | Manual | Manual |
| **Theme System** | XML globals | Built-in themes | CSS-like |
| **Embedded Target** | Optimized | Optimized | Heavy |
| **Touch Optimization** | Native | Native | Desktop-first |
| **Responsive Design** | Breakpoints | Multi-resolution | Fixed layouts |
| **Development Status** | Beta | Stable | Mature |
| **Backend** | libhv WebSocket | libhv WebSocket | Python WebSocket |
| **Language** | C++17 | C | Python 3 |
| **Build Time** | ~30s clean | ~25s clean | N/A (interpreted) |
| **First-Run Setup** | Auto-wizard | Manual config | Manual config |
| **G-code Preview** | Layer view | Thumbnails | Thumbnails |
| **Bed Mesh Visual** | 3D gradient | Color-coded | 2D heatmap |
| **Multi-Fan Control** | All fans | Configurable | All fans |

**Legend:** Full support | Partial/Limited | Not available

---

## Printer Control

- **Print Management** - Start, pause, resume, cancel with live progress tracking
- **Print Phase Tracking** - See exactly what's happening during PRINT_START (heating, mesh, purge)
- **Motion Controls** - XYZ jog pad, homing, emergency stop with safety confirmation
- **Temperature Control** - Nozzle/bed presets, custom targets, live graphs, heating animations
- **Fan Control** - Part fan, hotend fan, controller fan, and auxiliary fans
- **Z-Offset** - Baby stepping during prints with real-time adjustment

---

## Multi-Material Support

HelixScreen supports 5 multi-material backends:

| Backend | Systems Supported |
|---------|-------------------|
| **AFC** | Box Turtle (Aero Filament Changer) |
| **Happy Hare** | ERCF, 3MS, Tradrack, Night Owl |
| **Tool Changer** | Prusa/Bambu-style multi-nozzle |
| **ValgACE** | REST API integration |
| **Slot Management** | Color picker, load/unload, Spoolman linking |

---

## Visualization

- **G-code Layer View** - 2D isometric preview with layer slider navigation
- **3D Bed Mesh** - Gradient-colored surface visualization with profile management
- **Print Thumbnails** - Cached previews from slicer metadata (PNG/QOI)

---

## Calibration & Tuning

- **Input Shaper** - Resonance testing with ADXL345/ADXL375 support
- **Bed Mesh** - Profile management, calibration, visualization
- **Screws Tilt Adjust** - Guided bed leveling assistance
- **PID Tuning** - Heater calibration interface
- **Firmware Retraction** - Retraction length, speed, Z-hop configuration

---

## Integrations

| Integration | Description |
|-------------|-------------|
| **HelixPrint Plugin** | Enhanced print phase tracking via Moonraker |
| **Spoolman** | Spool tracking and management |
| **Power Devices** | Smart relay/PSU control |
| **Print History** | Statistics, filtering, job logs |
| **Timelapse** | Recording configuration (plugin required) |
| **Exclude Objects** | Skip objects mid-print |

---

## System Features

- **First-Run Wizard** - 8-step guided setup with auto-discovery
- **20+ Panels** - Home, Controls, Motion, Temps, Filament, Settings, Advanced, and more
- **Light/Dark Themes** - Runtime switching with design token system
- **Responsive Design** - 480×320 to 1024×600+ screen sizes
- **Connection-Aware** - Graceful disconnect handling with auto-reconnect

---

## Target Hardware

- **Raspberry Pi** (Pi 3/4/5, Zero 2 W)
- **BTT Pad 7** / similar touch displays
- **Vendor printer displays** (Creality K1/K1 Max, FlashForge AD5M, etc.)
- **Generic Linux ARM/x64** with framebuffer support
- **Development simulator:** macOS/Linux desktop with SDL2

### Hardware Requirements

| Spec | Minimum | Recommended |
|------|---------|-------------|
| CPU | ARM Cortex-A7+ or x86_64 (500MHz+) | ARM Cortex-A53+ |
| RAM | 128MB | 256MB+ |
| Storage | 50MB app + 20MB assets | 100MB+ |
| Display | 480×320 | 800×480+ |
| Touch | Capacitive or resistive | Capacitive |
| Network | WiFi or Ethernet | WiFi |
