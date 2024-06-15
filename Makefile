NAME	=	ft_shmup

# a variable SRCS contain any files in srcs with *.cpp directory
SRCS	=	$(wildcard srcs/*.cpp)
INCLUDE	=	$(wildcard includes/*.hpp)
SRCDIR	=	srcs
OBJDIR	=	objs
LIBS	=	-lncurses

# a variable OBJS contain any files in srcs with *.o directory which is created in objs directory
# srcs/main.cpp -> objs/main.o
OBJS	=	$(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

# a variable DEPS contain any files in srcs with *.d directory
DEPS	=	$(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.d)))

CXX		=	c++
CXXFLAGS	= 	-std=c++11 -Wall -Wextra -Werror -g -MMD -MP

RM		=	rm -rf

all:		$(NAME)

objs/%.o:	srcs/%.cpp
			@mkdir -p objs
			$(CXX) $(CXXFLAGS) -c $< -o $@ -Iincludes

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LIBS)

test:		$(NAME)
			./$(NAME)

debug:          CXXFLAGS += -DDEBUG
debug:          re

clean:
			$(RM) $(OBJS) $(DEPS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re


-include	$(DEPS)
