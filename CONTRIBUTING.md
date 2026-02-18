# Contributing

Thanks for helping improve json-c.

## Before you submit

- read the documentation for the area you are changing
- keep changes focused and small when possible
- include tests or examples when behavior changes
- preserve the current ISO C API style and ownership rules

## Recommended workflow

1. Fork the repository or create a topic branch.
2. Make the change and keep the build green.
3. Run the docs build and C test suite.
4. Open a pull request with a clear summary of the change.

## What helps most

- parser edge-case fixes
- serializer improvements
- portability fixes
- documentation updates
- regression tests

## Code style

- keep the public API explicit
- avoid hidden globals
- prefer simple, readable C
- document ownership and error handling

## Security-sensitive changes

If a change affects parsing of untrusted input, include a note about the security impact and the validation behavior it changes.