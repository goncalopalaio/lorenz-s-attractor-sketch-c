FLAGS  =  -g -ggdb -pg -lm -framework GLUT -framework OpenGL -framework Cocoa
CC     = g++ 
CXXFLAGS = -Wno-deprecated-declarations
PROG   = lorenz
OBJS   = main.o

all:	${PROG}

clean:
	rm ${OBJS} ${PROG}
  
${PROG}:	${OBJS}
	${CC} ${OBJS} -o $@ ${FLAGS} 

.c.o:
	${CC} ${FLAGS} $< -c

##########################
main.o: main.cpp
lorenz: main.o
