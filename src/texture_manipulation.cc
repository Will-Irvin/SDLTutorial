#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

bool init(SDL_Window**, SDL_Renderer **);
bool loadMedia(LTexture*, SDL_Window*, SDL_Renderer*);
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

bool loadMedia(LTexture* texture, SDL_Window* window,
							 SDL_Renderer* renderer) {
	if (!texture->loadPixelsFromFile("images/pixels_manip.png", window)) {
		return false;
	}

	// Manipulate the pixels of the texture

	// Pixel data
	Uint32* pixels = texture->getPixels32();
	Uint32 pixelCount = texture->getPitch32() * texture->getHeight();;

	Uint32 colorKey = SDL_MapRGBA(SDL_GetWindowSurface(window)->format,
																0xff, 0, 0xff, 0xff);
	Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(window)->format,
																	 0xff, 0xff, 0xff, 0);

	// Make all background pixels transparent
	for (Uint32 i = 0; i < pixelCount; i++) {
		if (pixels[i] == colorKey) {
			pixels[i] = transparent;
		}
	}

	if (!texture->loadFromPixels(renderer)) {
		return false;
	}
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
	LTexture texture = LTexture();

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	if (!init(&window, &renderer)) {
		return -1;
	}

	if (!loadMedia(&texture, window, renderer)) {
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

		texture.render(renderer, (SCREEN_WIDTH - texture.getWidth()) / 2,
									 (SCREEN_HEIGHT - texture.getHeight()) / 2);

		SDL_RenderPresent(renderer);
	}
	closeSDL(&window, &renderer, &texture, 1);
	return 0;
}
