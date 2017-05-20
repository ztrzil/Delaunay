#Makefile to create the shared library:

CXX = g++
INCLUDES = -I /Users/zac/work/Stephenson/testDelaunay/Cside/include
CFLAGS = $(INCLUDES)
LIB_DIR = /Users/zac/work/Stephenson/testDelaunay/Cside/lib
SRC_DIR = /Users/zac/work/Stephenson/testDelaunay/Cside/src

MV ?= mv

all: libDel.so

libDel.so: DelaunayBuilder.o triangle.o
	$(CXX) -shared -o libDel.so DelaunayBuilder.o triangle.o

triangle.o:
	$(CXX) $(INCLUDES) -c $(SRC_DIR)/triangle.cpp
#	@$(MV) $(SRC_DIR)/triangle.o $(LIB_DIR)/triangle.o

DelaunayBuilder.o:
	$(CXX) $(INCLUDES) -c $(SRC_DIR)/DelaunayBuilder.cpp 
#	@$(MV) $(SRC_DIR)/DelaunayBuilder.o $(LIB_DIR)/DelaunayBuilder.o

clean: 
	rm -f *.o libDel.so

