CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/include -I.
TARGET = route_runner

# Здесь указываем файлы для Перестановки
SRCS = main.cpp tableCipher.cpp 

LIBS = -lUnitTest++ 

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)