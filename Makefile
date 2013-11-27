EXEC= main
SOURCES= main.cpp model_tableau.cpp
CFLAGS= -Wall -Wextra
LIB= -lm
OBJETS= $(SOURCES:.cpp=.o)

$(EXEC):$(OBJETS)
	g++ -o $(EXEC) $(OBJETS) $(CFLAGS) $(LIB)

clean :
	rm -f *.o $(EXEC)
