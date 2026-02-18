---
title: Performance
description: Performance characteristics, memory behavior, and optimization strategy for json-c.
---

# Performance

json-c is designed to stay small and predictable. The aim is not only speed, but also good behavior under memory pressure and stable latency under repeated workloads.

## Goals

- minimize temporary allocations
- keep parser state compact
- make object and array operations cache-friendly
- avoid hidden global locks
- make validation costs explicit

## Recommended optimization strategy

1. Prefer compact output when size matters.
2. Reuse caller-owned allocators for embedded environments.
3. Reserve object and array capacity when sizes are known.
4. Enable stricter validation only where needed.
5. Benchmark on representative payloads instead of synthetic micro-input alone.

## Benchmark dimensions

- parse throughput
- serialization throughput
- object lookup latency
- array growth behavior
- memory consumption per parsed node
