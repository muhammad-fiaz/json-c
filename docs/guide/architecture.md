---
title: Architecture
description: High-level architecture of json-c.
---

# Architecture

json-c is organized around a small number of explicit layers.

## Layers

- Parser: converts UTF-8 text into structured values.
- DOM: stores and edits JSON data in memory.
- Serializer: emits compact or pretty JSON.
- Validation: checks syntax and policy constraints.
- Memory: centralizes allocation behavior.

## Goals

The architecture is intentionally simple so the library stays portable, testable, and easy to audit.
