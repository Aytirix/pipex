NAME = pipex
OBJDIR = objets
OBJ = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
LIBFT = Libft
LIBFT_A = $(LIBFT)/libft.a
FLAGS = -fsanitize=address -fdiagnostics-color=always -g3 -Wall -Wextra -g3 #-Werror
LIBS = -L$(LIBFT) -lft

SRC = pipex.c \
      tools.c

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m

BONUS = 1

all: $(LIBFT_A) $(NAME)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT) --no-print-directory

$(NAME): $(OBJ)
	@echo "$(GREEN)Building $(NAME) with BONUS=$(BONUS)...$(RESET)"
	@$(CC) $(FLAGS) -DBONUS=$(BONUS) -I$(LIBFT) $(OBJ) -o $@ $(LIBS)
	@echo "$(BLUE)$(NAME) built successfully!$(RESET)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling $< with BONUS=$(BONUS)...$(RESET)"
	@$(CC) $(FLAGS) -DBONUS=$(BONUS) -I$(LIBFT) -c $< -o $@

bonus: BONUS=1
bonus: all

clean:
	@echo "$(MAGENTA)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)
	@$(MAKE) -C $(LIBFT) clean --no-print-directory

fclean: clean
	@echo "$(RED)Cleaning all files...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT) fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re bonus
