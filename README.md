# Proptical Unreal Engine Plugin
**Practical effects, optically tracked – for free.**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Unreal Plugin](https://img.shields.io/badge/Unreal-5.3%2B-8A2BE2)](https://unrealengine.com)

> The open-source motion capture system that turns any prop, hand, or body into a perfectly tracked practical effect using $12 ESP32-CAMs, Wi-Fi, UWB anchors, and your choice of state-of-the-art 3D pose models.

Proptical Unreal Engine Plugin provides VRPN-based motion capture integration for Unreal projects.

**Cross-Platform Support:** This plugin is part of a cross-platform SDK. See also: [Unity Package](../../Proptical_Unity/README.md)

---

## 📋 Overview

Proptical Unreal Engine Plugin provides VRPN-based motion capture integration for Unreal projects.

The open-source motion capture system that turns any prop, hand, or body into a perfectly tracked practical effect using $12 ESP32-CAMs, Wi-Fi, UWB anchors, and your choice of state-of-the-art 3D pose models.

Proptical is the **Blender of mocap** – a single local server that speaks VRPN, OSC, and UDP, works with Unity and Unreal out of the box, and lets artists choose between fiducial markers, markerless bodies, or semantic single-point tracking with a single config toggle.

---

## Table of Contents

<details>
<summary><strong>THIS README</strong></summary>

- [Overview](#-overview)
- [Features](#-features)
- [Installation](#-installation)
- [Quick Start](#-quick-start)
- [Documentation](#-documentation)
- [Requirements](#-requirements)
- [Building from Source](#-building-from-source)
- [Roadmap](#-roadmap)
- [Support](#-support)
- [License](#-license)
- [Contributing](#-contributing)
- [Credits](#-credits)

</details>

<details>
<summary><strong>OTHER READMES IN THIS PROJECT</strong></summary>

- [Unity Package](../../Proptical_Unity/README.md)

</details>

---

## ✨ Features

* **VRPN Server Integration** - Drop-in Vicon/OptiTrack replacement using official VRPN library with named rigid bodies and skeletons
* **Native C++ Core + Blueprint Wrappers** - Unreal-optimized C++ implementation with Blueprint-friendly nodes
* **Fiducial Tracking** - OpenCV ArUco / ChArUco sub-mm precision tracking
* **Markerless Body Tracking** - Outside-in multi-person tracking via NIM containers (MMPose, MediaPipe, CLIFF, MvP)
* **Markerless Single-Point Tracking** - Inside-out 3D transforms via NIM containers (KP3D, OnePose, NOPE, VideoPose3D)
* **UWB Fusion** - Kalman filter-based 2D world-map fusion with anchor calibration
* **Auto-Calibration** - One-tap ChArUco board wave calibration
* **OSC/UDP Support** - Direct OSC and UDP communication alongside VRPN
* **Unreal Demo Map** - Example map demonstrating prop tracking with sword mesh following tracked prop 1:1
* **Blueprint Nodes** - Visual scripting support for all tracking features

---

## 📦 Installation

### Via Git Submodule (Recommended)

1. In your Unreal project, navigate to the `Plugins` folder
2. Add this repository as a submodule:
   ```bash
   git submodule add https://github.com/ajcampbell1333/Proptical.git Proptical
   ```
3. Regenerate Visual Studio project files (right-click `.uproject` file > Generate Visual Studio project files)
4. Open the project in Unreal Editor
5. The plugin should be automatically enabled

### Manual Installation

1. Download or clone this repository
2. Copy the `Proptical` folder into your Unreal project's `Plugins` folder
3. Regenerate Visual Studio project files
4. Open the project in Unreal Editor
5. Enable the plugin in **Edit > Plugins** if needed

---

## 🚀 Quick Start

1. Install the Proptical Server and configure your ESP32-CAMs
2. Start the Proptical Server with your desired tracking mode
3. Import the Proptical plugin into your Unreal project
4. Add the VRPN Tracker Actor or Component to your level
5. Configure the tracker to connect to your Proptical Server
6. Your Actor/Mesh will now follow the tracked prop/body in real-time

---

## 📚 Documentation

- [Getting Started Guide](docs/GettingStarted.md)
- [API Reference](docs/API.md)
- [Blueprint Examples](docs/BlueprintExamples.md)
- [C++ Examples](docs/CPPExamples.md)

---

## ⚙️ Requirements

- Unreal Engine 5.3 or later
- Visual Studio 2022 (for C++ development)
- Proptical Server running on local network
- ESP32-CAM hardware (for tracking)

---

## 🔨 Building from Source

1. Clone this repository into your project's `Plugins` folder
2. Right-click the `.uproject` file and select **Generate Visual Studio project files**
3. Open the solution in Visual Studio
4. Build the project in **Development Editor** configuration
5. Launch the project from Visual Studio or Unreal Editor

---

## 🗺️ Roadmap

<details>
<summary><strong>0.0.1 – Pre-Alpha ("It tracks one thing really, really well")</strong></summary>

- 🚧 Create public GitHub repo + MIT license + initial folder structure
- 🚧 Write and freeze the 0.0.1 README
- 🚧 Basic plugin structure and module setup
- 🚧 VRPN client integration for Unreal
- 🚧 Unreal demo map – sword mesh follows tracked prop 1:1
- 🚧 CI – GitHub Actions build and test
- 🚧 Release 0.0.1 – GitHub Release + plugin package

</details>

<details>
<summary><strong>0.1.0 – Pre-Alpha</strong></summary>

- 📋 First public release – single-prop inside-out + outside-in rigid bodies
- 📋 Documentation updates
- 📋 Example maps

</details>

<details>
<summary><strong>0.2.0–0.9.x – Pre-Alpha</strong></summary>

- 📋 Markerless bodies (outside-in)
- 📋 UWB fusion + auto-calibration
- 📋 Multi-person support
- 📋 Blueprint node library
- 📋 Performance optimizations

</details>

<details>
<summary><strong>1.0.0 – Alpha</strong></summary>

- 📋 Full multi-person outside-in, stable inside-out point tracking, production-ready plugin
- 📋 Full documentation
- 📋 Comprehensive test coverage
- 📋 Marketplace submission ready

</details>

<details>
<summary><strong>1.x.x – Alpha</strong></summary>

- 📋 Hand/face tracking
- 📋 Live retargeting UI
- 📋 Performance improvements
- 📋 Community feedback integration
- 📋 Blueprint examples

</details>

<details>
<summary><strong>2.0.0+ – Future</strong></summary>

- 📋 SMPL-X meshes
- 📋 Real-time denoising
- 📋 Mobile companion app integration
- 📋 API stability
- 📋 Long-term support
- 📋 Unreal Engine Marketplace listing

</details>

**Note:** Use ✅ Implemented icon for completed tasks.

---

## 💬 Support

* **Issues:** github.com/ajcampbell1333/Proptical/issues
* **Discussions:** github.com/ajcampbell1333/Proptical/discussions
* **Unity Version:** github.com/ajcampbell1333/Proptical_Unity

---

## 📄 License

Copyright (c) 2025 AJ Campbell

Licensed under the MIT License. See LICENSE for details.

---

## 🤝 Contributing

Proptical is open-source under the MIT License. Got ideas for how to make Proptical better? Contributions are welcome!

<details>
<summary><strong>Development Workflow</strong></summary>

1. Fork this repository and clone it (`git clone https://github.com/your-username/Proptical.git`)
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Add your changes (`git add .`)
4. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
5. Push to the branch (`git push origin feature/AmazingFeature`)
6. Open a Pull Request

</details>

<details>
<summary><strong>Code Standards</strong></summary>

* Follow Unreal Engine Coding Standard
* Add XML documentation comments to public APIs
* Write Blueprint-friendly functions where appropriate
* Test with both C++ and Blueprint workflows

</details>

<details>
<summary><strong>What We Welcome</strong></summary>

* New model containers
* Better ESP32 firmware
* Unreal Blueprint nodes
* Calibration UI improvements
* Demo maps (sword fights, virtual production, etc.)
* C++ optimizations

</details>

---

## 👤 Credits

Created by **AJ Campbell**.

---

_Proptical: because practical effects should stay practical, and tracking should be free._
