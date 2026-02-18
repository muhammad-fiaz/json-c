---
title: Security Policy
description: Security guidance for json-c.
---

# Security Policy

Security-sensitive input should be parsed with strict validation and bounded depth.

## Guidance

- validate before accepting untrusted JSON
- set explicit size and depth limits
- treat duplicate keys according to your application policy
- report vulnerabilities privately
