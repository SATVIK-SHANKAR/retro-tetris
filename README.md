# 🎮 Retro Tetris (Terminal Edition)

A classic Tetris game implemented in C++ that runs directly in your terminal using the `ncurses` library.  
Enjoy the nostalgic gameplay with a modern, lightweight, and cross-platform build system.

---

## 🚀 About This Project

This project demonstrates a simple terminal-based Tetris game that can be built and run on **macOS** and **Linux** systems. It leverages:

- **C++11** for core gameplay logic
- **ncurses** for terminal graphics and input handling
- **GitHub Actions** for automated **CI/CD** (Continuous Integration & Continuous Deployment)
- **Homebrew Tap** for easy installation on macOS

---

## ⚙️ What Is CI/CD and Why Does It Matter?

**CI/CD** is an automation practice used in software development that helps deliver updates faster and more reliably by automatically:

- **Building** the project on every code change to catch errors early
- **Testing** (if applicable) to verify that the code works as expected
- **Packaging and releasing** binaries for different platforms
- **Uploading** build artifacts as GitHub releases
- **Publishing** these releases to package managers like Homebrew

For this project:

- Every time you push a new **tag** (like `v1.2`), GitHub Actions triggers the build workflow.
- The workflow compiles the game for **macOS** and **Linux** automatically.
- The compiled executables are uploaded as **GitHub Release Assets**.
- The Homebrew Tap formula fetches the latest release binary for easy installation.

This means you don't need to build the game yourself unless you want to — you can just install it easily via Homebrew or build from source.

---

## 📦 Installation

### macOS (Using Homebrew Tap)

1. **Tap the repository**  
   This lets Homebrew know where to find the custom formula.

   ```bash
   brew tap satvik-shankar/tap
   ```

2. **Install Retro Tetris**

   ```bash
   brew install tetris
   ```

3. **Run the game**
   ```bash
   tetris
   ```

Homebrew will download the pre-compiled executable from the GitHub release assets built by CI/CD and install it on your system.

### Linux (Build From Source)

If you are on Linux or prefer to build yourself:

1. **Make sure you have the required tools installed:**

   For Debian/Ubuntu:

   ```bash
   sudo apt update
   sudo apt install g++ libncurses5-dev libtinfo-dev
   ```

2. **Clone the repository:**

   ```bash
   git clone https://github.com/satvikshankar/retro-tetris.git
   cd retro-tetris
   ```

3. **Compile the source code:**

   ```bash
   g++ -std=c++11 -O2 -o tetris tetris.cpp -lncurses -ltinfo
   ```

4. **Run the game:**
   ```bash
   ./tetris
   ```

---

## 🎮 Controls

- **W** — Rotate piece
- **A** — Move piece left
- **D** — Move piece right
- **S** — Fast drop
- **R** — Reset game
- **Q** — Quit game

---

## 🛠 Development & Contribution

### 🪟 Windows Support & Contribution

Currently, Retro Tetris officially supports **macOS** and **Linux**.  
Support for **Windows** is not yet available out-of-the-box due to differences in terminal handling and lack of native `ncurses` support.

However, we welcome community contributions to make a Windows version possible!

### 🧰 Potential Approaches:

- Use **PDCurses**, a Windows-compatible alternative to `ncurses`
- Implement a basic Windows terminal interface using the **Win32 Console API**
- Use **WSL (Windows Subsystem for Linux)** to run the Linux version directly

### 🙌 How You Can Help:

- Fork the repo and try porting the code with `PDCurses` or another method
- Submit a Pull Request (PR) with your working implementation or improvements
- Open an Issue if you encounter platform-specific problems or want to collaborate

Let's bring Tetris to more terminals together!

- The project uses GitHub Actions to automate building and releasing binaries on each new tag.
- Precompiled executables are automatically uploaded as release assets on GitHub.
- The Homebrew Tap formula references these assets so users can install easily on macOS.
- Contributions are welcome! Feel free to submit issues or pull requests for bug fixes, new features, or improvements.

---

## 📄 License

This project is licensed under the MIT License — see the LICENSE file for details.

---

## 🙌 Acknowledgments

- Thanks to the original creators of ncurses and all contributors to open source libraries.
- Inspired by classic terminal games and the timeless fun of Tetris.

**Enjoy playing!** 🎉
