---
title: Internals
description: Internal design notes for json-c.
---

# Internals

This page describes the implementation priorities for the first public release.

## Priorities

- keep value ownership explicit
- avoid hidden global mutable state
- keep parser recursion bounded
- keep the DOM representation compact
- preserve simple error reporting paths
