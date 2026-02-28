# Engine Instructions

The engine is a reusable static library.

Structure:
- engine/include → public API headers
- engine/src → implementation

Rules:
- Public headers must compile standalone
- No dependency on app
- Minimize external dependencies
- Prefer opaque pointers for encapsulation
