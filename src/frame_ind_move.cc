#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <iostream>

#include "LTexture.hh"
#include "Dot.hh"
#include "LTimer.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#define TICKS_PER_FRAME (1000 / 60)

bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int);

// Initialize SDL, Window, Renderer, Image, and TTF
bool init(SDL_Window** window, SDL_Renderer** renderer) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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

	// PRESENT_VSYNC flag has SDL uptate at the same time as the monitor during vertical refresh
	// Default is 60 fps
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

	if (TTF_Init() == -1) {
		std::cout << "TTF Init error: " << TTF_GetError() << '\n';
		return false;
	}
	return true;
}

bool loadMedia(LTexture* texture, SDL_Renderer* renderer) {
	*texture = LTexture();
	if (!texture->loadFromFile("images/dot.bmp", renderer)) {
		return false;
	}

	return true;
}

void closeSDL(SDL_Window** window, SDL_Renderer** renderer,
							LTexture* textures, int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
	SDL_DestroyWindow(*window);
	*window = NULL;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
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
	Dot dot;
	LTimer stepTimer;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			dot.handleEvent(e);
		}
		// Move based on calculated time step
		dot.move(stepTimer.getTicks() / 1000.f);

		// Reset timer
		stepTimer.start();

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);
	
		dot.render(renderer, &texture);

		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &texture, 1);
	return 0;
}
