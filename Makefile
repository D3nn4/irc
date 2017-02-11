NAME1 = CLIENT
NAME2 = SERVER

CFLAGS= -std=c++11 -ggdb3 -Wall -Wextra
SRC1 = client/client.cpp \
				client/main.cpp
SRC2 = server/server.cpp

HEADER = -I./client 

OBJDIR =

all : $(NAME1) $(NAME2)

$(NAME1) :
	g++ $(CFLAGS) $(HEADER) -c $(SRC1)
	g++ -o $(NAME1)   client.o main.o



$(NAME2) :
	g++ $(CFLAGS) $(HEADER) -c $(SRC2)
	g++ -o $(NAME2)  client.o server.o
	mv *.o objdir

clean :
	rm -f ./objdir/*.o

fclean : clean
	rm -f $(NAME1) $(NAME2)

re : fclean all
