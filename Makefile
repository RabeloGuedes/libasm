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

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $^

%.o: %.s
	$(COMP) $(COMP_FLAGS) $< -o $@

bonus:
	@$(MAKE) BONUS=1 all

clean:
	rm -rf $(S_FILES:%.s=%.o) $(B_FILES:%.s=%.o)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus

