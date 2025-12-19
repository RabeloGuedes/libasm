COMP = nasm
COMP_FLAGS = -f elf64
NAME = libasm.a
S_FILES = ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s ft_strdup.s

B_FILES = ft_atoi_base.s ft_list_push_front.s ft_list_size.s

SRCS = $(S_FILES)
ifdef BONUS
SRCS += $(B_FILES)
endif
OBJS = $(SRCS:%.s=%.o)

# Tester configuration (use local tester inside this project)
SELF_DIR := $(abspath .)
TESTER_DIR := $(abspath ./libasm_tester)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $^

%.o: %.s
	$(COMP) $(COMP_FLAGS) $< -o $@

bonus:
	@$(MAKE) BONUS=1 all

# Run local tester, passing this project's absolute path
tester:
	@if [ ! -d "$(TESTER_DIR)" ]; then \
		echo "Tester directory '$(TESTER_DIR)' not found."; \
		echo "Please ensure libasm_tester exists inside the project."; \
		exit 1; \
	fi
	@echo "Running tester in $(TESTER_DIR) for project $(SELF_DIR)"
	@echo -n "$(SELF_DIR)" > "$(TESTER_DIR)/.libasm_path"
	@$(MAKE) -C "$(TESTER_DIR)" test_verbose

clean:
	rm -rf $(S_FILES:%.s=%.o) $(B_FILES:%.s=%.o)

fclean: clean
	rm -f $(NAME)
	make -C $(TESTER_DIR) fclean_all

re: fclean all
