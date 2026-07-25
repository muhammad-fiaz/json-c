---
title: Internals
description: Internal design notes for json-c.
---

# Internals

This page describes the current implementation priorities and the design constraints that shape the codebase.

## Priorities

- keep value ownership explicit
- avoid hidden global mutable state
- keep parser recursion and path tracking simple
- keep the DOM representation compact
- preserve simple error reporting paths
- keep serialization allocation predictable
