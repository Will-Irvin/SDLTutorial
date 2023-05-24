#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

/*
	Joystick input is read as position values from -32768 to 32767
	A dead zone is used to make sure light taps don't register when they
	should not.
*/
#define JOYSTICK_DEAD_ZONE (8000)

// Function declarations
bool init(SDL_Window**, SDL_Renderer**, SDL_Joystick** controller);
bool loadMedia(LTexture*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int, SDL_Joystick**);

// Initialize SDL, Window, Renderer, and Image
bool init(SDL_Window** window, SDL_Renderer** renderer, SDL_Joystick** controller) {
	// Add additional flag to support joystick input
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		std::cout << "Init Error: " << SDL_GetError() << '\n';
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled\n";
	}

	// Load joysticks if they exist
	if (SDL_NumJoysticks() < 1) {
		std::cout << "Warning: No joystick connected\n";
	} else {
		*controller = SDL_JoystickOpen(0);
		if (*controller == NULL) {
			std::cout << "Controller open error: " << SDL_GetError() << '\n';
			return false;
		}
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

// Load image into texture object
bool loadMedia(LTexture* texture, SDL_Renderer* renderer) {
	*texture = LTexture();

	if (!texture->loadFromFile("images/arrow.png", renderer)) {
		return false;
	}

	return true;
}

// Free texture memory and quit SDL and imgs
void closeSDL(SDL_Window** window, SDL_Renderer** renderer, LTexture* textures,
							int numTextures, SDL_Joystick** controller) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

	SDL_JoystickClose(*controller);
	*controller = NULL;
	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
	SDL_DestroyWindow(*window);
	*window = NULL;

	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Joystick* controller = NULL; // Data structure for joystick

	LTexture texture;

	if (!init(&window, &renderer, &controller)) {
		return -1;
	}
	if (!loadMedia(&texture, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	// Keep track of direction that joystick is pointing
	int xDir = 0; // 1 for right, -1 for left, 0 in dead zone
	int yDir = 0; // -1 for down, 1 for up, 0 in dead zone

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_JOYAXISMOTION) {
				// Make sure motion is on correct controller
				if (e.jaxis.which == 0) {
					// X axis
					if (e.jaxis.axis == 0) {
						if (e.jaxis.value < -JOYSTICK_DEAD_ZONE) { // Left
							xDir = -1;
						} else if (e.jaxis.value > JOYSTICK_DEAD_ZONE) { // Right
							xDir = 1;
						} else { // Not out of dead zone
							xDir = 0;
						}
					} else if (e.jaxis.axis == 1) { // Y axis
						if (e.jaxis.value < -JOYSTICK_DEAD_ZONE) { // Down
							yDir = -1;
						} else if (e.jaxis.value > JOYSTICK_DEAD_ZONE) { // Up
							yDir = 1;
						} else {
							yDir = 0;
						}
					}
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Calculate joystickAngle
		double angle = atan2( (double) yDir, (double) xDir) * (180.0 / M_PI);

		if (xDir == 0 && yDir == 0) {
			angle = 0;
		}

		// Render joystick based on the given angle
		texture.render(renderer, (SCREEN_WIDTH - texture.getWidth()) / 2,
									 (SCREEN_HEIGHT - texture.getHeight()) / 2, NULL, angle);

		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &texture, 1, &controller);
	return 0;
}

