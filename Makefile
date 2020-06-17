NAME = webserv

OBJS = main.o\
	MyServer.o\
	Reader.o\
	Connection.o\
	Request.o\
	Response.o\
	Writer.o\
	Processor.o\
	Config.o\
	Utils.o\
	RequestURI.o\
	MimeTypes.o\
	Logger.o\
	Location.o\
	CgiProcess.o\
	StringBuffer.o\
	StringReader.o\


CXX = clang++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++11 
#-fsanitize=address 

all: $(NAME)

ft_lib:
	cd libft && $(MAKE) $(MAKEFLAGS)

$(NAME): $(OBJS) ft_lib
	$(CXX) -o $(NAME) $(CXXFLAGS) $(OBJS) -L libft -lft

clean:
	@rm -f $(OBJS)
	@cd libft && $(MAKE) clean

fclean: clean
	@rm -f $(NAME)
	@cd libft && $(MAKE) fclean

re:     fclean all
