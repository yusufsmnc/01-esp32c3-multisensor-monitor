# Embedded C Coding Standard

This project follows a pragmatic subset of established safety-critical C guidelines:

- **MISRA C** — rule numbers below use the MISRA C:2012 numbering, which is retained in MISRA C:2023
  and MISRA C:2025. Rule texts here are paraphrased; consult the official MISRA document for exact wording.
- **SEI CERT C Coding Standard**
- **NASA/JPL "Power of 10"** rules for safety-critical code

This is **not** a claim of formal MISRA compliance. It is a documented subset, enforced by compiler
warnings, Cppcheck (with the MISRA addon) and code review. Any intentional violation is recorded in
[`deviations.md`](deviations.md).

**Scope:** project code (`main/`, `components/` written for this project). Vendor code (ESP-IDF, STM32
HAL/LL, CMSIS, FreeRTOS kernel) is excluded from checks.

Rule levels: **Required** — must be followed or formally deviated. **Advisory** — follow unless there
is a documented reason not to.

---

## 1. Types and constants

| # | Rule | Level | Reference |
|---|---|---|---|
| T1 | Use fixed-width integer types (`uint8_t`, `int32_t`, ...) instead of `int`, `short`, `long` for data whose size matters (registers, protocols, buffers). `bool` from `<stdbool.h>`. | Required | MISRA Dir 4.6 |
| T2 | Unsigned integer constants carry a `U` suffix (`0xFFU`, `1U << 3`). | Required | MISRA Rule 7.2 |
| T3 | No implicit conversions between signed/unsigned or narrowing conversions; cast explicitly and only when the value range is known. | Required | MISRA Rules 10.1–10.8, CERT INT31-C |
| T4 | No bitwise operations or shifts on signed values. Shift amount must be less than the operand width. | Required | MISRA Rules 10.1, 12.2, CERT INT34-C |
| T5 | Guard against unsigned wrap-around and signed overflow in arithmetic on external/sensor data. | Required | CERT INT30-C, INT32-C |
| T6 | No magic numbers: pins, addresses, periods, timeouts and limits are named constants in a config header (`app_config.h`). | Required | — |
| T7 | Pointers to data that is not modified are `const`-qualified. | Advisory | MISRA Rule 8.13 |

## 2. Declarations and scope

| # | Rule | Level | Reference |
|---|---|---|---|
| D1 | Functions and objects used in only one file are `static`. | Required | MISRA Rule 8.7 |
| D2 | Declare variables in the smallest possible scope. | Advisory | Power of 10 #6 |
| D3 | Every automatic variable is initialized before it is read. | Required | MISRA Rule 9.1, CERT EXP33-C |
| D4 | Global mutable state is minimized; shared state is accessed through functions and protected (see §5). | Required | — |
| D5 | Each header has an include guard (or `#pragma once`) and declares only the module's public interface. | Required | MISRA Dir 4.10 |

## 3. Control flow

| # | Rule | Level | Reference |
|---|---|---|---|
| C1 | No recursion (direct or indirect). | Required | MISRA Rule 17.2, Power of 10 #1 |
| C2 | No `goto`. | Advisory | MISRA Rule 15.1, Power of 10 #1 |
| C3 | Every loop has a fixed or provable upper bound; every wait on hardware has a timeout. | Required | Power of 10 #2 |
| C4 | Bodies of `if`, `else`, `for`, `while`, `do` are always compound statements `{ }`. Enforced by `.clang-format` (`InsertBraces`). | Required | MISRA Rule 15.6 |
| C5 | An `if ... else if` chain ends with an `else`. | Required | MISRA Rule 15.7 |
| C6 | Every `switch` has a `default` clause; in state machines it leads to the fault state and logs. | Required | MISRA Rule 16.4 |
| C7 | Every non-empty `case` ends with `break` (no implicit fall-through). | Required | MISRA Rule 16.3 |
| C8 | Conditions are essentially Boolean: `if (ptr != NULL)`, not `if (ptr)`. | Advisory | MISRA Rule 14.4 |
| C9 | Functions fit on one screen (~60 lines); split otherwise. | Advisory | Power of 10 #4 |

## 4. Functions and error handling

