ALL_DIR_SRC 	= ./ 
INCLUDES 	= -Imysql-connector
CPPFLAGS	= -pthread -gdwarf-2 -lmysqlcppconn -L ./

all : project

include Makefile.inc

project : $(ALL_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
