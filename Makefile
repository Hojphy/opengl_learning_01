# Nazwa programu
TARGET = program

# Kompilator
CXX = g++
CC = gcc

# Pliki źródłowe
CPP_SOURCES = $(wildcard *.cpp)
C_SOURCES = glad.c

# Ścieżki
INCLUDE_DIRS = -I Linking/include/
LIB_DIRS = -L Linking/lib/

# Biblioteki
LIBS = -lglfw -lGL -lGLU -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor

# Flagi kompilacji (dodaj -Wall, jeśli chcesz widzieć ostrzeżenia)
CXXFLAGS = $(INCLUDE_DIRS)
CFLAGS = $(INCLUDE_DIRS)
LDFLAGS = $(LIB_DIRS) $(LIBS)

# Domyślny cel
all: $(TARGET)

$(TARGET): $(CPP_SOURCES) $(C_SOURCES)
	$(CXX) $(CPP_SOURCES) $(C_SOURCES) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

# Czyszczenie śmieci
clean:
	rm -f $(TARGET) *.o

# Przebudowa od zera
rebuild: clean all
