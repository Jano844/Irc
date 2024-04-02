NAME		= server
CXX			= c++
# CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

SRCS		=	main.cpp


OBJS		= $(SRCS:.cpp=.o)


RED			= \033[0;31m
GREEN		= \033[0;32m
CRYAN		= \033[0;36m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)..."
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(CRYAN)"

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning $(NAME) object files..."
	@rm -f $(OBJS)

fclean: clean
	@echo "$(RED)Cleaning $(NAME)..."
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
