# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This repository is **lvx-newlib**: a fork of GNU Newlib/Libgloss (imported wholesale from Kalray's KVX newlib tree, see the single commit "Initial import from KVX.") that will be ported to support **LVX**, a new VLIW ISA inspired by (but binary-incompatible with) Kalray's KVX architecture. LVX is **LP64-only** (no 32-bit mode).

**`newlib/libc/machine/lvx/`, `newlib/libm/machine/lvx/`, and (partially) `libgloss/lvx-mbr/` have been ported** (see "Porting Status" below). The libgloss port deliberately excludes files that depend on undefined LVX architecture details (memory map, syscall ABI, timer registers) — see that section for exactly what's missing and why. The `sys/mbr/lock.h`/`mppa_bare_runtime` header blocker mentioned below **has been fixed** (`newlib/libc/sys/mbr/include/mbr/lvx/{atomic.h,cache.h,cpu.h}` now exist — see "Porting Status"). Note: the LVX-side headers/includes use `mbr/lvx/...` (not `mppa_bare_runtime/lvx/...`) — the directory was deliberately renamed from `mppa_bare_runtime` to `mbr` for the LVX side only; the KVX side still uses `mppa_bare_runtime/kvx/...` unchanged, since that's the real external Kalray SDK's actual name.

**Toolchain reliability finding, measured with a full, non-parallel `all-target-newlib` build at the real default flags (`-g -O2 -march=lvx-1 -fdata-sections -ffunction-sections`)**: roughly **90% of newlib's ~1000 source files compile cleanly** (995 attempted, 121 failed in one multilib pass). The failures cluster into distinct, pre-existing `lvx-gcc` bugs, unrelated to anything in this repo — **always measure at `-O2`** (the real build's default), not `-O0`, since at least one of these bugs (see #2 below) is `-O0`-only and vanishes at `-O1` and above; testing at `-O0` alone previously led to an overly pessimistic assessment here that has since been corrected:
1. **`maybe_record_trace_start` ICE** (`dwarf2cfi.cc:2591`) — the single most common failure (180 raw hits across the build). Not yet root-caused.
2. **Bad-mnemonic codegen** — GCC emits instruction mnemonics the LVX assembler doesn't recognize (e.g. `frecw.rn`/`fwidenlwd` in `hash_page.c`, where the assembler suggests `fsrecw`/`fwidenwd`; `cmoved` with an unencodable 64-bit immediate in `w_pow.c`). 502 raw "Unexpected token" hits.
3. **`lvx_expand_builtin` ICE** (`lvx-builtins.cc:2124`) — any `__builtin_lvx_*` call. Rare in practice (only `wf_exp.c` hits it in the whole build) but 100% reproducible whenever a builtin is used, at every optimization level.
4. **"invalid rtl sharing" ICE** — hits `vfprintf.c`/`vfwprintf.c` specifically.
5. **`lvx_analyze_branches` ICE** (`lvx.cc:4835`, "any function with a branch") — confirmed **`-O0`-only**; does not reproduce at `-O1`/`-O2`/`-O3`/`-Os`, so it does not affect the real build at all (0 hits in the full `-O2` build log) despite looking severe when tested naively.

See "Key Porting Gotchas" for how these interact with specific ported files.

The companion repositories are `lvx-binutils` and `lvx-gcc` (siblings under `/home/bd3/lvx-csw/`), which must be built before this newlib can be built/tested. See `/home/bd3/lvx-csw/CLAUDE.md` for the overall LVX project context, target triple (`lvx-mbr`), and toolchain build instructions.

## KVX Reference Source

The upstream KVX newlib this repo was imported from lives at `/home/bd3/Work2/kvx-csw/newlib/newlib/`. When porting a feature, diff against the corresponding path there rather than guessing — the trees are still very close. Example:

```bash
diff -ru newlib/libc/machine/kvx/ /home/bd3/Work2/kvx-csw/newlib/newlib/libc/machine/kvx/
```

## Where KVX Support Lives (the porting templates)

| Path | Role |
|------|------|
| `newlib/libc/machine/kvx/` | Machine-specific libc: `memcpy.S`, `memset.S`, `memcmp.S`, `memmove.S`, `strlen.c`, `strcpy.c`, `strcmp.c`, `setjmp.S`, `arc4random*.c`, `hash_page.c`, `asm.h`, `kvx_defines.h` (KVX builtins/helpers used by the optimized string routines) |
| `newlib/libm/machine/kvx/` | Machine-specific libm: fenv.h implementation (`fesetenv.c`, `feclearexcept.c`, ...), `s_fdim.c`/`sf_fdim.c`, `w_pow.c`/`wf_exp.c` |
| `newlib/libc/sys/kvx/` | KVX-specific OS glue (used only by the `kvx-*-elf` target): `lock.c`, `include/sys/lock.h` |
| `newlib/libc/sys/mbr/` | "bare MBR runtime" OS glue, shared across machine backends via `sys_dir=mbr` (used by `kvx-*-mbr` and now `lvx-*-mbr` too — no per-machine fork needed in `configure.host`/`Makefile.inc`). Was **not actually machine-generic in content**: `include/sys/lock.h`/`lock.c` hardcoded `mppa_bare_runtime/kvx/{atomic.h,cache.h,...}` — Kalray's proprietary MPPA bare-runtime SDK, not part of this repo. **Fixed** — see "Porting Status" for the `#if defined(__lvx__)` branch and new `mbr/lvx/` headers added (that directory is named `mbr`, not `mppa_bare_runtime`, on the LVX side — see below). |
| `libgloss/kvx-mbr/` | Libgloss for the bare MBR runtime: `crt0.c` (calls `__start1` → `__libc_init_array` → `main` → `exit`), syscall stubs (`open.c`, `read.c`, `write.c`, `sbrk.c`, ...), `asm_syscalls.S`, `crti.c`/`crtn.c`, `linker_scripts/{bare.ld,memory_map.ld,bare_extern.ld}` (KVX physical memory map, boot address, exception vector table layout) |
| `libgloss/kvx-cos/` | Libgloss for Kalray's ClusterOS variant — richer syscall set (`fork.c`, `execve.c`, `mman.c`, `gmon.c`, ...). Not currently in the LVX plan (no `lvx-cos` target mentioned), but consult it if COS support is later requested. |

## Porting Status

### `newlib/libc/machine/lvx/`

Ported from `newlib/libc/machine/kvx/` and validated: every file assembles/compiles standalone against the installed `lvx-mbr-*` toolchain, and building the full `lvx-mbr` newlib tree (`configure --target=lvx-mbr && make`) successfully produces `libc/machine/lvx/libc_a-{setjmp,memset,memcpy,memmove,memcmp,rpmatch,arc4random,arc4random_uniform}.o` for both the default and `lvx-1` multilib variants. `hash_page.c` gets past the (fixed) `sys/mbr/lock.h` header cleanly at `-O2` but still doesn't build — it hits the bad-mnemonic codegen bug (#2 at the top of this file: GCC emits `frecw.rn`/`fwidenlwd`, which the assembler rejects, suggesting `fsrecw`/`fwidenwd` instead) — nothing wrong with the file itself. Notes on how each file was adapted:

- `asm.h`, `memcpy.S`, `memmove.S`, `memcmp.S`, `setjmp.S`, `hash_page.c` — copied verbatim; none contain KVX-specific text, and every mnemonic they use (`sq`/`so`/`lo`/`lq`, `cmoved`, `loopdo`, `ldcond`/`stcond` macros, etc.) exists unchanged in `lvx-binutils/opcodes/lvx-opc.c`.
- `memset.S` — the `#ifdef __kv3_1__` fast-zero path (using the `dzerol` cache-zeroing instruction) was **dropped**: `dzerol` doesn't exist in the LVX opcode table, and LVX-1 (a kv4-1 simplification) never had it either — kv4-1 already used the same generic `sbmm8d`-based fallback path unconditionally, which is what's kept.
- `lvx_defines.h` (renamed from `kvx_defines.h`), `strlen.c`, `strcpy.c`, `strcmp.c` — `__builtin_kvx_*` renamed to `__builtin_lvx_*` (`bitcntd`, `storecb/storech/storecw/storecd` all exist in `lvx-gcc/gcc/config/lvx/lvx-builtins.def` with identical signatures). **Known blocker**: every `__builtin_lvx_*` call currently crashes the installed `lvx-gcc` with an ICE (`internal_error` in `lvx_expand_builtin` at `lvx-builtins.cc:2124`, hitting `DECL_FUNCTION_CODE`/`DECL_MD_FUNCTION_CODE` assertion) — reproducible with a trivial call to any MD builtin (e.g. `__builtin_lvx_ctzd(a)`), so it's a bug in the sibling `lvx-gcc` repo's builtin-expansion path, not a mistake in these files. Front-end parsing/type-checking of the calls succeeds; only RTL expansion ICEs.
- `include/fenv.h` — copied, only trimmed a Kalray-hardware-specific ("Coolidge processor" dual-core-coprocessor) comment that doesn't apply to LVX.
- `sys/fenv.h` — collapsed the `#if defined(__KV4__)` / `elif defined(__KV3__)` rounding-mode-encoding branch to just the KV4 encoding unconditionally (LVX-1 has no KV3 lineage).
- `rpmatch.c`, `arc4random.c`, `arc4random_uniform.c` — stub files ("Unsupported on k1"), reworded to "Unsupported on lvx-mbr".

### `newlib/libc/sys/mbr/` (the `mbr/lvx` fix)

The user provided the actual KVX `mppa_bare_runtime` SDK source (`/home/bd3/Work2/kvx-csw/mppa_bare_runtime/`), which made this fixable. Key realization from reading it: `newlib/libc/sys/mbr/lock.c` only ever *declares/calls* `__kvx_recursive_*_base`/`__kvx_mb`/`__kvx_get_cpu_id` — their actual instruction-level implementations live in `mppa_bare_runtime`'s own `src/kv4_v1/` (a separate library, linked in only when building a full application). So unblocking **newlib's own compilation** only required porting the *declarations* (types, constants, prototypes), not writing real atomic-instruction implementations — that's a separate, later task for whoever builds an "lvx-bare-runtime" library.

- `newlib/libc/sys/mbr/include/mbr/lvx/{atomic.h,cache.h,cpu.h}` — new headers, ported from `mppa_bare_runtime/include/mppa_bare_runtime/kvx/{atomic.h,cache.h}` (cpu.h is a deliberately minimal subset — just `__lvx_get_cpu_id`, the one declaration actually used in this repo; KVX's full `cpu.h` also covers hwloop/FPU-rounding-mode control that nothing here needs yet). `__kvx_*`→`__lvx_*`, `_KVX_*`→`_LVX_*`, and the containing directory `mppa_bare_runtime`→`mbr` (LVX side only — the KVX side keeps the real SDK's actual `mppa_bare_runtime` name unchanged). Confirmed the underlying instructions exist in LVX with identical mnemonics: `acswapb/h/w/d/q` (atomic CAS), `aladdd`/`aswapd` (atomic add/swap), `fence`/`barrier` (memory barriers), `errop` (trap) — all in `lvx-opc.c`. Also fixed a latent header-guard bug in the KVX original (`cache.h`'s `#ifndef _KVX_CACHE_H` / `#define _KVX_COMMON_CACHE_H` mismatch, which silently disables the include guard on double-inclusion) — used one consistent guard name in the new file since it's new code, not a mirrored file.
- These land under `newlib/libc/sys/mbr/include/`, **not** some external sysroot path — confirmed via `newlib/Makefile.am`'s `stmp-targ-include` rule, which recursively copies everything under `libc/sys/$(sys_dir)/include/` into the build's `targ-include/` staging dir (already on `-isystem`), so `#include <mbr/lvx/atomic.h>` resolves during the newlib build itself with no extra configuration.
- `newlib/libc/sys/mbr/include/sys/lock.h` — added an `#if defined(__lvx__)` branch (`struct __lock` using `__lvx_recursive_lock_t`, `_LVX_DCACHE_LINE_SIZE` alignment) parallel to the existing (untouched) KVX one.
- `newlib/libc/sys/mbr/lock.c` — added a matching `#if defined(__lvx__)` branch. Used macro aliasing (`#define __get_cpu_id __lvx_get_cpu_id` etc.) rather than duplicating the function bodies, so the actual lock-acquire/release logic is written once and shared between architectures. Note: the KVX branch's own includes (`context.h`, `syscall.h`, `scall_no.h`) don't actually declare `__kvx_get_cpu_id` at all — that's only in `cpu.h`, which KVX's `lock.c` never includes; it silently works today only because the real KVX build passes `-std=gnu89 -w` (implicit-declaration-as-warning). Left that quirk alone on the KVX side (not our file to fix), but the new LVX side properly `#include`s `mbr/lvx/cpu.h` so it compiles clean under normal `-std=gnu17`-ish defaults too.
- **Validated**: `hash_page.c`, `s_fdim.c`, `sf_fdim.c`, `w_pow.c`, and `lock.c` itself all now get **past** this header entirely (confirmed zero remaining `mppa_bare_runtime/kvx/atomic.h: No such file` errors in a full `-O2` rebuild). At the real build's `-O2`: `lock.o` and `s_fdim.o`/`sf_fdim.o` compile clean end-to-end for both multilib variants (`lock.c` in particular has real branches/loops — `while`, `for` — and still compiles fine, confirming the separate `lvx_analyze_branches` ICE really is `-O0`-only and doesn't affect it); `hash_page.c` and `w_pow.c` hit the unrelated bad-mnemonic codegen bug (#2 at the top of this file) instead.

### `newlib/libm/machine/lvx/`

Ported from `newlib/libm/machine/kvx/` and validated the same way: `configure --target=lvx-mbr && make` produces `libm/machine/lvx/libm_a-{feclearexcept,fegetenv,fegetexceptflag,fegetround,feholdexcept,feraiseexcept,fesetenv,fesetexceptflag,fesetround,fetestexcept,feupdateenv}.o` **and now also `s_fdim.o`/`sf_fdim.o`** for both multilib variants, since the `sys/mbr/lock.h` fix unblocked them and they have no other issues. `w_pow.c` gets past the header too but hits the bad-mnemonic codegen bug (`cmoved` with an unencodable 64-bit immediate). `wf_exp.c` gets past the header but hits the `lvx_expand_builtin` ICE (it's the only file in the whole newlib tree that actually uses `__builtin_lvx_*`).

- `feclearexcept.c`, `fegetenv.c`, `fegetexceptflag.c`, `fegetround.c`, `feholdexcept.c`, `feraiseexcept.c`, `fesetenv.c`, `fesetexceptflag.c`, `fesetround.c`, `fetestexcept.c`, `feupdateenv.c` — copied verbatim. All are pure C using inline asm (`get %0 = $cs`, `wfxl $cs, %0`) with no KVX-specific identifiers; both mnemonics confirmed present in `lvx-opc.c`.
- `s_fdim.c`, `sf_fdim.c`, `w_pow.c` — copied verbatim. These are architecture-independent overrides of the generic `libm/common`/`libm/math` versions (fix `+-inf` handling, use `__builtin_inf()`), not KVX-specific at all.
- `ml_support_lib.h` — copied verbatim. Kalray's generic Metalibm support header (bit-pattern classification helpers via unions); zero KVX-specific content.
- `wf_exp.c` (metalibm-generated `expf`) — `__builtin_kvx_{fixedw,floatw,ffmsw,ffmaw}` renamed to `__builtin_lvx_*` (all four confirmed in `lvx-builtins.def` with identical signatures). Hits the `lvx_expand_builtin` ICE — same pre-existing `lvx-gcc` bug as `strlen.c`/`strcpy.c`/`strcmp.c` would hit, not a porting mistake.

### `libgloss/lvx-mbr/`

**Partially ported** from `libgloss/kvx-mbr/` — 28 of the 37 source files, plus `Makefile.in` and `aclocal.m4` (the latter is vestigial in both `kvx-mbr` and `lvx-mbr`: no `configure.ac` consumes it, kept only for file-for-file parity). Confirmed working: `configure --target=lvx-mbr` (run from the *top-level* `/home/bd3/lvx-csw/lvx-newlib/configure`, not `libgloss/configure` standalone — the standalone one doesn't know to use the cross tools) successfully generates `lvx-mbr/Makefile`.

Ported (verbatim, no KVX-specific content):
- `crt0.c`, `crti.c`, `crtn.c` (the latter two aren't actually wired into `CRT_FILES` even for `kvx-mbr` — mirrored deliberately, not fixed, same as the KVX-upstream `strlen.c`/`strcpy.c`/`strcmp.c` gap noted above)
- `sbrk.c`, `getpagesize.c`
- ENOSYS stubs: `execve.c`, `fork.c`, `kill.c`, `getpid.c`, `wait.c`
- Plain `sc_*`-calling syscall wrappers (declare `extern int sc_<name>(...)`, defined in the *not-yet-ported* `asm_syscalls.S`, and wrap with errno translation): `access.c`, `chdir.c`, `chmod.c`, `close.c`, `dup.c`, `dup2.c`, `exit.c`, `fstat.c`, `isatty.c`, `link.c`, `lseek.c`, `mkdir.c`, `mkfifo.c`, `read.c`, `rmdir.c`, `stat.c`, `unlink.c`, `write.c`
- `Makefile.in` — `SYS_OBJS` trimmed to just the files that exist (see below); install path `include/kvx` → `include/lvx`.

**Deliberately NOT ported yet** — each depends on an LVX architecture detail that plain KVX→LVX renaming can't invent, and needs a real design decision (same category as the `sys/mbr/lock.h` blocker, just larger in scope):
- `linker_scripts/bare.ld` + `memory_map.ld` — hardcode KVX's physical memory map (`internal_mem` at `0x0`/4M, `dsu` at `0xA46000`, `ddr` at `0x100000000`), boot address, and exception-vector-table offsets (debug/trap/interrupt/syscall at fixed KVX offsets). No LVX memory map or exception vector layout is defined anywhere in this project yet. (`bare_extern.ld` is trivial and could be ported, but is pointless without the other two.)
- `asm_syscalls.S`, plus the `open.c`/`fcntl.c` wrappers that need the same header — the actual `scall __NR_*` syscall trap uses numbers from `mppa_bare_runtime/kvx/scall_no.h`, a proprietary numbering agreed upon by the KVX ISS/simulator. There's no LVX ISS yet (per `/home/bd3/lvx-csw/CLAUDE.md`) to define an equivalent.
- `timer.c`, `times.c`, `gettimeofday.c`, `nanosleep.c` (and transitively `sleep.c`/`usleep.c`, which just call `__kvx_nanosleep`) — read KVX hardware timer/counter SFRs via `mppa_bare_runtime/kvx/{timer,cpu,timestamp,diagnostic,io,power_ctrl}.h`. Same story: no LVX equivalent register set is defined.

Once an LVX syscall ABI and memory map exist, port these the same way as everything else here, add their objects back into `Makefile.in`'s `SYS_OBJS`, and un-skip the `linker_scripts/*.ld` install loop.

## Build System Wiring (done for `libc/machine/lvx`, `libm/machine/lvx`, and `libgloss/lvx-mbr`)

These dispatch points were updated to mirror the existing `kvx` entries:

- `newlib/configure.host` — added an `lvx)` case (`machine_dir=lvx`, `libm_machine_dir=lvx`, `default_newlib_io_long_long=yes`, `default_newlib_io_c99_formats=yes`, `have_init_fini=no`) and an `lvx-*-mbr)` case (`sys_dir=mbr`, `syscall_dir=syscalls`, `have_crt0=no`) alongside the existing `kvx-*-mbr)` one.
- `newlib/libc/machine/lvx/{Makefile.am,Makefile.inc,configure.ac,aclocal.m4,configure}` — new autoconf/automake subproject dir, copied from `kvx`'s and renamed (`libc/machine` dirs are each their own autoconf subproject; `libm/machine` dirs are not — see below). Note `Makefile.am`'s `lib_a_SOURCES` intentionally omits `memmove.S`/`memcmp.S`/`strlen.c`/`strcpy.c`/`strcmp.c`, matching a pre-existing gap in KVX's own `Makefile.am` (only `Makefile.inc`, used by the real unified build, lists `memmove.S`/`memcmp.S`; none of the three string routines are wired into either build file even for KVX) — this was mirrored deliberately, not fixed, to keep the port a straight diff against upstream.
- `newlib/libc/machine/Makefile.inc` — added the `if HAVE_LIBC_MACHINE_LVX / include %D%/lvx/Makefile.inc / endif` block.
- `newlib/libc/acinclude.m4` — added `lvx` to the `m4_foreach_w([MACHINE], ...)` registry that generates `HAVE_LIBC_MACHINE_*` conditionals.
- `newlib/libm/machine/lvx/Makefile.inc` — libm machine dirs have **no** `Makefile.am`/`configure.ac`/own `configure` (unlike libc machine dirs) — just a `Makefile.inc` with `%C%_src = ...` + `libm_a_SOURCES += $(%C%_src)`, pulled in via `libm/Makefile.inc`.
- `newlib/libm/Makefile.inc` — added the `if HAVE_LIBM_MACHINE_LVX / include %D%/machine/lvx/Makefile.inc / endif` block (alongside the existing `HAVE_LIBM_MACHINE_KVX` one).
- `newlib/libm/acinclude.m4` — added `lvx` to its own separate `m4_foreach_w([MACHINE], ...)` registry (parallel to, but distinct from, `libc/acinclude.m4`'s).
- `newlib/configure` and `newlib/Makefile.in` (top-level, committed/generated) — regenerated via `autoreconf` using the *exact* versions this tree requires (autoconf 2.69, automake 1.15.1 — automake 1.15.1 isn't packaged by apt on this system and had to be built from source; a newer automake 1.16.5 was tried first but renames the `libc_a-` object prefix to `a-` tree-wide, a huge unrelated diff — don't use it here). Confirmed byte-identical `aclocal.m4` and near-minimal, fully-explained diffs in `configure`/`Makefile.in` each time (just the new `HAVE_LIB{C,M}_MACHINE_LVX` conditionals plus mechanical `am__append_N`/`am__objects_N` renumbering and brand-new per-object compile-rule stanzas for the new files — verified by diffing the sorted, number-normalized set of changed lines against itself, not just eyeballing).
- `config.sub` (top-level, vendored copy) — added `lvx` to the recognized CPU list; `lvx-binutils/config.sub` already had this patch, `lvx-newlib`'s own vendored copy didn't.
- `newlib/libc/include/machine/ieeefp.h` — added an `#ifdef __lvx__` branch (little-endian, same `FP_ILOGB0`/`FP_ILOGBNAN` as KVX) alongside the existing `__kvx__` one; without it every libm file fails with `#error Endianess not declared!!`.

The exact-version autotools built for this (autoconf 2.69 already on the system as `autoconf2.69`/etc.; automake 1.15.1 built from the upstream tarball) live at `/tmp/autotools269bin` and `/tmp/autotools-local/bin` in past sessions — outside the repo, so a fresh session will need to rebuild automake 1.15.1 from source again (`https://ftp.gnu.org/gnu/automake/automake-1.15.1.tar.gz`, plain `./configure --prefix=... && make && make install`, quick since it's Perl/shell) before regenerating `configure`/`Makefile.in` again.

`libgloss/configure.ac` — added an `lvx-*-mbr*` case (`AC_CONFIG_FILES([lvx-mbr/Makefile])`, `subdirs="$subdirs lvx-mbr"`) mirroring `kvx-*-mbr*` → `kvx-mbr/Makefile`. `libgloss/configure` regenerated with the same autoconf 2.69 (no automake involved here — `libgloss` has no `Makefile.am`, just plain `Makefile.in` + `AC_CONFIG_FILES`); diff was minimal (just the new case + its `CONFIG_FILES` entry). Note: configuring `--target=lvx-mbr` at the `libgloss/configure` level directly (rather than the top-level combined-tree `configure`) picks the *host* `gcc`, not `lvx-mbr-gcc` — always drive it from `/home/bd3/lvx-csw/lvx-newlib/configure --target=lvx-mbr ... ` (as with newlib) and build via `make all-target-libgloss`, or compile individual files directly with `lvx-mbr-gcc` to sidestep the fact that `all-target-libgloss` depends on `all-target-newlib` succeeding first (which it doesn't fully, since ~10% of newlib still fails on the unrelated pre-existing `lvx-gcc` bugs described at the top of this file).

## Key Porting Gotchas

- **Always test `lvx-gcc` reliability at `-O2`, never `-O0`.** See the callout at the top of this file — five distinct pre-existing `lvx-gcc` bugs were found, but one of them (the branch-analysis ICE) only reproduces at `-O0` and is a complete non-issue at the real build's `-O2`. Testing only at `-O0`/default produces a badly misleading picture (looked like "can't compile any function with a branch," when the real number is ~90% of files compile clean). Of the files ported here: `wf_exp.c` (the only one wired into the real build that uses `__builtin_lvx_*`) hits `lvx_expand_builtin`; `strlen.c`/`strcpy.c`/`strcmp.c` use the same builtins and would too, if they were wired into any build file (they aren't — see the pre-existing `Makefile.am`/`Makefile.inc` gap noted below). `hash_page.c`/`w_pow.c` hit the bad-mnemonic codegen bug instead. Everything else ported so far (`s_fdim.c`, `sf_fdim.c`, `lock.c`, the 9 fenv files, all `.S` files) compiles clean at `-O2`.
- **Do not carry over 32-bit assumptions.** KVX has kv3-1/kv3-2 32-bit-capable variants; LVX is LP64 only, so any `#ifdef`/asm that branches on word size should collapse to the 64-bit path only.
- **KVX-specific compiler builtins** (`__builtin_kvx_*`) rename mechanically to `__builtin_lvx_*` — every one checked so far exists in `lvx-gcc/gcc/config/lvx/lvx-builtins.def` with an identical signature. The blocker is runtime, not naming: see the `lvx_expand_builtin` ICE noted above.
- **`asm.h` and `.S` files use KVX assembly mnemonics/directives** — don't assume syntax compatibility, but in practice every mnemonic checked against `lvx-binutils/opcodes/lvx-opc.c` so far (register loads/stores of all widths, `loopdo`, conditional move/store, etc.) exists unchanged; the one confirmed KVX-only casualty is `dzerol` (kv3-1's cache-zeroing instruction). Always assemble-test with `lvx-mbr-gcc -c foo.S` (not bare `lvx-mbr-as`, which skips the required cpp preprocessing pass over `#include`/`#define`/`#if`) rather than assuming syntax compatibility.
- **Register/ABI details**: the LVX ABI is declared identical to KVX's kv4-v1 ABI (see `/home/bd3/lvx-csw/CLAUDE.md`), so calling conventions in `crt0.c`, `setjmp.S`, and syscall stubs port with minimal change — confirmed for `setjmp.S`/`longjmp.S`, which ported byte-for-byte apart from a comment.
- Every KVX/Kalray source file carries a Kalray copyright/license header; preserve or adapt these headers per your organization's policy when creating `lvx` counterparts rather than silently dropping them.
- **A "generic, shared" sys dir isn't necessarily machine-agnostic in content** — `libc/sys/mbr` is wired generically at the build-system level but its `lock.h` hardcodes KVX atomic/cache headers. Don't assume a shared dir needs zero changes; check its actual `#include`s.
- **`libc/machine/*` and `libm/machine/*` are wired into the build differently** even though they look parallel: `libc/machine/<name>/` is its own autoconf/automake subproject (`Makefile.am` + `configure.ac` + generated `configure`/`aclocal.m4`), while `libm/machine/<name>/` is just a `Makefile.inc` pulled in by `libm/Makefile.inc`'s own `if HAVE_LIBM_MACHINE_*` guard — don't copy the libc pattern (Makefile.am/configure.ac) into a new libm machine dir, it doesn't need one.

## Building and Testing

`--target=lvx-mbr` is wired up and configures successfully (confirmed working end-to-end with the toolchain installed at `/home/bd3/lvx-csw/lvx-toolchain/bin/lvx-mbr-*`):

```bash
export PATH=/home/bd3/lvx-csw/lvx-toolchain/bin:$PATH   # lvx-mbr-gcc/-as/-ar/-ranlib etc.
mkdir build-newlib && cd build-newlib
/home/bd3/lvx-csw/lvx-newlib/configure --target=lvx-mbr --prefix=<toolchain-prefix>
make -k -j$(nproc)   # -k (keep-going) matters: ~10% of files still fail on pre-existing
                     # lvx-gcc bugs unrelated to this repo (see the callout at the top
                     # of this file) - independent targets still build regardless
make install
```

For a trustworthy read on *which* files fail and why, prefer a non-parallel `make -k all-target-newlib > log 2>&1` over `-j$(nproc)`: parallel output interleaves multiple compilers' error messages, making it easy to misattribute one file's failure to another (this bit us once already — see the toolchain-reliability callout at the top of this file for the correction).

`.kalray/build-newlib.sh` is Kalray's original KVX build wrapper (defaults to `TRIPLET_DEFAULT=kvx-mbr`); it's a useful reference for staged configure/build/install invocation but targets the old `k1-elf`/`kvx-mbr` layout, not `lvx-mbr` — do not run it unmodified.

To build `libgloss` for `lvx-mbr`, configure the same top-level tree the same way and target `all-target-libgloss` instead of (or in addition to) `all-target-newlib` — note `all-target-libgloss` depends on `all-target-newlib` completing first, and since ~10% of newlib still fails on the unrelated `lvx-gcc` bugs above, a strict `all-target-libgloss` run won't fully succeed either. To validate just the ported `libgloss/lvx-mbr` files without waiting on that, compile them directly: `lvx-mbr-gcc -march=lvx-1 -O2 -I <path-to>/newlib/libc/include -c <file>.c`.

There is no LVX-specific test suite in this repo yet; testing a ported routine means building it into a static lib against the `lvx-mbr` newlib, linking a test program with the `lvx-gcc`/`lvx-binutils` toolchain, and running it under the LVX ISS (see `/home/bd3/lvx-csw/CLAUDE.md` — the ISS is a planned sibling component, not yet present).
