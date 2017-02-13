CLIENT = 	irc.exec
SERVER = 	server.exec

CFLAGS= 	-std=c++11 -ggdb3 -Wall -Wextra

SRC_CLIENT =	client/main.cpp \
							client/user.cpp

SRC_SERVER = 	server/main.cpp \
							server/server.cpp

SRC_LIB = 		lib/client.cpp

HEADER = 	-I./lib

OBJDIR =	./objdir

all : $(CLIENT) $(SERVER)

$(CLIENT) :
	g++ $(CFLAGS) $(HEADER) -c $(SRC_CLIENT) $(SRC_LIB)
	g++ -o $(CLIENT)  	 main.o user.o client.o
	mv main.o user.o $(OBJDIR)

$(SERVER) :
	g++ $(CFLAGS) $(HEADER) -c $(SRC_SERVER) 
	g++ -o $(SERVER)  client.o server.o main.o
	mv *.o $(OBJDIR)

clean :
	rm -f $(OBJDIR)/*.o 

fclean : clean
	rm -f $(CLIENT) $(SERVER)

re : fclean all
