#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#define ANIMATION_FRAMES (4)

// Function declarations
bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*, SDL_Rect*);
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
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
bool loadMedia(LTexture* texture, SDL_Renderer* renderer, SDL_Rect* spriteClips) {
	*texture = LTexture();

	if (!texture->loadFromFile("images/animation.png", renderer)) {
		return false;
	}

	spriteClips[0] = {0, 0, 64, 205};
	spriteClips[1] = {64, 0, 64, 205};
	spriteClips[2] = {128, 0, 64, 205};
	spriteClips[3] = {192, 0, 64, 205};

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
	SDL_Rect spriteClips[ANIMATION_FRAMES];

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(&texture, renderer, spriteClips)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;
	int frame = 0;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} 
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Render current frame (to make image move a bit slower use increments of 4)
		SDL_Rect* currentClip = &spriteClips[frame / 8];
		texture.render(renderer, (SCREEN_WIDTH - currentClip->w) / 2, 
									 (SCREEN_HEIGHT - currentClip->h), currentClip);
		SDL_RenderPresent(renderer);


		frame++;
		if (frame / 8 >= ANIMATION_FRAMES) {
			frame = 0;
		}
	}

	closeSDL(&window, &renderer, &texture, 1);
	return 0;
}

