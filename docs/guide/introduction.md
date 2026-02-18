title: Introduction
description: Overview of json-c, its goals, and its architecture.
---

# Introduction

json-c is a dependency-free JSON library for ISO C. The project is designed around predictable performance, small binaries, and explicit ownership rules so it can be embedded into constrained and high-throughput environments alike.

## Core principles

1. The parser should reject invalid JSON early and report where the failure occurred.
2. The DOM should be simple enough to audit and extend without hidden behavior.
3. Serialization should support compact and human-readable output.
4. Allocation and lifetime rules should remain explicit at every layer.

## Architectural layers

- Parser: turns bytes into a JSON tree or streams events to a SAX-style callback surface.
- DOM: provides editable tree nodes for arrays, objects, strings, numbers, booleans, and null.
- Serializer: converts values back to text or buffered output.
- Validation: enforces depth, duplicate-key, and UTF-8 policies.
- Memory: centralizes allocator selection, statistics, and optional pooling.

## Supported usage styles

- parse once, inspect many times
- build values programmatically
- transform and merge JSON documents
- validate documents before accepting them from untrusted sources
- format output for humans or compact transport

