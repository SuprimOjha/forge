# Forge CLI

**Modular C++ Developer Diagnostics, Build & Project Management CLI**

Forge is an open-source developer CLI built in modern **C++17/C++20** for inspecting software projects, diagnosing development environments, managing builds and artifacts, and automating common project workflows.

The project is designed around a modular architecture so individual capabilities can evolve independently while sharing a common command-line interface and core infrastructure.

## Overview

Modern software projects often require developers to switch between multiple tools for project inspection, environment diagnostics, builds, cleanup, metrics, packaging, and maintenance.

Forge brings a collection of these workflows into a single native CLI.

```text
                    Forge CLI
                       │
        ┌──────────────┼──────────────┐
        │              │              │
    Diagnostics      Build         Project
        │           Management      Analysis
        │              │              │
        └──────────────┼──────────────┘
                       │
                Core Infrastructure
```

## Features

### Project Intelligence

Forge can inspect project workspaces and identify common project structures across multiple ecosystems.

Currently supported project types include:

* C++
* Python
* Node.js
* Multi-language repositories

Project detection is designed to provide Forge commands with the context they need to operate on the workspace.

### Diagnostics

The `doctor` command provides development-environment diagnostics and validation.

Example:

```bash
forge doctor
```

Depending on the detected environment, Forge can inspect areas such as:

* Compiler availability
* Build tool availability
* Project configuration
* Workspace structure
* Development environment issues

### Build & Artifact Management

Forge provides utilities for managing development builds and generated artifacts.

Capabilities include:

* Build operations
* Workspace cleaning
* Artifact management
* Dependency analysis
* Build-related project inspection

The goal is to provide a consistent interface without replacing established build systems.

### Automated Fixes

Forge includes a fix engine for selected project and environment problems.

```bash
forge fix
```

When a supported issue is detected, Forge can provide an automated or interactive resolution.

**Important:** automated modifications should always be reviewed before being used in critical projects.

### Metrics & Benchmarking

Forge includes project and build-related measurements such as:

* Compilation timing
* Lines-of-code metrics
* Project statistics
* Command usage information

These tools are intended to make development workflows more observable and measurable.

### Distribution & Packaging

Forge also includes utilities for preparing projects and releases.

Supported functionality includes:

* `.zip` archive generation
* `.tar.gz` archive generation
* Shell alias generation
* Release-note generation
* Windows Inno Setup installer automation

## Installation

### Prebuilt Binary

The easiest way to install Forge is to download a prebuilt binary from the project's GitHub Releases page.

Extract the downloaded archive and place the Forge executable somewhere available through your system `PATH`.

Verify the installation:

```bash
forge --version
```

Then run:

```bash
forge --help
```

to see the available commands and usage information.

## Quick Start

After installation, start by inspecting the available commands:

```bash
forge --help
```

For environment diagnostics:

```bash
forge doctor
```

For supported automated fixes:

```bash
forge fix
```

The exact commands and options available in your installed version can always be viewed with:

```bash
forge --help
```

## Architecture

Forge is organized as a collection of independent modules built around shared core infrastructure.

A simplified view of the architecture:

```text
forge/
│
├── core/
│   ├── project detection
│   ├── configuration
│   ├── command infrastructure
│   └── shared utilities
│
├── commands/
│   ├── doctor
│   ├── fix
│   ├── build
│   ├── clean
│   └── ...
│
├── diagnostics/
│   ├── environment checks
│   └── project diagnostics
│
├── build/
│   ├── build management
│   └── artifact handling
│
├── metrics/
│   └── project and build measurements
│
└── distribution/
    └── packaging and release tooling
```

The architecture is intentionally modular so new commands and subsystems can be added without tightly coupling the entire application.

## Technology

* **Language:** C++17 / C++20
* **Architecture:** Modular CLI
* **Build:** CMake
* **Platforms:** Windows, Linux, macOS
* **License:** Open Source

## Design Goals

Forge is being developed around several principles:

### Native Performance

Forge is implemented in C++ to provide a lightweight native executable with minimal runtime overhead.

### Modularity

Commands and subsystems should remain independently maintainable and extensible.

### Developer Experience

Common development operations should be accessible through a consistent command-line interface.

### Safety

Operations that modify a project should be explicit and predictable. Forge should avoid destructive behavior unless the user has intentionally requested it.

### Extensibility

The architecture is intended to support additional languages, build systems, diagnostics, and developer workflows over time.

## Current Status

**Version: v1.0.0**

Forge is actively evolving. Some functionality is mature while other areas are still under development.

The current release focuses on establishing the core CLI architecture and providing a foundation for additional developer tooling.

If you encounter a problem, please open an issue with:

* Operating system
* Forge version
* Project type
* Command that was executed
* Relevant output
* Steps to reproduce the issue

## Roadmap

Planned areas of development include:

* Expanded project detection
* More build-system integrations
* Additional diagnostics
* More automated fixes
* Improved dependency analysis
* Expanded testing infrastructure
* Cross-platform improvements
* Additional developer workflow commands

The roadmap may change based on technical findings and community feedback.

## Contributing

Contributions, bug reports, feature requests, and technical feedback are welcome.

Before contributing, please read the project's contribution guidelines and open an issue when discussing substantial changes.

## License

Forge is open source. See the `LICENSE` file for the applicable license and terms.

---

## Getting Started

If you have installed Forge, the best place to start is:

```bash
forge --help
```

This displays the commands available in your installed version and the options required to use them.

**Forge — one CLI for project diagnostics, build workflows, and developer tooling.**
