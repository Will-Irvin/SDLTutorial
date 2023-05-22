#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(SDL_Texture**, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, SDL_Texture**, int);
SDL_Texture* loadTexture(std::string, SDL_Renderer*);

int main(int argc, char *argv[]) {
	SDL_Window* window = NULL;

	// Data structures used to manage textures
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(&texture, renderer)) {
		return -1;
	}

	// Viewport rectangles, used to blit to portion of the screen
	SDL_Rect topLeft = {0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
	SDL_Rect topRight = {SCREEN_WIDTH / 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
	SDL_Rect bottom = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};

	// Game Loop
	SDL_Event e;
	bool quit = false;

	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;
		}

		// Clear screen (set to color of SetRenderDrawColor)
		SDL_RenderClear(renderer);

		// Set viewport of renderer (top left)
		SDL_RenderSetViewport(renderer, &topLeft);
		// Render Texture
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		// Repeat for right
		SDL_RenderSetViewport(renderer, &topRight);
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		// Repeat for bottom
		SDL_RenderSetViewport(renderer, &bottom);
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		// Update screen
		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &texture, 1);

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

	return true;
}

/**
 * This function loads the desired image into an SDL_Texture for later use
 * Returns true on success and false if an error occurs
 */
bool loadMedia(SDL_Texture** texture, SDL_Renderer* renderer) {
	*texture = loadTexture("images/hockey.png", renderer);
	return *texture != NULL;
}

/**
 * Free allocated memory for SDL window/surfaces and quit SDL before the
 * program terminates
 */
void closeSDL(SDL_Window** window_ptr, SDL_Renderer** renderer_ptr, SDL_Texture** textures, int num_textures) {
	for (int i = 0; i < num_textures; i++) {
		SDL_DestroyTexture(textures[i]);
	}

	SDL_DestroyRenderer(*renderer_ptr);
	*renderer_ptr = NULL;
	SDL_DestroyWindow(*window_ptr);
	*window_ptr = NULL;

	SDL_Quit();
	IMG_Quit();
}

/**
 * Load an image into an SDL_Surface ptr and convert it to a texture
 * Return the generated texture
 */
SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer) {
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Load Surface Error (Path: " << path << "): " << SDL_GetError() << '\n';
		return NULL;
	}
	// Create texture from the loaded surface
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);;
	if (newTexture == NULL) {
		std::cout << "Texture error (Path: " << path << "): " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(loadedSurface); // Free old surface

	return newTexture;
}
