#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
bool init(SDL_Window**, SDL_Surface**);
bool loadMedia(SDL_Surface**);
void closeSDL(SDL_Window**, SDL_Surface**);

int main(int argc, char *argv[]) {
	SDL_Window* window = NULL;
	SDL_Surface* win_surface = NULL;  // Used to store the surface for the window
	SDL_Surface* img_surface = NULL;  // Used to store the surface for the image

	if (!init(&window, &win_surface)) {
		return -1;
	}
	if (!loadMedia(&img_surface)) {
		return -1;
	}

	/*
	 * This segment is covered more extensively in lesson 3. SDL_Event is a 
	 * union containing information regarding some SDL event 
	 * (key press, mouse move, etc.)
	 */
	SDL_Event e;
	bool quit = false;
	/*
	 * This loop is called the main loop/game loop and it continues to run
	 * until the game is finished/quit out of. 
	 * SDL_PollEvent processes the most recent events from the event queue.
	 * Continues to process events until the queue is empty when it returns 0
	 * and the inner loop breaks. However, the main loop won't terminate until
	 * one of those events is a quit out event.
	 */
	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;
		}

		// Apply image onto window
		// SDL_BlitSurface(src surface, relevant later, dest surface, relevant later)
		SDL_BlitSurface(img_surface, NULL, win_surface, NULL);

		SDL_UpdateWindowSurface(window);
	}

	closeSDL(&window, &img_surface);

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
		printf("Init Error: %s\n", SDL_GetError());
		return false;
	}

	*window_ptr = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
														 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, 
														 SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (*window_ptr == NULL) {
		printf("Window Error: %s\n", SDL_GetError());
		return false;
	}
	*surface_ptr = SDL_GetWindowSurface(*window_ptr);
	return true;
}

/**
 * This function loads the desired image into an SDL surface for later use
 * Returns true on success and false if an error occurs
 */
bool loadMedia(SDL_Surface** img_surface_ptr) {
	// Load bmp image from image directory
	*img_surface_ptr = SDL_LoadBMP("images/hello_world.bmp");
	if (img_surface_ptr == NULL) {
		printf("Image Load Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void closeSDL(SDL_Window** window_ptr, SDL_Surface** img_surface_ptr) {
	SDL_FreeSurface(*img_surface_ptr);
	*img_surface_ptr = NULL;

	SDL_DestroyWindow(*window_ptr);
	*window_ptr = NULL;

	SDL_Quit();
}
