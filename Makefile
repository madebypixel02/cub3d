# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/22 16:44:37 by aperez-b          #+#    #+#              #
#    Updated: 2022/02/21 17:22:28 by mbueno-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Color Aliases
DEFAULT = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

SHELL=/bin/bash
UNAME = $(shell uname -s)

# Properties for MacOS
CDEBUG = #-g3 -fsanitize=address
LMLX = -lmlx -framework OpenGL -framework AppKit
OS_X = -D OSX=1
END = end_mac.c
ifeq ($(UNAME), Linux)
	# Properties for Linux
	LEAKS = valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes -s -q 
	LMLX = -lmlx -lXext -lX11 -lm
	OS_X =
	END = end_linux.c
	CDEBUG = #-g3
endif

# Make variables
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
CC = gcc -MD
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
BIN = cub3D
NAME = $(BIN_DIR)/$(BIN)
PRINTF = LC_NUMERIC="en_US.UTF-8" printf
LIBFT = libft/bin/libft.a

# Window Size
WIDTH=1080
HEIGHT=720
WW=WIN_W=$(WIDTH)
WH=WIN_H=$(HEIGHT)
SZ=SIZE=10
WIN_SIZE = -D $(WW) -D $(WH) -D $(SZ) $(OS_X)

SRC_GNL = get_next_line.c get_next_line_utils.c

SRC = main.c map.c error.c color.c	\
	  minimap.c raycast.c render.c	\
	  utils.c game.c my_mlx.c		\
	  parse_map.c textures.c 		\
	  player.c $(END) $(SRC_GNL)


OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Progress vars
SRC_COUNT_TOT := $(shell expr $(shell echo -n $(SRC) | wc -w) - $(shell ls -l $(OBJ_DIR) 2>&1 | grep ".o" | wc -l) + 1)
ifeq ($(shell test $(SRC_COUNT_TOT) -le 0; echo $$?),0)
	SRC_COUNT_TOT := $(shell echo -n $(SRC) | wc -w)
endif
SRC_COUNT := 0
SRC_PCT = $(shell expr 100 \* $(SRC_COUNT) / $(SRC_COUNT_TOT))

all: $(NAME)

$(NAME): create_dirs compile_libft $(OBJ)
	@$(CC) $(CFLAGS) $(CDEBUG) $(OBJ) $(LIBFT) $(LMLX) -o $@
	@$(PRINTF) "\r%100s\r$(GREEN)$(BIN) is up to date!$(DEFAULT)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(eval SRC_COUNT = $(shell expr $(SRC_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)..." "" $(SRC_COUNT) $(SRC_COUNT_TOT) $(SRC_PCT)
	@$(CC) $(CFLAGS) $(CDEBUG) $(WIN_SIZE) -c $< -o $@

bonus: all

compile_libft:
	@if [ ! -d "libft" ]; then \
		git clone https://gitlab.com/madebypixel02/libft.git; \
	fi
	@make all -C libft/

create_dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

test: all
	@$(PRINTF) "$(YELLOW)Performing test with custom map...$(DEFAULT)\n\n"
	@$(PRINTF) "Command: $(GRAY)$(LEAKS)./$(NAME) $(MAP)$(DEFAULT)\n\n"
	@$(LEAKS)./$(NAME) $(MAP)

clean:
	@$(PRINTF) "$(CYAN)Cleaning up object files in $(BIN)...$(DEFAULT)\n"
	@if [ -d "libft" ]; then \
		make clean -C libft; \
	fi
	@$(RM) -r $(OBJ_DIR)
	@$(RM) -r $(OBJ_GNL_DIR)

fclean: clean
	@$(RM) -r $(BIN_DIR)
	@if [ -d "libft" ]; then \
		$(RM) $(LIBFT); \
	fi
	@$(PRINTF) "$(CYAN)Removed $(NAME)$(DEFAULT)\n"
	@if [ -d "libft" ]; then \
		$(PRINTF) "$(CYAN)Removed $(LIBFT)$(DEFAULT)\n"; \
	fi

norminette:
	@if [ -d "libft" ]; then \
		make norminette -C libft/; \
	fi
	@$(PRINTF) "$(CYAN)\nChecking norm for $(BIN)...$(DEFAULT)\n"
	@norminette -R CheckForbiddenSourceHeader $(SRC_DIR) inc/

re: fclean
	@make all

git:
	git add .
	git commit
	git push

-include $(OBJ_DIR)/*.d
-include $(OBJ_GNL_DIR)/*.d

.PHONY: all clean fclean bonus compile_libft norminette test play create_dirs git re
