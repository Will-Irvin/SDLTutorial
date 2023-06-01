#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>

#include "LTexture.hh"
#include "LWindow.hh"

bool init(LWindow*, int*, SDL_Rect*);
void closeSDL(LWindow*, SDL_Rect*);

bool init(LWindow* window_ptr, int* displays_ptr, SDL_Rect** displayBounds) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Init Error: " << SDL_GetError() << '\n';
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled\n";
	}

	// Get display information
	*displays_ptr = SDL_GetNumVideoDisplays(); // Get amount of displays
	if (*displays_ptr < 2) {
		std::cout << "Note: One one display connected\n";
	}

	*displayBounds = new SDL_Rect[*displays_ptr];
	for (int i = 0; i < *displays_ptr; i++) {
		// Set each element of the array to the rectangle corresponding to that display
		SDL_GetDisplayBounds(i, (*displayBounds) + i);
	}
	if (!window_ptr->init()) {
		std::cout << "Window creation error: " << SDL_GetError() << '\n';
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "Image init error: " << IMG_GetError() << '\n';
		return false;
	}
	return true;
}

void closeSDL(LWindow* window_ptr, SDL_Rect* displayBounds) {
	window_ptr->free();
	delete displayBounds;

	SDL_Quit();
	IMG_Quit();
}
	
int main(int argc, char** argv) {
  LWindow window;
	int totalDisplays = 0;
	SDL_Rect* displayBounds = NULL;
	if (!init(&window, &totalDisplays, &displayBounds)) {
		return -1;
	}

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			window.handleEvent(e, NULL, displayBounds, totalDisplays);
		}
		window.render();
	}

	closeSDL(&window, displayBounds);
	return 0;
}
