#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <iostream>

#include "LTexture.hh"
#include "Dot.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#define TICKS_PER_FRAME (1000 / 60)
#define COLLISION

bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int);
bool checkCollision(SDL_Rect, SDL_Rect);

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

bool loadMedia(LTexture* texture, SDL_Renderer* renderer) {
	*texture = LTexture();
	if (!texture->loadFromFile("images/dot.bmp", renderer)) {
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

// Checks that both axises of the two rectangles do not collide with each
// other. Returns true when they collide
bool checkCollision(SDL_Rect a, SDL_Rect b) {
	int rightA = a.x + a.w;
	int rightB = b.x + b.w;
	int bottomA = a.y + a.h;
	int bottomB = b.y + b.h;

	// Check if any side is outside of the collision
	if (bottomA <= b.y) { // A is over B
		return false;
	}

	if (a.y >= bottomB) { // B is over A
		return false;
	}

	if (rightA <= b.x) { // A is left of B
		return false;
	}

	if (rightB <= a.x) { // B is left of A
		return false;
	}

	return true; // All sides intersect
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
	Dot dot;

	SDL_Rect wall = {300, 40, 40, 400}; // Outline of collision wall

	while (!quit) {
		int startTime = SDL_GetTicks(); // Simple way to cap frame rate
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			dot.handleEvent(e);
		}
		dot.move(wall, checkCollision);

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
		SDL_RenderDrawRect(renderer, &wall); // Draw wall
	
		dot.render(renderer, &texture); // Draw dot

		SDL_RenderPresent(renderer);

		// Simple frame cap
		int time = SDL_GetTicks() - startTime;
		if (time < 0) continue;

		int sleepTime = TICKS_PER_FRAME - time;
		if (sleepTime > 0) {
			SDL_Delay(sleepTime);
		}
	}

	closeSDL(&window, &renderer, &texture, 1);
	return 0;
}
