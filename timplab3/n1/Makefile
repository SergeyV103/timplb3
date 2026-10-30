CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/include -I.
TARGET = alpha_runner

# Здесь указываем файлы для Гронсфельда
SRCS = main.cpp modAlphaCipher.cpp 

LIBS = -lUnitTest++ 

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)