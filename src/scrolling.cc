#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdio>
#include <cmath>
#include <iostream>

#include "LTexture.hh"
#include "Dot.hh"

#define LEVEL_WIDTH (1280)
#define LEVEL_HEIGHT (960)
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

bool loadMedia(LTexture* textures, SDL_Renderer* renderer) {
	textures[0] = LTexture();
	if (!textures[0].loadFromFile("images/dot.bmp", renderer)) {
		return false;
	}

	textures[1] = LTexture();
	if (!textures[1].loadFromFile("images/bg.png", renderer)) {
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

	LTexture textures[2];

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(textures, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;
	Dot dot;

	// When scrolling have a large image to represent the level
	// Render only a portion of that image on the screen
	// Adjust the segment being shown as needed

	// Camera area
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	while (!quit) {
		int startTime = SDL_GetTicks();
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			dot.handleEvent(e);
		}
		dot.move(true); // Move Dot

		// Adjust camera to be centered around dot
		camera.x = (dot.getPosX() + Dot::DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
		camera.y = (dot.getPosY() + Dot::DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

		// Adjust camera bounds if needed
		if (camera.x < 0) {
			camera.x = 0;
		} else if (camera.x > LEVEL_WIDTH - camera.w) {
			camera.x = LEVEL_WIDTH - camera.w;
		}
		if (camera.y < 0) {
			camera.y = 0;
		} else if (camera.y > LEVEL_HEIGHT - camera.h) {
			camera.y = LEVEL_HEIGHT - camera.h;
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);
	
		textures[1].render(renderer, 0, 0, &camera); // Render level image
		dot.render(renderer, textures, camera.x, camera.y);

		SDL_RenderPresent(renderer);

		// Simple frame cap
		int time = SDL_GetTicks() - startTime;
		if (time < 0) continue;

		int sleepTime = TICKS_PER_FRAME - time;
		if (sleepTime > 0) {
			SDL_Delay(sleepTime);
		}
	}

	closeSDL(&window, &renderer, textures, 2);
	return 0;
}
