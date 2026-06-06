CC        = cc
CFLAGS    = -Wall -Werror -Wextra -g3

SRC_DIR   = src
INC_DIR   = include
OBJ_DIR   = .obj

LIBFT_DIR = libft
LIBFT     = $(LIBFT_DIR)/libft.a

vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR) libft/

SRC_FILES  = malloc.c

OBJS      := $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

HEADERS = $(LIBFT_DIR)/include/libft.h Makefile
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR)/include/

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBFT) -shared -o $(NAME)
	rm -f libft_malloc.so
	ln -s $(NAME) libft_malloc.so

$(OBJ_DIR)/%.o: %.c $(HEADERS) | $(OBJ_DIR)
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

test: $(NAME)
	$(CC) $(CFLAGS) $(INCLUDES) main.c -L. -lft_malloc -o test

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)
	rm -f libft_malloc.so
	rm -f test

re: fclean all

.PHONY: all clean fclean re test