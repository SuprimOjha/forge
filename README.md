## Soon to be released on windows package installer.!
# Forge CLI

**Modular C++ Developer Diagnostics, Build System & Project Management Engine**

Forge is an open-source, high-performance C++ developer toolchain CLI engineered to analyze software projects, automate build pipelines, diagnose workspace health, and streamline developer workflows across Windows, Linux, and macOS.

> **Current Release: v1.0.0 - Production Release (90-Day Challenge Complete)**

---

## What Forge Does

Forge operates as a central developer command center. Built from the ground up in modern C++17/C++20, it combines project scaffolding, static analysis, performance telemetry, build tools, and automated fix routines into a unified interface.

### Key Capabilities
- **Project Intelligence:** Deep workspace inspection across C++, Node.js, Python, and multi-language repositories.
- **Build & Artifact Management:** Incremental target compilation, dependency graph generation, symbol stripping, and workspace cleaning.
- **Diagnostics & Health:** Workspace safety checks, compiler toolchain validation (`doctor`), and real-time health scoring.
- **Automated Repair:** Interactive detection and resolution of missing dependencies, broken configurations, and environment issues.
- **Telemetry & Benchmarking:** Compilation timing benchmarks, line-of-code metrics, and subcommand usage statistics.
- **Distribution Tooling:** Multi-format packaging (`.zip`, `.tar.gz`), shell alias generation, release-notes generation, and Windows Inno Setup installer automation.

---

## Installation

### Method 1: Windows Package Manager (WinGet)

Install the official package directly from your terminal:

```powershell
winget install SuprimOjha.Forge
