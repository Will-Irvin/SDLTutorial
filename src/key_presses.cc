#include <SDL2/SDL.h>
#include <cstdio>
#include <string>
#include <iostream>

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

// Enum for key press surface constants
enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL // Not actually used as a value but for array size
};

bool init(SDL_Window**, SDL_Surface**);
bool loadMedia(SDL_Surface**);
void closeSDL(SDL_Window**, SDL_Surface**);
SDL_Surface* loadSurface(std::string);

int main(int argc, char *argv[]) {
	SDL_Window* window = NULL;
	SDL_Surface* win_surface = NULL;  // Used to store the surface for the window
	// Used to store the current surface being displayed
	// Stores the different images that may be blitted during the program
	SDL_Surface* direction_surfaces[KEY_PRESS_SURFACE_TOTAL];

	if (!init(&window, &win_surface)) {
		return -1;
	}
	if (!loadMedia(direction_surfaces)) {
		return -1;
	}

	SDL_Surface* curr_surface = direction_surfaces[KEY_PRESS_SURFACE_DEFAULT];
	
	// Game Loop
	SDL_Event e;
	bool quit = false;

	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;

			if (e.type == SDL_KEYDOWN) { // Key is pressed
				switch(e.key.keysym.sym) { // Check which key is pressed
					// Assign proper image to the current surface
					case SDLK_UP: 
					curr_surface = direction_surfaces[KEY_PRESS_SURFACE_UP];
					break;

					case SDLK_DOWN:
					curr_surface = direction_surfaces[KEY_PRESS_SURFACE_DOWN];
					break;

					case SDLK_LEFT:
					curr_surface = direction_surfaces[KEY_PRESS_SURFACE_LEFT];
					break;

					case SDLK_RIGHT:
					curr_surface = direction_surfaces[KEY_PRESS_SURFACE_RIGHT];
					break;

					default:
					curr_surface = direction_surfaces[KEY_PRESS_SURFACE_DEFAULT];
					break;
				}
			}
		}

		SDL_BlitSurface(curr_surface, NULL, win_surface, NULL);
		SDL_UpdateWindowSurface(window);
	}

	closeSDL(&window, direction_surfaces);

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
	*surface_ptr = SDL_GetWindowSurface(*window_ptr);
	return true;
}

/**
 * This function loads the desired images into an SDL surface array for later
 * use.
 * Returns true on success and false if an error occurs
 */
bool loadMedia(SDL_Surface** surfaces) {
	surfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("images/press.bmp");
	if (surfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
		return false;
	}

	surfaces[KEY_PRESS_SURFACE_UP] = loadSurface("images/up.bmp");
	if (surfaces[KEY_PRESS_SURFACE_UP] == NULL) {
		return false;
	}
	
	surfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("images/down.bmp");
	if (surfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
		return false;
	}
	
	surfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("images/left.bmp");
	if (surfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
		return false;
	}

	surfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("images/right.bmp");
	if (surfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
		return false;
	}

	return true;
}

/**
 * Free allocated memory for SDL window/surfaces and quit SDL before the
 * program terminates
 */
void closeSDL(SDL_Window** window_ptr, SDL_Surface** surfaces) {
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
		SDL_FreeSurface(surfaces[i]);
		surfaces[i] = NULL;
	}

	SDL_DestroyWindow(*window_ptr);
	*window_ptr = NULL;

	SDL_Quit();
}

/**
 * Load an image into an SDL_Surface ptr and return that pointer
 */
SDL_Surface* loadSurface(std::string path) {
	SDL_Surface* surface = SDL_LoadBMP(path.c_str());
	if (surface == NULL) {
		std::cout << "Load Surface Error (Path: " << path << ": " << SDL_GetError() << '\n';
	}

	return surface;
}
