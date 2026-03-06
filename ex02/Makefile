CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

OBJ_DIR = obj

SRC = ./main.cpp \
	  ./PmergeMe.cpp

OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

NAME = PmergeMe

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all