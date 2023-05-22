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
	textures[0] = LTexture();
	textures[1] = LTexture();

	if (!textures[0].loadFromFile("images/fadeout.png", renderer)) {
		return false;
	}

	textures[0].setBlendMode(SDL_BLENDMODE_BLEND);

	if (!textures[1].loadFromFile("images/fadein.png", renderer)) {
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

	LTexture texture[2];
	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(texture, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;
	// Initialize color values to "1"
	Uint8 a = 255;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_w) { // Increase on w
					if (a + 32 > 255) {
						a = 255;
					} else {
						a += 32;
					}
				} else if (e.key.keysym.sym == SDLK_s) { // Decrease on s
					if (a - 32 < 0) {
						a = 0;
					} else {
						a -= 32;
					}
				}
			}
		}

		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);


		// Render background
		texture[1].render(renderer, 0, 0);

		// Modulate alpha of foreground texture
		texture[0].setAlpha(a);
		texture[0].render(renderer, 0, 0);

		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, texture, 2);
	return 0;
}

