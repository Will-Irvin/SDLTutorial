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
bool loadMedia(LTexture* texture, SDL_Renderer* renderer) {
	*texture = LTexture();

	if (!texture->loadFromFile("images/arrow.png", renderer)) {
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

	LTexture texture;

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(&texture, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	// Variables to keep track of what the user wants to do to image
	double degrees = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_a:
						degrees -= 60;
						break;

					case SDLK_d:
						degrees += 60;
						break;

					case SDLK_q:
						flip = SDL_FLIP_HORIZONTAL;
						break;

					case SDLK_w:
						flip = SDL_FLIP_NONE;
						break;

					case SDLK_e:
						flip = SDL_FLIP_VERTICAL;
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Render Arrow
		texture.render(renderer, (SCREEN_WIDTH - texture.getWidth()) / 2, 
									 (SCREEN_HEIGHT - texture.getHeight()) / 2, NULL, degrees,
									 NULL, flip);

		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &texture, 1);
	return 0;
}

