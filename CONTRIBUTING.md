# Contributing Guidelines

This project is developed by a single engineer but follows a team-grade workflow: every change is
traceable, reviewed, tested on hardware and documented. These rules apply to humans and AI assistants alike.

---

## 1. Branching model

`main` is always buildable, flashable and tested. Nothing is committed to `main` directly.

| Branch | Purpose | Example |
|---|---|---|
| `main` | Stable, released code. Protected. | — |
| `feat/<short-name>` | New functionality | `feat/bme280-driver` |
| `fix/<short-name>` | Bug fix | `fix/i2c-timeout-hang` |
| `refactor/<short-name>` | Restructuring without behaviour change | `refactor/split-hal-layer` |
| `docs/<short-name>` | Documentation only | `docs/wiring-table` |
| `test/<short-name>` | Tests / measurement tooling | `test/fsm-transitions` |
| `chore/<short-name>` | Build, config, tooling | `chore/cppcheck-misra` |

- Branch names: lowercase, hyphen-separated, short.
- One branch = one roadmap step or one logical change. Keep branches short-lived.
- Delete the branch after merge.

---

## 2. Commit messages — Conventional Commits

Format:

```
<type>(<scope>): <summary>

<body: what and why, wrapped at 72 chars>

<footer: Refs/Closes/BREAKING CHANGE>
```

**Types:** `feat`, `fix`, `refactor`, `perf`, `test`, `docs`, `build`, `ci`, `chore`, `revert`

**Scopes** (module names): `bme280`, `mpu6050`, `lcd`, `i2c`, `uart`, `gpio`, `fsm`, `app`, `config`,
`rtos`, `can`, `boot`, `docs`, `build`

**Rules**
- Summary: imperative mood ("add", not "added"), lowercase, no trailing period, ≤ 50 characters.
- Body explains **why**, not just what. Mention root cause for fixes.
- One logical change per commit. Code must build at every commit.
- Hardware test evidence goes in the PR, not necessarily in every commit.
- Never commit secrets (Wi-Fi passwords, keys) — use `sdkconfig` defaults or untracked config files.

**Examples**

```
feat(bme280): add forced-mode temperature/pressure/humidity read

Uses forced mode instead of normal mode so the sensor sleeps between
samples, matching the 1 s logging period.

Refs: Level 1 Step 5
```

```
fix(i2c): add 50 ms timeout to prevent hang on sensor disconnect

Root cause: i2c_master_transmit_receive was called with portMAX_DELAY,
so a disconnected sensor blocked the main loop forever.

Closes #7
```

```
refactor(fsm): move state transitions into single dispatch function
```

---

## 3. Pull requests

Every change reaches `main` through a PR, even when working alone. The PR is the review record.

**Before opening a PR (self-review checklist)**
- [ ] Builds with `-Wall -Wextra` and zero new warnings
- [ ] Static analysis (Cppcheck + MISRA addon) run on changed files; findings fixed or documented
- [ ] Flashed and tested on real hardware
- [ ] Error paths tested (disconnect, timeout, invalid input)
- [ ] Measurements updated where relevant (RAM/Flash, timing)
- [ ] README / `docs/` updated
- [ ] Diff re-read line by line as if reviewing someone else's code

**PR rules**
- Title follows Conventional Commits: `feat(mpu6050): add accelerometer and gyro read`
- Use the PR template (`.github/pull_request_template.md`).
- Small PRs: ideally < 400 changed lines. Split large work.
- Attach evidence: serial log excerpt, photo, logic analyzer screenshot, measurement table.
- Merge strategy: **squash and merge**; the squash commit message follows Conventional Commits.
- Wait at least a few hours (or until the next day) before merging non-trivial PRs and re-review
  with fresh eyes — the solo substitute for a second reviewer.

---

## 4. Issues

Use issues to track roadmap steps, bugs and ideas.

- **Labels:** `type: feature`, `type: bug`, `type: docs`, `type: test`, `level: 1` … `level: 6`,
  `priority: high`, `blocked: hardware`
- **Bug reports include:** hardware setup, firmware version/commit, steps to reproduce, expected vs
  actual behaviour, log excerpt.
- Each roadmap level can be a GitHub **Milestone**.

---

## 5. Versioning and releases

[Semantic Versioning](https://semver.org): `MAJOR.MINOR.PATCH`

- `0.x.y` during development of a level; `1.0.0` when the level's success criteria are met.
- `MINOR` for new features, `PATCH` for fixes.
- Tag releases on `main`: `git tag -a v0.3.0 -m "v0.3.0: MPU6050 driver"`.
- Firmware prints its version and build date at boot.
- `CHANGELOG.md` follows [Keep a Changelog](https://keepachangelog.com) with sections
  Added / Changed / Fixed / Removed.

---

## 6. Code standards

- Embedded C rules: see `docs/coding-standard.md` (based on MISRA C:2025, CERT C, NASA/JPL Power of 10).
- Formatting: `.clang-format` in repo root; format before committing.
- Comments and identifiers in English. Explain *why*, not *what*.
- Deviations from the coding standard are recorded in `docs/deviations.md` with rule, location and reason.

---

## 7. Documentation

Every repo's `README.md` contains: problem and goal, hardware list, toolchain, architecture diagram,
pin/wiring table, protocol/message format, build/flash/test steps, test scenarios and results,
measured RAM/Flash/CPU/timing, known issues and limitations, future work.

Architecture decisions are recorded as ADRs in `docs/adr/`.
