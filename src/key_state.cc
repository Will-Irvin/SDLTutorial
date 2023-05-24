#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

enum texture_options {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	PRESS,
	TOTAL
};

// Function declarations
bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int);

// Initialize SDL, Window, Renderer, and Image
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
	return true;
}

// Load image into texture object
bool loadMedia(LTexture* textures, SDL_Renderer* renderer) {
	for (int i = 0; i < TOTAL; i++) {
		textures[i] = LTexture();
	}
	if (!textures[UP].loadFromFile("images/up.png", renderer)) {
		return false;
	}
	if (!textures[DOWN].loadFromFile("images/down.png", renderer)) {
		return false;
	}
	if (!textures[LEFT].loadFromFile("images/left.png", renderer)) {
		return false;
	}
	if (!textures[RIGHT].loadFromFile("images/right.png", renderer)) {
		return false;
	}
	if (!textures[PRESS].loadFromFile("images/press.png", renderer)) {
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

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	LTexture textures[TOTAL];

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(textures, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	LTexture* currTexture = NULL; // Keep track of current texture

	while (!quit) {
		// Note: still need an event loop, SDL_PollEvent updates key states
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP]) {
			currTexture = &textures[UP];
		} else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
			currTexture = &textures[DOWN];
		} else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
			currTexture = &textures[LEFT];
		} else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
			currTexture = &textures[RIGHT];
		} else {
			currTexture = &textures[PRESS];
		}
			

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		currTexture->render(renderer, 0, 0);

		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, textures, TOTAL);
	return 0;
}

