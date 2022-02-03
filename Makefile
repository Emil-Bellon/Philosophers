
# NAME=philo

# SRC_DIR=src
# SRCS=	main.c \
# 		utils.c \
# 		philo_thread.c \
# 		actions.c

# OBJDIR=bin
# OBJS=$(SRCS:%.c=$(OBJDIR)/%.o)

# CLANG=clang
# CFLAGS=-Wextra -Wall -Werror -pthread
# IFLAGS=-Iinc

# all: $(NAME)
# 	@echo "Done"

# $(NAME) : $(OBJDIR) $(OBJS)
# 	@$(CLANG) $(CFLAGS) $(IFLAGS) $(OBJS) -o $(NAME)

# $(OBJDIR) :
# 	@echo "No ./$(OBJDIR)/ : Creating it."
# 	@mkdir -p $(OBJDIR)

# $(OBJDIR)/%.o : $(SRC_DIR)/%.c
# 	@echo "Compiling :" $<
# 	@$(CLANG) -c $(CFLAGS) $(IFLAGS) $< -o $@

# clean:
# 	@rm -rf $(NAME)
# 	@echo "Removed executable : $(NAME)"

# fclean: clean
# 	@rm -rf $(OBJDIR)
# 	@echo "Removed $(OBJDIR) objects directory."

# re : fclean all


#~~~~ Output ~~~~#

NAME		=	philo

#~~~~ Paths ~~~~#

VPATH		=	src/
PATH_OBJ	=	objs/
PATH_INC	=	inc/

#~~~~ Sources ~~~~#
 
SRCS		=	main.c \
				utils.c \
				philo_thread.c \
				actions.c

#~~~~ Objects ~~~~#

OBJS		=	$(addprefix $(PATH_OBJ), $(SRCS:.c=.o))

#~~~~ Includes ~~~~#

INCS		=	$(addprefix $(PATH_INC), philo.h)

#~~~~ Macros ~~~~#

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror
RM			=	rm -rf

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			$(NAME)

$(NAME) :		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -I $(PATH_INC)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJ)%.o :	%.c $(INCS)
					@mkdir -p $(dir $@);
					$(CC) $(CFLAGS) -c $< -o $@ -I $(PATH_INC)

#~~~~ Cleaning Rules ~~~~#

clean :
				$(RM) $(PATH_OBJ)

fclean :		
				$(RM) $(PATH_OBJ) $(NAME)

.PHONY:		all clean fclean re