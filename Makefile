
NAME=philo

SRC_DIR=src
SRCS=	main.c \
		utils.c \
		philo_thread.c \
		actions.c

OBJDIR=bin
OBJS=$(SRCS:%.c=$(OBJDIR)/%.o)

CLANG=clang
CFLAGS=-Wextra -Wall -Werror -pthread
IFLAGS=-Iinc

all: $(NAME)
	@echo "Done"

$(NAME) : $(OBJDIR) $(OBJS)
	@$(CLANG) $(CFLAGS) $(IFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR) :
	@echo "No ./$(OBJDIR)/ : Creating it."
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o : $(SRC_DIR)/%.c
	@echo "Compiling :" $<
	@$(CLANG) -c $(CFLAGS) $(IFLAGS) $< -o $@

clean:
	@rm -rf $(NAME)
	@echo "Removed executable : $(NAME)"

fclean: clean
	@rm -rf $(OBJDIR)
	@echo "Removed $(OBJDIR) objects directory."

re : fclean all
