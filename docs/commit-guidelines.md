# Commit guidelines

A useful commit history communicates how a project evolved. Make each commit represent one coherent change, such as adding a guide, correcting an example, or fixing a build.

Recommended style:

```text
docs: add build-from-source guide
docs: clarify command help discovery
build: document CMake requirements
fix: handle invalid command input
```

Do not split an unchanged file into arbitrary commits only to increase the count. Prefer small commits that can be reviewed, reverted, and understood independently. Mention checks in the commit body when useful.