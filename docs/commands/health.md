# Project health: health

The `health` command is intended to analyze project health. The separate `score` command calculates a project health score, while `metrics` and `stats` expose related measurements.

```sh
forge health
forge health --help
```

Treat scores and summaries as heuristics unless the project defines a documented scoring methodology. They should support engineering judgment rather than replace code review, tests, security review, or a reproducible build.