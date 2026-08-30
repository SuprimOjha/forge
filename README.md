# Forge CLI

**Modular C++ Developer Diagnostics, Build System & Project Management Engine**

Forge is an open-source, high-performance developer CLI built in modern C++17/C++20. It provides a unified interface for analyzing software projects, managing builds and artifacts, diagnosing development environments, and automating common project maintenance tasks.

**Current Release: v1.0.0 — Production Release**

## What Forge Does

Forge acts as a centralized command-line toolkit for common development and project-management workflows. It is designed to inspect project workspaces, validate development environments, assist with builds, collect project metrics, and automate selected maintenance and repair operations.

### Core Capabilities

* **Project Intelligence** — Inspects workspaces and identifies project structures across C++, Node.js, Python, and multi-language repositories.
* **Build & Artifact Management** — Provides build-related utilities, dependency analysis, artifact management, workspace cleaning, and related project operations.
* **Diagnostics & Health Checks** — Validates compiler toolchains and development environments through commands such as `forge doctor`.
* **Automated Repair** — Detects selected configuration, dependency, and environment problems and provides automated or interactive fixes where supported.
* **Telemetry & Benchmarking** — Provides compilation benchmarks, project metrics, line-of-code analysis, and command usage statistics.
* **Distribution Tooling** — Supports archive generation (`.zip`, `.tar.gz`), shell alias generation, release-note generation, and Windows Inno Setup installer automation.

## Installation

### Download a Prebuilt Binary

Download the appropriate binary for your operating system from the **GitHub Releases** page.

Extract the archive and add the Forge executable to your system `PATH` if you want to invoke it globally.

Once installed, verify the installation with:

```bash
forge --version
```

## Getting Started

Forge provides a command-driven interface, so you can explore the available functionality directly from the CLI.

Run:

```bash
forge --help
```

to view the available commands and usage information.
