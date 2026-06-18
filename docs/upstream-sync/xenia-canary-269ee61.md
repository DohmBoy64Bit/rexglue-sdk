# Upstream Sync: Xenia Canary `269ee61`

## Source Metadata

| Field | Value |
|-------|-------|
| **Date** | 2026-06-18 |
| **ReXGlue base commit** | `e8ce24fa73cd7c1ede80262c06f34893b7963dbe` (v0.8.0) |
| **Xenia Canary commit** | `269ee6184595d35979136993174c0109efae5c5f` |
| **Xenia Canary branch** | `canary_experimental` |
| **Xenia Canary HEAD message** | `[XAM] Fixed returning error in XamLoaderGetLaunchData` |

## Sync Rules

These rules govern every change imported from Xenia Canary into ReXGlue:

### Legal / Source Discipline

1. **No XDK-derived information.** Xenia's contribution rules explicitly reject XDK-derived information. ReXGlue must maintain the same standard. Every behavioral change must trace to documented guest-observable behavior, not leaked SDK internals.
2. **Every imported behavior must cite source.** Each ported change must reference the Xenia Canary source file and commit hash it was derived from.
3. **Every approximation must be marked.** Any behavior that is guessed, inferred, or approximated must be annotated with `TODO` or `HYPOTHESIS` in the code and documented here.

### Patching Discipline

4. **Never wholesale-replace files.** Import changes file-by-file or function-by-function, preserving ReXGlue's naming, layout, and code conventions.
5. **No game-specific hacks.** Do not import game-specific workarounds from Xenia Canary unless they demonstrate correct general behavior per the Xenia specification.
6. **Document deviations.** Every deviation from Xenia Canary's behavior (intentional or otherwise) must be recorded in this document.

## Deviation Register

| # | Subsystem | Deviation | Rationale | Date |
|---|-----------|-----------|-----------|------|
|   |           |           |           |      |

## Import Log

| Date | Subsystem | File(s) | Canary Commit | Reason | Status |
|------|-----------|---------|---------------|--------|--------|
|      |           |         |               |        |        |
