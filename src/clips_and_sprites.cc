#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

bool init(SDL_Window**, SDL_Renderer **);
bool loadMedia(LTexture*, SDL_Rect*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int);

bool init(SDL_Window** window_ptr, SDL_Renderer** renderer_ptr) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Init Error: " << SDL_GetError() << '\n';
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled\n";
	}

	*window_ptr = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, 
																 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
																 SDL_WINDOW_SHOWN);
	if (*window_ptr == NULL) {
		std::cout << "Window creation error: " << SDL_GetError() << '\n';
		return false;
	}

	*renderer_ptr = SDL_CreateRenderer(*window_ptr, -1, SDL_RENDERER_ACCELERATED);
	if (*renderer_ptr == NULL) {
		std::cout << "Renderer creation error: " << SDL_GetError() << '\n';
	}

	SDL_SetRenderDrawColor(*renderer_ptr, 0xff, 0xff, 0xff, 0xff);
	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "Image init error: " << IMG_GetError() << '\n';
		return false;
	}
	return true;
}

bool loadMedia(LTexture* spriteSheetTexture, SDL_Rect* spriteClips,
							 SDL_Renderer* renderer) {
	if (!spriteSheetTexture->loadFromFile("images/dots.png", renderer)) {
		return false;
	}

	spriteClips[0].x = 0;
	spriteClips[0].y = 0;
	spriteClips[0].w = 100;
	spriteClips[0].h = 100;

	spriteClips[1].x = 100;
	spriteClips[1].y = 0;
	spriteClips[1].w = 100;
	spriteClips[1].h = 100;

	spriteClips[2].x = 0;
	spriteClips[2].y = 100;
	spriteClips[2].w = 100;
	spriteClips[2].h = 100;

	spriteClips[3].x = 100;
	spriteClips[3].y = 100;
	spriteClips[3].w = 100;
	spriteClips[3].h = 100;
	return true;
}

void closeSDL(SDL_Window** window_ptr, SDL_Renderer** renderer, LTexture* textures,
							int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}
	
	SDL_DestroyWindow(*window_ptr);
	*window_ptr = NULL;
	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;

	SDL_Quit();
	IMG_Quit();
}
	
int main(int argc, char** argv) {

	// Array of sprite rectangles used to retrieve different portions of the loaded texture
	SDL_Rect spriteClips[4];
	LTexture spriteSheetTexture = LTexture();

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	if (!init(&window, &renderer)) {
		return -1;
	}

	if (!loadMedia(&spriteSheetTexture, spriteClips, renderer)) {
		return -1;
	}

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Render to each corner of the screen using the appropriate clip rectangle
		spriteSheetTexture.render(renderer, 0, 0, spriteClips);
		spriteSheetTexture.render(renderer, SCREEN_WIDTH - spriteClips[1].w, 0, 
															spriteClips + 1);
		spriteSheetTexture.render(renderer, 0, SCREEN_HEIGHT - spriteClips[2].h, spriteClips + 2);
		spriteSheetTexture.render(renderer, SCREEN_WIDTH - spriteClips[3].w, 
															SCREEN_HEIGHT - spriteClips[3].h, spriteClips + 3);

		SDL_RenderPresent(renderer);
	}
	closeSDL(&window, &renderer, &spriteSheetTexture, 1);
	return 0;
}
