COMP = nasm
COMP_FLAGS = -f elf64
NAME = libasm.a
S_FILES = ft_strlen.s

O_FILES = $(S_FILES:%.s=%.o)

all: $(NAME)

$(NAME): $(O_FILES)
	ar r $(NAME) $(O_FILES)

$(O_FILES): $(S_FILES)
	$(COMP) $(COMP_FLAGS) $^ 

clean:
	rm -rf $(O_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

