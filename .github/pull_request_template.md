## Summary
<!-- What does this PR change and why? Link the roadmap step / issue. -->

Refs: Level _ Step _ · Closes #

## Type
- [ ] feat
- [ ] fix
- [ ] refactor
- [ ] test
- [ ] docs
- [ ] build / chore

## Changes
-

## Hardware test
**Setup:** <!-- board, sensors, wiring, power -->

**Result:**
```
<!-- relevant serial log excerpt -->
```

## Error paths tested
- [ ] Sensor/peripheral disconnected
- [ ] Timeout
- [ ] Invalid input / out-of-range data
- [ ] N/A (explain):

## Measurements
| Metric | Before | After |
|---|---|---|
| Flash | | |
| RAM (static) | | |
| Timing / period | | |

## Self-review checklist
- [ ] Builds with `-Wall -Wextra`, no new warnings
- [ ] Cppcheck + MISRA addon run; findings fixed or recorded in `docs/deviations.md`
- [ ] Return values checked, every wait has a timeout
- [ ] No pins/addresses hard-coded outside config header
- [ ] State machine diagram updated (if touched)
- [ ] README / docs updated
- [ ] CHANGELOG updated
- [ ] Diff re-read line by line
