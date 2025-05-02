# 🌬️ wind

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/std/the-standard) [![Windows](https://img.shields.io/badge/Platform-Windows-blue.svg)](https://microsoft.com/windows) [![DirectX 11](https://img.shields.io/badge/DirectX-11-blue.svg)](https://www.microsoft.com/en-us/download/details.aspx?id=17431)

> **Minimal, high-performance DirectX 11 + Dear ImGui application skeleton for Windows.**

---

## ✨ Features

- ⚡ **Ultra-minimal**: Clean, modular C++17 codebase
- 🖼️ **DirectX 11**: Modern graphics backend
- 🪟 **Win32 API**: Native window management
- 🎨 **Dear ImGui**: Immediate-mode GUI, easily extensible
- 🧩 **Modular**: Window, Renderer, and Interface classes
- 🛠️ **Easy Build**: CMake + batch script for one-command setup
- 🕶️ **Hidden Window**: Starts invisible, perfect for overlays or background tools

## 🚀 Quick Start

### Prerequisites
- **Windows**
- [Visual Studio](https://visualstudio.microsoft.com/) (with C++ Desktop Development tools)
- [DirectX SDK (June 2010)](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
- [CMake](https://cmake.org/)

### Build (One Command)
```sh
build.bat
```

Or manually:
```sh
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

The executable will be in `build/Release/`.

---

## 🗂️ Project Structure

```
wind/
├── main.cpp            # Entry point, WinMain loop
├── utils/
│   ├── window.*        # Window creation & management
│   ├── renderer.*      # DirectX 11 rendering
│   └── interface.*     # ImGui interface logic
├── imgui/              # Dear ImGui core & backends
├── build.bat           # Windows build script
├── CMakeLists.txt      # CMake project file
└── .gitignore
```

---

## 🛠️ Extending
- Add your ImGui widgets in `utils/interface.cpp`
- Customize rendering in `utils/renderer.cpp`
- Use the modular structure to add new features or backends

---

## 🤝 Credits
- [Dear ImGui](https://github.com/ocornut/imgui) by Omar Cornut
- [Microsoft DirectX 11](https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-graphics)

---

## 📄 License

Copyright (c) hiraeeth

> Everyone is free to copy, modify, and use this code for any purpose, **without any requirement to credit the original author**.
> 
> _"No credit needed, just build cool things."_

---

> Made with ❤️ by hiraeeth 