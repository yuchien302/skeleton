CC = gcc
CXXFLAGS	= -O2 -g -fmessage-length=0
SOURCES	:= $(shell find src -name '*.cpp') $(shell find src -name '*.c')
OBJECTS	:= $(subst .c,.o,$(subst .cpp,.o,$(subst src/,build/,$(SOURCES))))
DIRECTORIES := $(sort $(dir $(OBJECTS)))
SEARCH_PATHS = 
LDFLAGS	= 
TARGET	= assignment

ifeq ($(OS),Windows_NT)
    CXXFLAGS += -D WIN32
    LDFLAGS += -lfreeglut -lglu32 -lglew32 -lopengl32 -lcomdlg32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CXXFLAGS += -D LINUX
        LDFLAGS += -lglut -lGL -lGLU -lGLEW
    endif
    ifeq ($(UNAME_S),Darwin)
        CXXFLAGS += -D OSX
        LDFLAGS += -framework GLUT -framework OpenGL
    endif
endif

all: build $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(SEARCH_PATHS) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

build/%.o: src/%.cpp
	$(CXX) $(SEARCH_PATHS) $(CXXFLAGS) -c -o $@ $<

build/%.o: src/%.c
	$(CC) $(SEARCH_PATHS) $(CXXFLAGS) -c -o $@ $<

build:
	mkdir $(DIRECTORIES)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe