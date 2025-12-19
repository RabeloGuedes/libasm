# Memory Leak Detection with Valgrind

This test suite includes comprehensive memory leak detection using valgrind.

## Installation

### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install valgrind
```

### Fedora/RHEL:
```bash
sudo dnf install valgrind
```

### macOS:
```bash
brew install valgrind
```

## Usage

### Run tests with valgrind:
```bash
make test_valgrind
```

This will:
1. Build your libasm with the tester
2. Compile with debug symbols (`-g`)
3. Run all tests through valgrind
4. Report any memory leaks detected
5. Create `valgrind.log` with detailed information

### Reading the output:

**No leaks (success):**
```
╔════════════════════════════════════════════════════════════╗
║                                                            ║
║              VALGRIND MEMORY LEAK DETECTION                ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝

Running tests with memory leak detection...
This may take 30-60 seconds...

▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓

════════════════════════════════════════════════════════════
                     TEST RESULTS
════════════════════════════════════════════════════════════

  Mandatory Tests: 78/78 passed (100.0%)
  Bonus Functions:  2/5 complete (+10%) [40/40 tests passed]
  Final Score: 110.0% / 125%

════════════════════════════════════════════════════════════
                   MEMORY LEAK SUMMARY
════════════════════════════════════════════════════════════

  Heap Usage:
    Allocations:  1,523
    Frees:        1,523

  Memory Leaks:
    ✓ Definitely lost:   0 bytes
    ✓ Indirectly lost:   0 bytes
    ✓ Possibly lost:     0 bytes
    ℹ Still reachable:   456 bytes (not a leak)

  ✓ Error Summary: 0 errors detected

════════════════════════════════════════════════════════════
✓ PASSED: No memory leaks detected!

Valgrind logs saved to:
  • valgrind_raw.log (detailed analysis)
```

**Leaks found (failure):**
```
════════════════════════════════════════════════════════════
                   MEMORY LEAK SUMMARY
════════════════════════════════════════════════════════════

  Heap Usage:
    Allocations:  1,523
    Frees:        1,520

  Memory Leaks:
    ✗ Definitely lost:   48 bytes
    ✓ Indirectly lost:   0 bytes
    ⚠ Possibly lost:     24 bytes
    ℹ Still reachable:   456 bytes (not a leak)

  ✗ Error Summary: 3 errors detected

════════════════════════════════════════════════════════════
                     LEAK DETAILS
════════════════════════════════════════════════════════════

Stack traces for leaked memory:

  at malloc (vg_replace_malloc.c:380)
  by ft_list_push_front (ft_list_push_front.s:14)
  by test_ft_list_push_front (test_ft_list_push_front.c:234)

════════════════════════════════════════════════════════════
✗ FAILED: Memory leaks or errors detected

Next steps:
  1. Review the leak details above
  2. Check valgrind_raw.log for complete stack traces
  3. Fix memory allocations in your assembly code
  4. Ensure malloc/free are properly paired

Valgrind logs saved to:
  • valgrind_raw.log (detailed analysis)
  • valgrind_summary.log (leak summary)
```

## What It Checks

- **Definite leaks**: Memory that was allocated but never freed
- **Possible leaks**: Memory that might be leaked (lost pointers)
- **Invalid reads/writes**: Accessing freed or unallocated memory
- **Use of uninitialized values**: Reading from uninitialized memory

## Functions Tested for Memory Leaks

1. **ft_strdup** - Must properly allocate and allow freeing of duplicated strings
2. **ft_list_push_front** - Must properly allocate list nodes that can be freed

## Tips

- The test includes fork() calls for segfault protection, which may show as "still reachable" - these are safe to ignore
- Check `valgrind.log` for detailed stack traces if leaks are found
- Ensure your assembly functions use the same malloc/free as the C library

## Suppressions

Known system library issues are suppressed via `valgrind.supp`. These include:
- dlopen/dlclose internal allocations
- libc initialization allocations
- fork() internal allocations

## Understanding the Output

### Symbols:
- **✓** Green checkmark: No issues
- **✗** Red cross: Critical error or leak
- **⚠** Yellow warning: Possible issue (not critical)
- **ℹ** Blue info: Informational (not a problem)

### Leak Types:

| Type | Severity | Description |
|------|----------|-------------|
| **Definitely lost** | 🔴 Critical | Memory allocated but pointer lost - must fix |
| **Indirectly lost** | 🔴 Critical | Memory reachable only through definitely lost blocks |
| **Possibly lost** | 🟡 Warning | Pointer may exist but possibly invalid |
| **Still reachable** | 🔵 Info | Memory still accessible at exit - usually OK |

### Common Issues:

**1. Missing free() in ft_strdup:**
```
✗ Definitely lost: 48 bytes
  at malloc
  by ft_strdup
```
→ Your ft_strdup allocates but the test couldn't free it

**2. Missing free() in ft_list_push_front:**
```
✗ Definitely lost: 16 bytes  
  at malloc
  by ft_list_push_front
```
→ List node allocated but not freed

**3. Still reachable (OK):**
```
ℹ Still reachable: 456 bytes
```
→ System allocations from fork(), dlopen(), etc. - not your problem

## Manual Valgrind Testing

You can also run valgrind manually on specific tests:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./libasm_tester /path/to/libasm
```

## Performance Note

Valgrind runs tests **10-30x slower** than normal because it instruments every memory operation. Be patient! The comprehensive leak detection is worth the wait.
