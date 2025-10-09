# 🍄 MarioGame — Cocos2d-x C++ Platformer

![Cocos2d-x](https://img.shields.io/badge/engine-Cocos2d--x-blue.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Android-lightgrey.svg)

> A simple Super Mario–style platformer built with **Cocos2d-x 3.x** using C++.  
> This project demonstrates how to use TMX tiled maps, physics, and sprite animations in Cocos2d-x.

---

## 🎮 Features

- 🧱 Classic 2D Mario-style gameplay  
- 🧩 TMX tile map support (Tiled Editor)  
- 🔥 Animated sprites with texture atlases  
- 💥 Collision detection & physics simulation  
- 🎵 Background music and sound effects  
- 🚀 Cross-platform: Windows, Android (extendable)

---

## 🧰 Tech Stack

| Component | Description |
|-----------|-------------|
| **Engine** | Cocos2d-x 3.17+ |
| **Language** | C++17 |
| **IDE** | Visual Studio 2022 |
| **Build System** | CMake |
| **Assets** | TMX Maps, PNG Sprites, WAV Audio |

---

## ⚙️ Project Structure

```
MarioGame/
├── Classes/              # Game source code (.cpp / .h)
│   ├── Mario.cpp
│   ├── Hero.cpp
│   ├── ItemFireString.cpp
│   └── ...
├── Resources/            # Images, sounds, tile maps
│   ├── map1.tmx
│   ├── bossBullet.png
│   └── ...
├── cocos2d/             # Linked Cocos2d-x engine
├── proj.win32/          # Visual Studio project folder
│   ├── Debug.win32/
│   └── Release.win32/
├── CMakeLists.txt       # Build configuration
└── README.md            # This file
```

---

## 🧩 Prerequisites

Make sure you have the following installed:

| Tool | Version | Notes |
|------|---------|-------|
| **Visual Studio** | 2022 | Include "Desktop Development with C++" workload |
| **Python** | 2.7.x | Required for Cocos2d-x setup scripts |
| **CMake** | ≥ 3.6 | For generating build files |
| **Cocos2d-x** | 3.17+ | Clone from [cocos2d/cocos2d-x](https://github.com/cocos2d/cocos2d-x) |
| **Visual C++ Redistributable 2012 (x86)** | – | Required for `MSVCR110.dll` |

---

## 🚀 Build Instructions (Windows)

### 1️⃣ Setup Cocos2d-x Engine

```bash
# Clone and setup cocos2d-x
git clone https://github.com/cocos2d/cocos2d-x.git
cd cocos2d-x
python setup.py
python download-deps.py
```

### 2️⃣ Create Project

```bash
cocos new MarioGame -p com.example.mygame -l cpp -d F:\Projects
```

### 3️⃣ Link Engine (if cloned separately)

```bash
mklink /D F:\Projects\MarioGame\cocos2d F:\Projects\cocos2d-x\cocos2d
```

### 4️⃣ Generate Build Files

```bash
cd F:\Projects\MarioGame\proj.win32
cmake -G "Visual Studio 17 2022" -A Win32 ..
```

### 5️⃣ Open Solution

Open `MarioGame.sln` → Select:

```
Configuration: Debug
Platform: Win32
```

Then click ▶ **Start Debugging** (F5)

---

## 🐛 Troubleshooting

### Missing MSVCR110.dll

Download and install **Visual C++ Redistributable for Visual Studio 2012 (x86)**:
- [Microsoft Download Link](https://www.microsoft.com/en-us/download/details.aspx?id=30679)

### CMake Generation Fails

Ensure you're using the correct generator:
```bash
cmake -G "Visual Studio 17 2022" -A Win32 ..
```

### Python Version Issues

Cocos2d-x requires Python 2.7.x for setup scripts. Make sure it's in your PATH.

---

## 🎨 Assets Credits

Game assets are for educational purposes only. Original Super Mario assets are property of Nintendo.

---

## 📝 License

This project is for educational purposes. Cocos2d-x is licensed under the MIT License.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to check the issues page.

---

## 📧 Contact

For questions or support, please open an issue on the repository.

---

## 🔗 Useful Links

- [Cocos2d-x Official Website](https://www.cocos.com/en/)
- [Cocos2d-x Documentation](https://docs.cocos2d-x.org/cocos2d-x/v3/en/)
- [Tiled Map Editor](https://www.mapeditor.org/)
- [CMake Documentation](https://cmake.org/documentation/)
