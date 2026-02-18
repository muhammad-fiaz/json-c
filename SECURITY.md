# Security Policy

json-c is designed to process untrusted JSON defensively.

## Supported guidance

- validate input before accepting it from untrusted sources
- enforce size and depth limits in your application
- treat duplicate keys according to your policy
- handle parser errors explicitly
- release parsed values and serialized strings after use

## Reporting a vulnerability

Report security issues privately to the repository owner rather than opening a public issue.

When you report an issue, include:

- a short description of the problem
- the affected input or code path
- the observed behavior
- any proof-of-concept input if available
- whether the issue affects parsing, serialization, or memory handling

## Response expectations

Security reports should be handled before public disclosure. Do not publish exploit details until the issue has been reviewed and addressed.