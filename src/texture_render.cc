#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

#define TICKS_PER_FRAME (1000 / 60)

bool init(SDL_Window** window, SDL_Renderer** renderer) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Could not initialize SDL " << SDL_GetError() << '\n';
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

bool loadMedia(LTexture* texture, SDL_Renderer* renderer) {
	if (!texture->createBlank(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_TEXTUREACCESS_TARGET, renderer)) {
		return false;
	}
	return true;
}

void closeSDL(SDL_Window** window_ptr, SDL_Renderer** renderer, LTexture* texture) {
	texture->free();
	SDL_DestroyWindow(*window_ptr);
	*window_ptr = NULL;
	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;

	SDL_Quit();
	IMG_Quit();
}
	
int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	LTexture texture;

	if (!init(&window, &renderer)) {
		return false;
	}

	if (!loadMedia(&texture, renderer)) {
		return false;
	}

	bool quit = false;
	SDL_Event e;
	// Rotation variables
	double angle = 0;
	SDL_Point center = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

	while (!quit) {
		int startTicks = SDL_GetTicks();
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		// Rotate
		angle += 2;
		if (angle > 360) {
			angle -= 360;
		}

		texture.setAsRenderTarget(renderer);

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Red center
		SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
		SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
		SDL_RenderFillRect(renderer, &fillRect);

		// Green outline
		SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
		SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0xff);
		SDL_RenderFillRect(renderer, &outlineRect);

		// Blue horizontal line
		SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0xff);
		SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

		// Vertical line of yellow dots
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0xff);
		for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
			SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i);
		}

		SDL_SetRenderTarget(renderer, NULL); // Reset render target

		texture.render(renderer, 0, 0, NULL, angle, &center); // Show rendered texture
		
		SDL_RenderPresent(renderer);

		int finish = SDL_GetTicks() - startTicks;
		if (finish < TICKS_PER_FRAME) {
			SDL_Delay(finish);
		}
	}

	closeSDL(&window, &renderer, &texture);
	return 0;
}
