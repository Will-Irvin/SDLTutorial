#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

// Function declarations
bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int);

/* Callback function
	 These functions must always: start with a Uint32 arg, 
	 have the second arg be a void*, return a Uint32
	 Callbacks are also asynchrounous
*/
Uint32 callback(Uint32, void*);

// Initialize SDL, Window, Renderer, and Image
bool init(SDL_Window** window, SDL_Renderer** renderer) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "Init Error: " << SDL_GetError() << '\n';
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled\n";
	}
	*window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
														 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, 
														 SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (*window == NULL) {
		std::cout << "Window creation error: " << SDL_GetError() << '\n';
		return false;
	}
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (*renderer == NULL) {
		std::cout << "Renderer creation error: " << SDL_GetError() << '\n';
		return false;
	}

	SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0xff, 0xff);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "SDL_Image init error: " << IMG_GetError() << '\n';
		return false;
	}
	return true;
}

// Load image into texture object
bool loadMedia(LTexture* texture, SDL_Renderer* renderer) {
	*texture = LTexture();

	if (!texture->loadFromFile("images/splash.png", renderer)) {
		return false;
	}

	return true;
}

// Free texture memory and quit SDL and imgs
void closeSDL(SDL_Window** window, SDL_Renderer** renderer, LTexture* textures, int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
	SDL_DestroyWindow(*window);
	*window = NULL;

	SDL_Quit();
	IMG_Quit();
}

// This callback function will print the string passed into param
Uint32 callback(Uint32 interval, void* param) {
	std::cout << "Callback called with message: " << (char *) param << '\n';
	return 0;
}

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	LTexture texture;
	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(&texture, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	char str[] = "3 seconds waited!";
	// Set callback to run in 3 seconds
	SDL_TimerID timerID = SDL_AddTimer(3 * 1000, callback, (void *) str);

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Modulate texture w/ object and render
		texture.render(renderer, 0, 0);

		SDL_RenderPresent(renderer);
	}

	// Remove timer if callback wasn't called)
	SDL_RemoveTimer(timerID);

	closeSDL(&window, &renderer, &texture, 1);
	return 0;
}

