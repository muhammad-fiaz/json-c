---
title: Performance
description: Performance characteristics, memory behavior, and optimization strategy for json-c.
---

# Performance

json-c is designed to stay small and predictable. The aim is not only speed, but also good behavior under memory pressure and stable latency under repeated workloads.

## Goals

- minimize temporary allocations
- keep parser state compact
- make object and array growth predictable
- avoid hidden global locks
- make validation costs explicit

## Recommended optimization strategy

1. Prefer compact output when size matters.
2. Keep documents in memory only as long as needed.
3. Reuse parsed trees when multiple reads or edits are required.
4. Benchmark on representative payloads instead of synthetic micro-input alone.
5. Measure both parse time and serialization time before tuning.

## Benchmark dimensions

- parse throughput
- serialization throughput
- object lookup latency
- array growth behavior
- memory consumption per parsed node

## Current cost profile

- object lookup is linear in the number of members
- arrays grow dynamically as items are added internally by the parser
- strings are copied into owned storage
- serialization grows a buffer as needed and then returns the final text
