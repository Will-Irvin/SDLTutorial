#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

int main(int argc, char* argv[]) {
  SDL_Window* window = NULL;

	// Surfaces are 2D images, something downloaded from a file or in a window
	SDL_Surface* screenSurface = NULL;

	// Only flag needed is using the video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { // True if error
		printf("Init Error: %s\n", SDL_GetError());
		return -1;
	}

	/*
		SDL_CreateWindow(char* caption, pos, pos, width, height, window flags)
		Using undefined means we don't care where the window appears
	*/
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, 
														SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, 
														SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window error: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	// Set SDL_Surface to window surface
	screenSurface = SDL_GetWindowSurface(window);

	// Color surface red
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0, 0));

	// Update surface after changes
	SDL_UpdateWindowSurface(window);

	// Segment used to keep window open until user closes it
	SDL_Event e; 
	bool quit = false; 
	while(quit == false){ 
		while(SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT) quit = true; 
		}
	}

	// Clear memory and quit SDL
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
