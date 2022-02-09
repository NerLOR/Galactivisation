
all:
	mkdir -p "bin/"
	g++ src/*.cpp src/utils/voronoi.cpp src/game/*.cpp -o "bin/galactivisation" -lSDL2 -lSDL2_image -std=c++20 -O3
	@echo "Finished compiling"
