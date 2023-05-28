#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <iostream>

#include "LTexture.hh"
#include "LTimer.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#define SCREEN_FPS (60)
// Milliseconds to wait before the next frame
#define SCREEN_TICKS_PER_FRAME (1000 / SCREEN_FPS) 

bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*, TTF_Font**);
void closeSDL(SDL_Window**, SDL_Renderer**, TTF_Font**, LTexture*, int);

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

bool loadMedia(LTexture* texture, SDL_Renderer* renderer, TTF_Font** font_ptr) {
	*font_ptr = TTF_OpenFont("fonts/lazy.ttf", 28);
	if (*font_ptr == NULL) {
		std::cout << "Failed to load lazy font: " << TTF_GetError() << '\n';
		return false;
	}

	*texture = LTexture();
	return true;
}

void closeSDL(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font_ptr,
							LTexture* textures, int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

	TTF_CloseFont(*font_ptr);
	*font_ptr = NULL;

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
	TTF_Font* font = NULL;

	LTexture texture;

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(&texture, renderer, &font)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;
	SDL_Color text_color = {0, 0, 0, 0xff};

	std::stringstream timeText;
	LTimer fpsTimer = LTimer();
	LTimer capTimer = LTimer();

	int countedFrames = 0; // Keep track of number of frames renderered
	fpsTimer.start(); // Keep track of time elapsed

	while (!quit) {
		capTimer.start(); // Count duration of current frame
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} 		
		}

		// Frame rate calculations, number of frames / time passed
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000) {
			avgFPS = 0;
		}

		timeText.str("");
		timeText << "Average FPS: " << avgFPS;

		texture.loadFromRenderedText(timeText.str().c_str(), text_color,
																			 renderer, font);

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Render text centered
		texture.render(renderer, (SCREEN_WIDTH - texture.getWidth()) / 2, 
									 (SCREEN_HEIGHT - texture.getHeight()) / 2);

		SDL_RenderPresent(renderer);
		countedFrames++;

		// Stall as needed
		int elapsedTicks = capTimer.getTicks();
		if (elapsedTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(SCREEN_TICKS_PER_FRAME - elapsedTicks);
		}
	}

	closeSDL(&window, &renderer, &font, &texture, 1);
	return 0;
}