| # | Rule | Level | Reference |
|---|---|---|---|
| F1 | Every function that can fail returns an error code (`esp_err_t`, `HAL_StatusTypeDef` or a module `*_err_t`). | Required | — |
| F2 | Every non-void return value is checked or explicitly discarded with `(void)` and a comment. | Required | MISRA Rule 17.7, Power of 10 #7 |
| F3 | Public functions validate their parameters (NULL pointers, lengths, ranges). | Required | Power of 10 #7, MISRA Dir 4.11 |
| F4 | Abort-style macros (`ESP_ERROR_CHECK`, `Error_Handler()`, `assert`) only for unrecoverable init failures or true invariants — never for runtime sensor/bus errors. | Required | — |
| F5 | Each peripheral/sensor failure has a defined behaviour: timeout → retry (bounded) → degraded/fault state → log. | Required | MISRA Dir 4.1 |
| F6 | Prefer `static inline` functions over function-like macros. Macro parameters and bodies are fully parenthesized. | Advisory / Required | MISRA Dir 4.9, Rule 20.7 |

## 5. Memory

| # | Rule | Level | Reference |
|---|---|---|---|
| M1 | No dynamic allocation (`malloc`, `calloc`, `free`) in project code. Static buffers with documented sizes. RTOS objects are created at init (static creation APIs preferred). | Required | MISRA Rule 21.3, Power of 10 #3 |
| M2 | All array/buffer accesses are bounds-checked; lengths are passed with buffers. | Required | CERT ARR30-C, STR31-C |
| M3 | No variable-length arrays. | Required | MISRA Rule 18.8 |
| M4 | Protocol frames are serialized byte by byte with defined endianness; never `memcpy` a struct onto the wire. | Required | — |
| M5 | Task stack sizes are documented and verified with the stack high-water mark. | Required | — |

## 6. Interrupts and concurrency

| # | Rule | Level | Reference |
|---|---|---|---|
| I1 | ISRs are short: no blocking calls, no logging/printf, no allocation, no mutexes. Defer work via queue / semaphore / task notification (`...FromISR` APIs). | Required | — |
| I2 | Variables shared between ISR and task/main context are `volatile` **and** accessed atomically or inside a critical section when read-modify-written or wider than the native word. | Required | CERT CON (general) |
| I3 | `volatile` is not used as an inter-task synchronization mechanism; use RTOS primitives. | Required | — |
| I4 | Shared resources between tasks are protected by a mutex (not a binary semaphore) to allow priority inheritance. | Required | — |
| I5 | Each task documents its period, priority, stack size and rationale. Periodic tasks use `vTaskDelayUntil` or timers. | Required | — |
| I6 | The watchdog is fed only from a point that proves the system is healthy. | Required | — |

## 7. State machines

| # | Rule | Level | Reference |
|---|---|---|---|
| S1 | System modes, protocol handling and recovery logic use an explicit FSM; no implicit state spread across Boolean flags. | Required | — |
| S2 | States and events are `enum`s; every state handles every event explicitly (even if only to ignore it). | Required | — |
| S3 | All transitions go through one dispatch function; the state variable is never assigned elsewhere. | Required | — |
| S4 | Every transition is logged; the FSM is documented as a Mermaid `stateDiagram-v2` plus a state/event table in `docs/`. | Required | — |
| S5 | Events from ISRs are queued; the FSM runs only in task/main context. Timeouts are events. | Required | — |

## 8. Preprocessor

| # | Rule | Level | Reference |
|---|---|---|---|
| P1 | Preprocessor use is limited to includes, include guards, simple constants and configuration switches. | Advisory | Power of 10 #8 |
| P2 | No code is disabled with `#if 0` in committed code; delete it (history is in Git). | Advisory | MISRA Dir 4.4 |

## 9. Naming and style

- Formatting: `.clang-format` in the repository root. Run before every commit.
- Files and functions: `module_action()` in `snake_case` — e.g. `bme280_read_temperature()`.
- Types: `snake_case_t` — e.g. `bme280_config_t`, `sys_state_t`.
- Constants and macros: `UPPER_SNAKE_CASE` — e.g. `I2C_TIMEOUT_MS`.
- File-local (`static`) variables: `s_` prefix — e.g. `s_state`.
- Logging: `ESP_LOGx` (or a project log wrapper) with `static const char *TAG = "module";`.
  Message format: `event: detail (err=0x%x)`.
- Comments explain **why**, not what. Every public function has a brief header comment: purpose,
  parameters, return values, side effects.

---

## 10. Enforcement

| Check | Tool / method | When |
|---|---|---|
| Compiler warnings | `-Wall -Wextra` (plus `-Wconversion` where practical); zero new warnings | Every build |
| Formatting | `clang-format --dry-run --Werror` on changed files | Before commit / CI |
| Static analysis | Cppcheck with `--addon=misra` on project code | Before PR / CI |
| Unit tests | Unity/Ceedling (host) for hardware-independent logic | Before PR / CI |
| Review | PR self-review checklist (`.github/pull_request_template.md`) | Every PR |

Findings that are intentionally not fixed are recorded in [`deviations.md`](deviations.md).
