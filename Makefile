# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/22 16:44:37 by aperez-b          #+#    #+#              #
#    Updated: 2023/04/08 14:58:51 by aperez-b         ###   ########.fr        #
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
PRINTF = printf
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
CC = gcc -MD
AR = ar rcs
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
OBJ_GNL_DIR = obj_gnl
BIN = cub3D
NAME = $(BIN_DIR)/$(BIN)
LIBFT = libft/bin/libft.a
LIBFT_DIR = libft
LIBFT_SRC = $(shell [ -d libft ] && ls libft/src*/*.c)
GNL_DIR = get_next_line

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
	  player.c $(END)


OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

OBJ_GNL = $(addprefix $(OBJ_GNL_DIR)/, $(SRC_GNL:.c=.o))

# Progress vars
SRC_COUNT_TOT := $(shell expr $(shell echo -n $(SRC) | wc -w) - $(shell ls -l $(OBJ_DIR) 2>&1 | grep ".o" | wc -l) + 1)
ifeq ($(shell test $(SRC_COUNT_TOT) -le 0; echo $$?),0)
	SRC_COUNT_TOT := $(shell echo -n $(SRC) | wc -w)
endif
SRC_COUNT := 0
SRC_PCT = $(shell expr 100 \* $(SRC_COUNT) / $(SRC_COUNT_TOT))
SRC_GNL_COUNT_TOT := $(shell expr $(shell echo -n $(SRC_GNL) | wc -w) - $(shell ls -l $(OBJ_GNL_DIR) 2>&1 | grep ".o" | wc -l) + 1)
ifeq ($(shell test $(SRC_GNL_COUNT_TOT) -le 0; echo $$?),0)
	SRC_GNL_COUNT_TOT := $(shell echo -n $(SRC_GNL) | wc -w)
endif
SRC_GNL_COUNT := 0
SRC_GNL_PCT = $(shell expr 100 \* $(SRC_GNL_COUNT) / $(SRC_GNL_COUNT_TOT))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ) $(OBJ_GNL) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $(CDEBUG) $(OBJ) $(OBJ_GNL) $(LIBFT) $(LMLX) -o $@
	@$(PRINTF) "\r%100s\r$(GREEN)$(BIN) is up to date!$(DEFAULT)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(GNL_DIR)
	@$(eval SRC_COUNT = $(shell expr $(SRC_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)..." "" $(SRC_COUNT) $(SRC_COUNT_TOT) $(SRC_PCT)
	@$(CC) $(CFLAGS) $(CDEBUG) $(WIN_SIZE) -c $< -o $@

$(OBJ_GNL_DIR)/%.o: $(GNL_DIR)/%.c | $(OBJ_GNL_DIR)
	@$(eval SRC_GNL_COUNT = $(shell expr $(SRC_GNL_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(MAGENTA)$<$(DEFAULT)..." "" $(SRC_GNL_COUNT) $(SRC_GNL_COUNT_TOT) $(SRC_GNL_PCT)
	@$(CC) $(CFLAGS) $(CDEBUG) $(WIN_SIZE) -c $< -o $@

bonus: all

$(LIBFT): $(LIBFT_SRC) | $(LIBFT_DIR) $(BIN_DIR)
	@make all -C libft
	@$(AR) $(NAME) $@

test: all
	@$(PRINTF) "$(YELLOW)Performing test with custom map...$(DEFAULT)\n\n"
	@$(PRINTF) "Command: $(GRAY)$(LEAKS)./$(NAME) $(MAP)$(DEFAULT)\n\n"
	@$(LEAKS)./$(NAME) $(MAP)

clean: | $(LIBFT_DIR)
	@$(PRINTF) "$(CYAN)Cleaning up object files in $(BIN)...$(DEFAULT)\n"
	@make clean -C libft
	@$(RM) -r $(OBJ_DIR)
	@$(RM) -r $(OBJ_GNL_DIR)

fclean: clean | $(LIBFT_DIR)
	@$(RM) $(LIBFT)
	@$(PRINTF) "$(CYAN)Removed $(LIBFT)$(DEFAULT)\n"
	@$(RM) -r $(BIN_DIR)
	@$(PRINTF) "$(CYAN)Removed $(NAME)$(DEFAULT)\n"

norminette: | $(LIBFT_DIR) $(GNL_DIR)
	@make norminette -C libft
	@$(PRINTF) "$(CYAN)\nChecking norm for get_next_line...$(DEFAULT)\n"
	@norminette -R CheckForbiddenSourceHeader $(GNL_DIR)
	@$(PRINTF) "$(CYAN)\nChecking norm for $(BIN)...$(DEFAULT)\n"
	@norminette -R CheckForbiddenSourceHeader $(SRC_DIR) inc/

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_GNL_DIR):
	@mkdir -p $(OBJ_GNL_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(LIBFT_DIR):
	@git clone https://gitlab.com/madebypixel02/libft.git

$(GNL_DIR):
	@git clone https://gitlab.com/madebypixel02/get_next_line.git

re: fclean
	@make all

git:
	git add .
	git commit
	git push

-include $(OBJ_DIR)/*.d
-include $(OBJ_GNL_DIR)/*.d

.PHONY: all clean fclean bonus norminette test play git re
