all:
	# make && ./game
	# g++ -std=c++11 ./src/*.cpp -o game -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2
	g++ -std=c++11 ./src/*.cpp -o game -I/opt/homebrew/include/SDL2 -I/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2 -I/opt/homebrew/Cellar/sdl2_mixer/2.8.0/include/SDL2 -I/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2 -D_THREAD_SAFE -L/opt/homebrew/lib -L/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib -L/opt/homebrew/Cellar/sdl2_mixer/2.8.0/lib -L/opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf