**I only push stuffs after fixing all bugs so that don't worry while cloning this repo, just hit up a star ⭐ if you liked this project** 
# Forge

**Developer Project Diagnostics & Repair CLI**

Forge is a C++ developer tool that analyzes software projects, detects common configuration and dependency problems, and provides actionable diagnostics and fixes.

> **Current release: v0.1.0 — First Public MVP**

Forge is being built with a long-term goal: make project setup, diagnosis, and maintenance dramatically easier for developers.

---

## ✨ What Forge Does

Forge inspects a project and builds a picture of its development environment.

It can currently detect:

* 📁 Project files
* 🐙 Git repositories
* 🟢 Node.js projects
* 📦 Package managers
* ⚛️ Frameworks
* 📚 Dependencies
* 📜 npm scripts
* 🧩 Dependency health
* 💻 Development environment availability
* 🩺 Project health
* ⚠️ Potential project issues
* 🔧 Automatically fixable problems

---

## 🚀 Example

Run Forge inside a project:

```bash
forge project
```

Example output:

```text
Forge Project
--------------------------------------------

Project: web-ui
Path:    D:\cyber_demo\web-ui

Detected:
  [OK] Git repository
  [OK] package.json
  [OK] package-lock.json

Git:
  [OK] Repository detected
  Root: D:\cyber_demo

Project Type:
  Node.js

Package Manager:
  npm

Frameworks:
  [OK] React
  [OK] Create React App

Dependencies:
  [OK] react ^19.2.4
  [OK] react-dom ^19.2.4
  [OK] axios ^1.13.6

Dependency Health:
  [OK] node_modules found
  Installed: 9
  [OK] All detected dependencies installed

Scripts:
  [OK] start -> react-scripts start
  [OK] build -> react-scripts build
  [OK] test -> react-scripts test

Environment:
  [OK] Node.js detected
  [OK] npm detected

Health:
  [OK] Dependencies detected
  [OK] Package manager detected
  [OK] Node.js available
  [OK] npm available
  [OK] Dependencies installed
  [OK] Git repository detected

Status:
  [OK] Project looks healthy
  Score: 100%
```

---

## 🔧 Fix Problems Automatically

Forge can detect problems that can be repaired automatically.

For example:

```bash
forge fix
```

If dependencies are missing:

```text
Forge Fix
--------------------------------------------

Project: web-ui

Issues detected:

[ERROR] node_modules not found
  Suggestion: Run: npm install

Fix available:
  [1] Install Node.js dependencies
      Command: npm install

Run npm install now? [y/N]:
```

Forge is designed to move beyond simply **reporting problems** toward actually **helping developers resolve them**.

---

## 🩺 Project Diagnostics

Forge separates project detection from project health analysis.

It can identify issues such as:

```text
[ERROR] node_modules not found
Suggestion: Run: npm install
```

It can also identify dependency-specific warnings:

```text
[WARNING] react-scripts 5.0.1
  WARNING: Legacy Create React App dependency
```

This allows developers to understand not only **what exists in a project**, but also **what may need attention**.

---

## 🧠 Health Score

Forge provides a simple project health score.

For example:

```text
Status:
  [OK] Project looks healthy
  Score: 100%
```

When an issue is detected:

```text
Status:
  [WARNING] Project has 1 issue(s)
  Score: 83%
```

The goal is to give developers a quick overview of project health without manually checking every configuration.

---

## 🛠️ Commands

### `forge project`

Analyze the current project.

```bash
forge project
```

Displays:

* Project information
* Detected files
* Git information
* Project type
* Package manager
* Frameworks
* Dependencies
* Dependency health
* Scripts
* Environment
* Diagnostics
* Health score

### `forge fix`

Detect and repair supported project problems.

```bash
forge fix
```

Forge asks for confirmation before executing available fixes.

### `forge doctor`

Run Forge's environment/project diagnostics.

```bash
forge doctor
```

---

## 🏗️ Architecture

Forge is written in **C++** and uses **CMake** as its build system.

The project is organized around separate responsibilities:

```text
Forge
│
├── CLI
│   └── Command handling
│
├── Core
│   └── Project detection
│
├── Project Analysis
│   ├── Project type
│   ├── Frameworks
│   ├── Dependencies
│   ├── Scripts
│   └── Git
│
├── Diagnostics
│   └── Project issues
│
└── Fixes
    └── Automatic repairs
```

This architecture is intended to allow Forge to grow into a much larger developer platform without putting all functionality into one command.

---

## 📦 Building From Source

### Requirements

Currently, building Forge from source requires a C++17-compatible compiler and CMake.

On Windows, the project has been developed and tested using:

* C++
* CMake
* MinGW/MSYS2
* UCRT64

### Clone

```bash
git clone https://github.com/SuprimOjha/forge.git
cd forge
```

### Configure

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

The resulting executable will be located in the build directory.

---

## 🖥️ Windows

Forge currently runs as a command-line developer tool on Windows.

Example:

```powershell
.\build\forge.exe project
```

The current public release is distributed as source code through GitHub.

> A dedicated Windows installer and package-manager distribution are intentionally not part of v0.1.0.

---

## 📈 Roadmap

Forge v0.1.0 is only the beginning.

### Phase 1 — Project Intelligence

* [x] Project detection
* [x] Git detection
* [x] Package manager detection
* [x] Framework detection
* [x] Dependency detection
* [x] Script detection
* [x] Environment detection
* [x] Dependency health
* [x] Project diagnostics
* [x] Health scoring

### Phase 2 — Automated Repair

* [x] Detect fixable problems
* [x] Interactive fix confirmation
* [x] Dependency installation
* [ ] More automatic repairs
* [ ] Safer repair planning
* [ ] Repair verification
* [ ] Detailed repair reports

### Phase 3 — Developer Intelligence

* [ ] Better project detection
* [ ] Python project support
* [ ] C/C++ project analysis
* [ ] TypeScript-specific analysis
* [ ] Configuration diagnostics
* [ ] Build-system diagnostics
* [ ] Security-oriented project checks
* [ ] Performance diagnostics

### Phase 4 — Forge Platform

Long-term ideas include:

* Project-wide diagnostics
* Intelligent troubleshooting
* Automated environment setup
* Dependency upgrade assistance
* Build failure analysis
* CI/CD diagnostics
* Developer workflow automation
* Cross-platform support
* A richer plugin/extension architecture

---

## 🎯 Vision

Most developer tools tell you **what went wrong**.

Forge aims to go further:

```text
Detect
   ↓
Understand
   ↓
Diagnose
   ↓
Explain
   ↓
Fix
   ↓
Verify
```

The long-term goal is for Forge to become a developer assistant that can understand the state of a software project and help bring it back to a healthy state with minimal manual work.

---

## 🤝 Contributing

Forge is currently an early-stage project.

Issues, ideas, experiments, and contributions are welcome.

If you find a bug or have an idea for improving Forge, open an issue on GitHub.

---

## 📄 License

See the `LICENSE` file in this repository for licensing information.

---

## ⭐ Project Status

**Forge v0.1.0**

This is the **first public MVP release**.

Forge is actively under development, and its architecture and CLI may change significantly as new capabilities are added.

If you find the project interesting, consider giving it a ⭐ on GitHub and following its development.

---

**Built by Suprim Ojha**
