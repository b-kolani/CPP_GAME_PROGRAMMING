NAME = Timber

CC = c++
# CFLAGS = -Wall -Wextra -Werror 
CFLAGS =
SFML_INC = -I ../SFML/include
# SFML_LIB = -L ../SFML/lib -Wl,-rpath,'$$ORIGIN/SFML/lib' -lsfml-graphics -lsfml-window -lsfml-system

# SFML_LIB = -L ../SFML/lib -Wl,-rpath,'$$ORIGIN/../SFML/lib' -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SFML_LIB = -L ../SFML/lib -Wl,-rpath,'$$ORIGIN/../SFML/lib' -lsfml-graphics -lsfml-window -lsfml-system


# SFML_LIB = -L ../SFML/lib -lsfml-graphics -lsfml-window -lsfml-system

HEADERS = 
SRCS = SourceFolder/Timber.cpp 
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

%.o: %.cpp 
	$(CC) $(CFLAGS) $(SFML_INC) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(SFML_LIB) $^ -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all 

.PHONY: all clean fclean re
