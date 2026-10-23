# Metrics and statistics

Forge includes `metrics`, `stats`, `score`, and `bench` commands for measurements, statistics, health scoring, and benchmarking.

```sh
forge metrics
forge stats
forge score
forge bench --help
```

Measurements need context: project size, compiler, hardware, build mode, cache state, and workload can all affect results. For comparisons, use the same workload under similar conditions; avoid presenting one benchmark as a universal performance claim.