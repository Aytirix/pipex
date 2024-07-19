NAME = pipex
OBJDIR = objets
SRC = pipex.c \
      tools.c

OBJ = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
LIBFT = Libft
LIBFT_A = $(LIBFT)/libft.a
FLAGS = -Wall -Wextra -Werror
LIBS = -L$(LIBFT) -lft

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m

BONUS = 0
BONUS_FLAG_FILE = $(OBJDIR)/.bonus_flag

all: $(LIBFT_A) $(NAME)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT) --no-print-directory

$(NAME): check_bonus_flag $(OBJ)
	@echo "$(GREEN)Linking $(NAME) with BONUS=$(BONUS)...$(RESET)"
	@$(CC) $(FLAGS) -DBONUS=$(BONUS) -I$(LIBFT) $(OBJ) -o $@ $(LIBS)
	@echo "$(BLUE)$(NAME) linked successfully!$(RESET)"

check_bonus_flag:
	@mkdir -p $(OBJDIR)
	@if [ ! -f $(BONUS_FLAG_FILE) ] || [ "$$(cat $(BONUS_FLAG_FILE))" != "$(BONUS)" ]; then \
		echo "$(BONUS)" > $(BONUS_FLAG_FILE); \
		$(MAKE) --no-print-directory recompile_objs; \
	fi

recompile_objs: clean_objs $(OBJ)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling $< with BONUS=$(BONUS)...$(RESET)"
	@$(CC) $(FLAGS) -DBONUS=$(BONUS) -I$(LIBFT) -c $< -o $@

bonus:
	@$(MAKE) -s BONUS=1 all

clean_objs:
	@echo "$(MAGENTA)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)/*.o

clean: clean_objs
	@$(MAKE) -C $(LIBFT) clean --no-print-directory
	@rm -f $(BONUS_FLAG_FILE)

fclean: clean
	@echo "$(RED)Cleaning all files...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT) fclean --no-print-directory

re: fclean all

.PHONY: all clean clean_objs fclean re bonus check_bonus_flag recompile_objs
