#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <iostream>

#include "LTexture.hh"
#include "Particle.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#define TICKS_PER_FRAME (1000 / 60)

bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, LTexture*, LTexture*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, LTexture*, LTexture*, int);

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

bool loadMedia(LTexture* textures, LTexture* dotTexturePtr,
							 LTexture* shimmerTexturePtr, SDL_Renderer* renderer) {
	if (!dotTexturePtr->loadFromFile("images/dot.bmp", renderer)) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		textures[i] = LTexture();
	}
	if (!textures[0].loadFromFile("images/red.bmp", renderer)) {
		return false;
	}
	if (!textures[1].loadFromFile("images/blue.bmp", renderer)) {
		return false;
	}
	if (!textures[2].loadFromFile("images/green.bmp", renderer)) {
		return false;
	}
	if (!shimmerTexturePtr->loadFromFile("images/shimmer.bmp", renderer)) {
		return false;
	}

	// Make particles somewhat transparent
	for (int i = 0; i < 3; i++) {
		textures[i].setAlpha(192);
	}
	shimmerTexturePtr->setAlpha(192);

	return true;
}

void closeSDL(SDL_Window** window, SDL_Renderer** renderer,
							LTexture* dotTexturePtr, LTexture* shimmerTexturePtr,
							LTexture* textures, int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}
	dotTexturePtr->free();
	shimmerTexturePtr->free();

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

	LTexture textures[3];
	LTexture dotTexture;
	LTexture shimmerTexture;

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(textures, &dotTexture, &shimmerTexture, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;
	ParticleDot dot = ParticleDot(textures);
	while (!quit) {
		int startTime = SDL_GetTicks(); // Simple way to cap frame rate
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			dot.handleEvent(e);
		}
		dot.move();

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);
	
		dot.render(&dotTexture, textures, &shimmerTexture, renderer);

		SDL_RenderPresent(renderer);

		// Simple frame cap
		int time = SDL_GetTicks() - startTime;
		if (time < 0) continue;

		int sleepTime = TICKS_PER_FRAME - time;
		if (sleepTime > 0) {
			SDL_Delay(sleepTime);
		}
	}

	closeSDL(&window, &renderer, &dotTexture, &shimmerTexture, textures, 3);
	return 0;
}
