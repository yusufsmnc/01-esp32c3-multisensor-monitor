# Coding Standard Deviations

Intentional departures from [`coding-standard.md`](coding-standard.md) are recorded here, as done in
MISRA compliance processes. A deviation is only acceptable when following the rule would be impossible
or would make the code less safe or less clear, and the risk is understood and contained.

**Not recorded here:** findings in vendor code (ESP-IDF, STM32 HAL/LL, CMSIS, FreeRTOS kernel), which is
out of scope.

## How to add a deviation

1. Give it the next ID (`DEV-NNN`).
2. Reference the rule (project rule ID and MISRA/CERT reference).
3. Point to the exact location (file and function, or "project-wide").
4. Explain why the rule cannot or should not be followed here.
5. Describe how the risk is mitigated (test, review, runtime check).
6. Mark the code with a comment: `/* Deviation DEV-NNN: <short reason> */`.
7. If a Cppcheck suppression is needed, suppress only that line and reference the deviation ID.

## Deviation log

| ID | Rule | Location | Reason | Mitigation | Status | Date |
|---|---|---|---|---|---|---|
| DEV-001 | — | — | — | — | — | — |

<!--
Example:
| DEV-001 | M1 / MISRA Rule 21.3 | Project-wide (ESP-IDF framework) | ESP-IDF APIs allocate internally (e.g. driver install, task creation); project code itself does not call malloc. | All allocations happen at init; heap minimum free size is logged and checked against a threshold at runtime. | Accepted | 2026-12-01 |
| DEV-002 | F2 / MISRA Rule 17.7 | main/app_display.c: lcd_update() | Return value of lcd_write() ignored in the 100 ms refresh loop; a missed frame is harmless and is overwritten on the next cycle. | Error counter incremented and reported in status log every 10 s. | Accepted | 2026-12-05 |
-->

## Status values

- **Proposed** — under consideration.
- **Accepted** — deviation approved and in effect.
- **Closed** — code changed, deviation no longer needed (keep the row for history).
