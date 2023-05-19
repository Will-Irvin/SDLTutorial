#include <SDL2/SDL.h>
#include <cstdio>
#include <string>
#include <iostream>

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

bool init(SDL_Window**, SDL_Renderer**);
void closeSDL(SDL_Window**, SDL_Renderer**);

int main(int argc, char *argv[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (!init(&window, &renderer)) {
		return -1;
	}

	// Game Loop
	SDL_Event e;
	bool quit = false;

	// Define Rectangle to fill the screen
	// Note: coordinates work backwards for the y-axis, as y increases, the point moves down
	SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
	SDL_Rect borderRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;
		}

		// Clear screen (set to color of SetRenderDrawColor to black)
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		
		// Draw red square in center
		SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
		SDL_RenderFillRect(renderer, &fillRect);

		// Draw green outline
		SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0xff);
		// DrawRect draws the outline of the rectangle as opposed to filling it
		SDL_RenderDrawRect(renderer, &borderRect);

		// Draw Blue line
		SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0xff);
		// Draw line by giving init coordinates then ending coordinates
		SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
		
		// Draw dotted yellow line (by drawing multiple points
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0xff);
		for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
			SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i);
		}

		// Update screen
		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer);

	return 0;
}

/**
 * This function handles the initialization of the SDL window
 * The passed in window pointer is updated to point to the address of the newly
 * created window, and the renderer is initialized to map to the new window.
 * Returns true on success and false if an error occurs
 */
bool init(SDL_Window** window_ptr, SDL_Renderer** renderer_ptr) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Init Error:" << SDL_GetError() << '\n';
		return false;
	}

	*window_ptr = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
														 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, 
														 SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (*window_ptr == NULL) {
		std::cout << "Window Error:" << SDL_GetError() << '\n';
		return false;
	}

	// Initialize Renderer for the window (window, index (-1 for first one), flags)
	*renderer_ptr = SDL_CreateRenderer(*window_ptr, -1, SDL_RENDERER_ACCELERATED);
	if (*renderer_ptr == NULL) {
		std::cout << "Renderer Creation Error: " << SDL_GetError() << '\n';
		return false;
	}

	// Set color for drawing operations
	// First 3 values are rgb, last arg is a or alpha: how opaque/transparent drawing is
	SDL_SetRenderDrawColor(*renderer_ptr, 0xFF, 0xFF, 0xFF, 0xFF);

	return true;
}

/**
 * Free allocated memory for SDL window/renderer and quit SDL before the
 * program terminates
 */
void closeSDL(SDL_Window** window_ptr, SDL_Renderer** renderer_ptr) {
	SDL_DestroyRenderer(*renderer_ptr);
	*renderer_ptr = NULL;
	SDL_DestroyWindow(*window_ptr);
	*window_ptr = NULL;

	SDL_Quit();
}
