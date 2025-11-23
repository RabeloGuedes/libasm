COMP = nasm
COMP_FLAGS = -f elf64
NAME = libasm.a
S_FILES = ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s

O_FILES = $(S_FILES:%.s=%.o)

all: $(NAME)

$(NAME): $(O_FILES)
	ar rcs $(NAME) $(O_FILES)

%.o: %.s
	$(COMP) $(COMP_FLAGS) $< -o $@

clean:
	rm -rf $(O_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

