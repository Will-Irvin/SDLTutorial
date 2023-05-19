#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

bool init(SDL_Window**, SDL_Surface**);
bool loadMedia(SDL_Surface**, SDL_Surface*);
void closeSDL(SDL_Window**, SDL_Surface**, int);
SDL_Surface* loadSurface(std::string, SDL_Surface*);

int main(int argc, char *argv[]) {
	SDL_Window* window = NULL;
	SDL_Surface* win_surface = NULL;  // Used to store the surface for the window
	// Used to store the current surface being displayed
	// Stores the different images that may be blitted during the program
	SDL_Surface* stretched_surface;

	if (!init(&window, &win_surface)) {
		return -1;
	}
	if (!loadMedia(&stretched_surface, win_surface)) {
		return -1;
	}

	// Rectangle used to stretch the image over the entire window
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;

	// Game Loop
	SDL_Event e;
	bool quit = false;

	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;
		}

		// Blits image stretched out to the specified rectangle (defined above)
		SDL_BlitScaled(stretched_surface, NULL, win_surface, &rect);
		SDL_UpdateWindowSurface(window);
	}

	closeSDL(&window, &stretched_surface, 1);

	return 0;
}

/**
 * This function handles the initialization of the SDL window
 * The passed in window pointer is updated to point to the address of the newly
 * created window, and the surface pointer will point to the address of the
 * surface of that window
 * Returns true on success and false if an error occurs
 */
bool init(SDL_Window** window_ptr, SDL_Surface** surface_ptr) {
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

	// Initialize Image Loading (PNG specifically)
	int imgFlags = IMG_INIT_PNG;

	/*
	 * IMG_Init returns the flags that were loaded successfully
	 * Since we only care if PNG is enabled or not, we use binary & to check
	 * for that flag only. != would also work, but it would be more strict
	 */
	if ( !(IMG_Init(imgFlags) & imgFlags) ) {
		std::cout << "Image Init Error: " << IMG_GetError() << '\n';
		return false;
	}
	*surface_ptr = SDL_GetWindowSurface(*window_ptr);
	return true;
}

/**
 * This function loads the desired images into an SDL surface array for later
 * use.
 * Returns true on success and false if an error occurs
 */
bool loadMedia(SDL_Surface** surface, SDL_Surface* surface_ptr) {
	*surface = loadSurface("images/hockey.png", surface_ptr);
	return *surface != NULL;
}

/**
 * Free allocated memory for SDL window/surfaces and quit SDL before the
 * program terminates
 */
void closeSDL(SDL_Window** window_ptr, SDL_Surface** surfaces, int num_surfaces) {
	for (int i = 0; i < num_surfaces; i++) {
		SDL_FreeSurface(surfaces[i]);
	}

	SDL_DestroyWindow(*window_ptr);
	*window_ptr = NULL;

	SDL_Quit();
	IMG_Quit();
}

/**
 * Load an image into an SDL_Surface ptr and return that pointer
 * Optimize that image by formatting it to the given window surface
 */
SDL_Surface* loadSurface(std::string path, SDL_Surface* windowSurface) {
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Load Surface Error (Path: " << path << "): " << SDL_GetError() << '\n';
		return NULL;
	}
	// Optimization function, returns a newly optimized copy without getting rid of the old one
	SDL_Surface* optimizedSurface = SDL_ConvertSurface(loadedSurface, windowSurface->format, 0);
	if (optimizedSurface == NULL) {
		std::cout << "Optimization error (Path: " << path << "): " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(loadedSurface); // Free old surface

	return optimizedSurface;
}
