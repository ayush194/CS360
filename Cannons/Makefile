SHELL = /bin/bash

TARGET = cannons
OBJS = main.o cannon.o missile.o
CPPFLAGS = -std=c++11 -Wall -g
CC = g++
INCLUDES = gl_includes.h
LIBS = -lm
UNAME := $(shell uname)
FRAMEWORKS = 
ifeq ($(UNAME), Linux)
	FRAMEWORKS += -lGL -lGLU -lglut
endif
ifeq ($(UNAME), Darwin)
	FRAMEWORKS += -framework OpenGL -framework GLUT
endif

${TARGET} : ${OBJS}
	@${CC} ${CPPFLAGS} -o ${TARGET} ${OBJS} ${FRAMEWORKS}

main.o : main.cpp ${INCLUDES}
	@${CC} ${CPPFLAGS} -c main.cpp

cannon.o : cannon.cpp cannon.hpp ${INCLUDES}
	@${CC} ${CPPFLAGS} -c cannon.cpp

missile.o : missile.cpp missile.hpp ${INCLUDES}
	@${CC} ${CPPFLAGS} -c missile.cpp

cannon.hpp missile.hpp : 

${INCLUDES} : 
	
clean : 
	@rm ${OBJS} ${TARGET}
