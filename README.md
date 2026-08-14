## I kindly request everyone to not to clone this repo as I am working on this project and it gonna take 6 more months to built an strong version of this!
# Forge

**A blazing-fast, cross-platform C++ developer CLI for understanding, managing, and automating development environments.**

Forge helps developers detect projects, inspect their development environment, and streamline common workflows through one fast, native command-line tool.

> **Status:** 🚧 Early Development

---

## Why Forge?

Modern software projects often depend on a large collection of tools, runtimes, package managers, databases, containers, and frameworks.

A developer may need to check:

* Is Git installed?
* Which version of Python is available?
* Is Node.js installed?
* Is CMake configured correctly?
* What framework is this project using?
* Which dependencies are missing?
* How should this project be built?
* How should the development server be started?

Forge aims to bring these checks and workflows into a single, fast, developer-friendly CLI.

```bash
forge doctor
```

Instead of manually checking your environment, Forge analyzes it for you.

---

## Features

### 🔍 Environment Detection

Detect development tools and runtimes installed on your system.

```text
Forge Doctor
────────────────────────────

Development Tools

✓ Git
✓ CMake
✓ Python
✓ Node.js
✓ C++ Compiler

Environment

✓ Windows x64

Everything looks good.
```

### 🧠 Project Detection

Forge will automatically identify technologies used by a project.

Planned detection includes:

* C / C++
* Python
* JavaScript / TypeScript
* Rust
* Go
* Java
* React
* Next.js
* Django
* Node.js
* Docker
* CMake
* Cargo
* npm / pnpm / yarn

### ⚡ Native Performance

Forge is written in modern C++ with a focus on:

* Fast startup
* Low memory usage
* Efficient filesystem operations
* Native process execution
* Cross-platform compatibility

### 🛠️ Developer Workflows

Planned commands include:

```bash
forge init
forge doctor
forge build
forge test
forge dev
forge clean
```

The goal is to make common development workflows simple and consistent.

---

## Example

Navigate into an existing project:

```bash
cd my-project
```

Run:

```bash
forge doctor
```

Forge analyzes the project and environment:

```text
Forge Doctor
────────────────────────────

Project

✓ Git repository
✓ Next.js project
✓ TypeScript
✓ Docker

Development Environment

✓ Node.js
✓ npm
✓ Git
✓ Docker

Configuration

✓ package.json
⚠ .env.example not found

Project appears healthy.
```

---

## Architecture

Forge is designed around a modular architecture so that new languages, frameworks, tools, and workflows can be added without making the core CLI difficult to maintain.

```text
Forge
│
├── CLI
│
├── Commands
│   ├── doctor
│   ├── init
│   ├── build
│   ├── test
│   ├── dev
│   └── clean
│
├── Core
│   ├── Project Detection
│   ├── Environment Detection
│   ├── Process Management
│   ├── Filesystem
│   └── Configuration
│
└── Platform Layer
    ├── Windows
    ├── Linux
    └── macOS
```

---

## Tech Stack

Forge is built with:

* **C++20**
* **CMake**
* **Git**
* **GitHub Actions**

The project intentionally keeps its core lightweight and minimizes unnecessary dependencies.

---

## Building From Source

### Requirements

* C++20-compatible compiler
* CMake 3.20+
* Git

### Clone

```bash
git clone https://github.com/suprimojha/forge.git
cd forge
```

### Configure

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build --config Release
```

### Run

On Windows:

```powershell
.\build\Release\forge.exe doctor
```

On Linux/macOS:

```bash
./build/forge doctor
```

---

## Roadmap

Forge is currently in early development.

### Phase 1 — Core CLI

* [x] Basic CLI
* [x] `forge doctor`
* [x] Basic tool detection
* [ ] Version detection
* [ ] Better error reporting

### Phase 2 — Project Intelligence

* [ ] Project type detection
* [ ] Language detection
* [ ] Framework detection
* [ ] Package manager detection
* [ ] Configuration validation

### Phase 3 — Developer Workflows

* [ ] `forge init`
* [ ] `forge build`
* [ ] `forge test`
* [ ] `forge dev`
* [ ] `forge clean`
* [ ] Project configuration

### Phase 4 — Cross-Platform

* [ ] Windows
* [ ] Linux
* [ ] macOS
* [ ] Automated builds
* [ ] Release binaries

### Phase 5 — Ecosystem

* [ ] Plugin system
* [ ] Docker integration
* [ ] Database tooling
* [ ] Cloud integrations
* [ ] Community plugins

### Phase 6 — Intelligent Development

* [ ] Advanced project analysis
* [ ] Dependency diagnostics
* [ ] Automated recommendations
* [ ] Optional AI integrations

---

## Contributing

Forge is being built as an open-source project, and contributions are welcome.

Before submitting a pull request:

1. Fork the repository.
2. Create a feature branch.
3. Make your changes.
4. Add or update tests where appropriate.
5. Build the project successfully.
6. Open a pull request describing your changes.

Example:

```bash
git checkout -b feature/project-detection
```

Then:

```bash
git add .
git commit -m "feat: add project detection"
git push origin feature/project-detection
```

---

## Development Principles

Forge follows a few core principles:

**Fast**
Startup and common operations should remain fast.

**Simple**
Commands should be easy to understand and remember.

**Portable**
Forge should work consistently across major desktop operating systems.

**Extensible**
New languages, frameworks, and tools should be easy to integrate.

**Developer-first**
Features should solve real problems rather than add complexity for its own sake.

**Open source**
The core project should remain transparent and community-driven.

---

## License

Forge is released under the **MIT License**.

See [`LICENSE`](LICENSE) for the full license text.

---

## Project Status

Forge is currently an experimental open-source project under active development.

The architecture and command-line interface may change as the project evolves.

If you find Forge useful, consider giving the repository a ⭐ and contributing to its development.

---

**Built with C++ for developers who build things.**
