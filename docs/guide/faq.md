---
title: FAQ
description: Frequently asked questions about json-c.
---

# FAQ

## Why another JSON library?

Because the project is being rebuilt with a specific focus on portability, documentation, and a small dependency-free C surface.

## Is the API stable?

The 0.x line is still evolving, but the public API is intentionally small and conservative. Breaking changes should be documented in release notes.

## Does it support C++?

Yes. The public header can be included from C++ projects directly. If you prefer, wrap it in `extern "C"` in your integration layer.
