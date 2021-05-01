# Directorios del proyecto
SRC = src
INC = include
OBJ = obj
BIN = bin

# Opciones de compilación
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g -I./$(INC) -O3

# Archivos del proyecto
SOURCES := random.cpp par.cpp readfiles.cpp greedyCOPKM.cpp busqlocal.cpp genetico.cpp memetico.cpp
INCLUDES := $(addprefix $(INC)/, $(SOURCES:.cpp=.h))
OBJECTS := $(addprefix $(OBJ)/, $(SOURCES:.cpp=.o))
SOURCES := $(addprefix $(SRC)/, $(SOURCES))

.PHONY: clean mrproper

all: main
main: $(BIN)/main

# ************ Generación de ejecutables *************

# -- Práctica 1 --
$(BIN)/main: $(OBJECTS) $(OBJ)/main.o
	$(CXX) -o $@ $^

# ************ Compilación de módulos ************

# -- Prácticas --
$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

# -- Utilidades --
$(OBJ)/%.o: $(SRC)/%.cpp $(INC)/%.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# ************ Limpieza ************
clean :
	-@rm -f $(OBJ)/* $(SRC)/*~ $(INC)/*~ *~
	@echo "Limpiando..."

mrproper : clean
	-@rm -f $(BIN)/*
	@echo "Limpieza completada"

zip :
	zip -r practica.zip .
