CC = cc
CFLAGS = -Wall -Wextra -Werror -I.
LDFLAGS =

# Test executable name
NAME = libasm_tester

# Source files
SRCS = main.c \
       test_framework.c \
       test_ft_strlen.c \
       test_ft_strcpy.c \
       test_ft_strcmp.c \
       test_ft_write.c \
       test_ft_read.c \
       test_ft_strdup.c \
       test_ft_atoi_base.c \
       test_ft_list_push_front.c \
       test_ft_list_size.c \
       test_ft_list_sort.c \
       test_ft_list_remove_if.c \
       test_stubs.c \
       libasm_weak.c

OBJS = $(SRCS:.c=.o)

# Path to libasm project (will be prompted or provided)
LIBASM_PATH_FILE = .libasm_path
LIBASM_PATH = $(shell cat $(LIBASM_PATH_FILE) 2>/dev/null | tr -d '\n')
LIBASM_LIB = $(LIBASM_PATH)/libasm.a

.PHONY: all clean fclean re test test_verbose test_valgrind help fclean_all fclean_objs test_with_lib

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "✓ Test suite compiled successfully!"

%.o: %.c libtest.h
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests with progress bar (default)
test:
	@if [ ! -f $(LIBASM_PATH_FILE) ]; then \
		read -p "Enter the absolute path to your libasm project: " path; \
		echo -n "$$path" > $(LIBASM_PATH_FILE); \
	fi
	@LIBASM_PATH=$$(cat $(LIBASM_PATH_FILE) | tr -d '\n'); \
	echo "Using libasm from: $$LIBASM_PATH"; \
	echo "Building libasm..."; \
	$(MAKE) -C "$$LIBASM_PATH" > /dev/null 2>&1; \
	$(MAKE) -C "$$LIBASM_PATH" bonus > /dev/null 2>&1 || true; \
	if [ ! -f "$$LIBASM_PATH/libasm.a" ]; then \
		echo "Error: libasm.a not found at $$LIBASM_PATH/libasm.a"; \
		exit 1; \
	fi; \
	$(MAKE) fclean_objs > /dev/null 2>&1; \
	$(MAKE) CFLAGS="$(CFLAGS) -I$$LIBASM_PATH" LDFLAGS="$$LIBASM_PATH/libasm.a" test_with_lib > /dev/null 2>&1; \
	./$(NAME) "$$LIBASM_PATH"

# Run tests with full verbose output
test_verbose:
	@if [ ! -f $(LIBASM_PATH_FILE) ]; then \
		read -p "Enter the absolute path to your libasm project: " path; \
		echo -n "$$path" > $(LIBASM_PATH_FILE); \
	fi
	@LIBASM_PATH=$$(cat $(LIBASM_PATH_FILE) | tr -d '\n'); \
	echo "Using libasm from: $$LIBASM_PATH"; \
	echo "Building libasm..."; \
	$(MAKE) -C "$$LIBASM_PATH" 2>&1; \
	echo "Building libasm bonus..."; \
	$(MAKE) -C "$$LIBASM_PATH" bonus 2>&1 || true; \
	if [ ! -f "$$LIBASM_PATH/libasm.a" ]; then \
		echo "Error: libasm.a not found at $$LIBASM_PATH/libasm.a"; \
		exit 1; \
	fi; \
	echo "Recompiling test suite with libasm..."; \
	$(MAKE) fclean_objs; \
	$(MAKE) CFLAGS="$(CFLAGS) -I$$LIBASM_PATH" LDFLAGS="$$LIBASM_PATH/libasm.a" test_with_lib; \
	echo ""; \
	./$(NAME) -v "$$LIBASM_PATH"

# Run tests with valgrind memory leak detection
test_valgrind:
	@if [ ! -f $(LIBASM_PATH_FILE) ]; then \
		read -p "Enter the absolute path to your libasm project: " path; \
		echo -n "$$path" > $(LIBASM_PATH_FILE); \
	fi
	@if ! command -v valgrind >/dev/null 2>&1; then \
		echo "\033[1;31mError: valgrind is not installed\033[0m"; \
		echo "Install it with: sudo apt-get install valgrind"; \
		exit 1; \
	fi
	@LIBASM_PATH=$$(cat $(LIBASM_PATH_FILE) | tr -d '\n'); \
	echo "Using libasm from: $$LIBASM_PATH"; \
	echo "Building libasm..."; \
	$(MAKE) -C "$$LIBASM_PATH" > /dev/null 2>&1; \
	$(MAKE) -C "$$LIBASM_PATH" bonus > /dev/null 2>&1 || true; \
	if [ ! -f "$$LIBASM_PATH/libasm.a" ]; then \
		echo "Error: libasm.a not found at $$LIBASM_PATH/libasm.a"; \
		exit 1; \
	fi; \
	echo "Recompiling test suite with debug symbols..."; \
	$(MAKE) fclean_objs > /dev/null 2>&1; \
	$(MAKE) CFLAGS="$(CFLAGS) -g -I$$LIBASM_PATH" LDFLAGS="$$LIBASM_PATH/libasm.a" test_with_lib > /dev/null 2>&1; \
	echo ""; \
	./run_valgrind.sh ./$(NAME) "$$LIBASM_PATH"

test_with_lib: $(OBJS)
	$(CC) $(CFLAGS) -Wl,--whole-archive $(LDFLAGS) -Wl,--no-whole-archive $(OBJS) -o $(NAME)

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJS)

fclean_objs:
	@rm -f $(OBJS)

fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)
	@rm -f valgrind_raw.log valgrind_summary.log valgrind.log

fclean_all: fclean
	@echo "Cleaning libasm path cache and valgrind files..."
	@rm -f $(LIBASM_PATH_FILE) valgrind.supp

re: fclean all

help:
	@echo "LIBASM Tester Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make              - Compile the test suite (without libasm)"
	@echo "  make test         - Build libasm and run all tests"
	@echo "  make test_verbose - Run tests with detailed output"
	@echo "  make test_valgrind- Run tests with valgrind memory leak detection"
	@echo "  make clean        - Remove object files"
	@echo "  make fclean       - Remove object files and executable"
	@echo "  make fclean_all   - Remove everything including saved libasm path"
	@echo "  make re           - Recompile from scratch"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "You can also run the tester directly:"
	@echo "  ./$(NAME) [path_to_libasm]"

