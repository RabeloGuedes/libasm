# LIBASM Comprehensive Test Suite

A professional testing framework for your libasm assembly library project with colorful output and detailed test results.

## Features

✨ **Comprehensive Testing**
- Tests all mandatory functions (ft_strlen, ft_strcpy, ft_strcmp, ft_write, ft_read, ft_strdup)
- Tests all bonus functions (ft_atoi_base, ft_list_push_front, ft_list_size, ft_list_sort, ft_list_remove_if)

🎨 **User-Friendly Interface**
- Colorful terminal output (green for pass, red for fail)
- Clear test descriptions and results
- Visual check marks (✓) and cross marks (✗)
- Detailed failure information

🛡️ **Robust Design**
- Failed tests don't stop execution
- Missing functions are detected and skipped gracefully
- Each function is tested independently
- Comprehensive summary at the end

## Project Structure

```
libasm_tester/
├── Makefile              # Build system
├── libtest.h             # Header file with function prototypes
├── main.c                # Main entry point
├── test_framework.c      # Testing framework utilities
├── test_ft_strlen.c      # ft_strlen tests (implemented)
├── test_stubs.c          # Stub functions for other tests
└── README.md             # This file
```

## Installation & Usage

### Quick Start

1. **Clone or download this tester**
   ```bash
   cd /path/to/libasm_tester
   ```

2. **Run the test suite**
   ```bash
   make test
   ```
   
   You'll be prompted for the path to your libasm project. The tester will:
   - Build your libasm project
   - Compile the test suite
   - Run all tests automatically

3. **Alternative: Run with path argument**
   ```bash
   make fclean_objs
   echo "/path/to/your/libasm" > .libasm_path
   make test
   ```
   
   Or run the executable directly:
   ```bash
   ./libasm_tester /path/to/your/libasm
   ```

### Makefile Commands

- `make` - Compile the test suite (standalone, without libasm)
- `make test` - Build libasm and run all tests
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make fclean_all` - Remove everything including saved libasm path
- `make re` - Recompile from scratch
- `make help` - Display help message

## Current Test Coverage

### ✅ Implemented: ft_strlen

The `ft_strlen` test suite includes 10 comprehensive tests:

1. **Empty string** - Tests with ""
2. **Simple string** - Tests with "Hello"
3. **String with spaces** - Tests with "Hello World"
4. **Long string** - Tests with 59-character string
5. **Special characters** - Tests with tabs, newlines, and symbols
6. **Numeric string** - Tests with "1234567890"
7. **Single character** - Tests with "A"
8. **Very long string** - Stress test with 1000 characters
9. **UTF-8 characters** - Tests with "Hello 世界"
10. **Only spaces** - Tests with "     "

Each test compares your `ft_strlen` output against the standard library `strlen` function.

### 🚧 To Be Implemented

- ft_strcpy
- ft_strcmp
- ft_write
- ft_read
- ft_strdup
- ft_atoi_base (bonus)
- ft_list_push_front (bonus)
- ft_list_size (bonus)
- ft_list_sort (bonus)
- ft_list_remove_if (bonus)

## Sample Output

```
╔════════════════════════════════════════════════════════════╗
║                                                            ║
║              LIBASM COMPREHENSIVE TEST SUITE               ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝

Using libasm path from argument: /path/to/libasm

Building libasm...
Libasm built successfully!
Found libasm.a at /path/to/libasm/libasm.a

════════════════════════════════════════════════════════════
                    MANDATORY FUNCTIONS                     
════════════════════════════════════════════════════════════


============================================================
                     Testing ft_strlen
============================================================

  ✓ Empty string                                       [PASS]
  ✓ Simple string                                      [PASS]
  ✓ String with spaces                                 [PASS]
  ✓ Long string                                        [PASS]
  ✓ String with special characters                     [PASS]
  ✓ String with numbers                                [PASS]
  ✓ Single character                                   [PASS]
  ✓ Very long string (1000 chars)                      [PASS]
  ✓ String with UTF-8 characters                       [PASS]
  ✓ String with only spaces                            [PASS]

------------------------------------------------------------
Summary:
  Total tests: 10
  Passed:      10
  Failed:      0

  All tests passed! ✓
------------------------------------------------------------
```

## Requirements

- **Compiler**: GCC or Clang with support for `-Wall -Wextra -Werror`
- **Assembler**: NASM (for building your libasm project)
- **Make**: GNU Make or compatible
- **Terminal**: Terminal with ANSI color support (most modern terminals)

## Your libasm Project Structure

Your libasm project should have:
- A `Makefile` with an `all` target
- Assembly source files (`.s` or `.asm`)
- Output: `libasm.a` static library

Example libasm Makefile:
```makefile
NAME = libasm.a
SRCS = ft_strlen.s ft_strcpy.s ...
OBJS = $(SRCS:.s=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.s
	nasm -f elf64 $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
```

## How It Works

1. **Prompt for path** - The tester asks for your libasm project path (or uses cached path)
2. **Build libasm** - Runs `make` in your libasm directory
3. **Validate** - Checks that `libasm.a` was created successfully
4. **Compile tests** - Links test files with your `libasm.a`
5. **Run tests** - Executes all test functions
6. **Report results** - Shows colored output with pass/fail status

## Troubleshooting

### "libasm.a not found"
- Make sure your libasm Makefile creates `libasm.a` in the project root
- Check that `make` runs successfully in your libasm directory

### "Function not found"
- The tester will skip functions that don't exist (this is expected behavior)
- Implement the function in your libasm project and rebuild

### Compilation errors
- Ensure your function signatures match the expected prototypes
- Check that your assembly follows the System V AMD64 calling convention

## Contributing

To add tests for other functions:
1. Create a new file `test_ft_<function>.c`
2. Implement comprehensive tests following the pattern in `test_ft_strlen.c`
3. Update the Makefile's `SRCS` variable
4. Remove the stub from `test_stubs.c`

## License

This test suite is free to use for educational purposes.

## Author

Created as a comprehensive testing tool for the 42 School libasm project.
