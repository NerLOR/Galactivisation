
all:
	mkdir -p "bin/"
	g++ "src/main.cpp" -o "bin/galactivisation" -lSDL2 -lSDL2_image -std=c++20
