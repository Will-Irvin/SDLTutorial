#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#define BUTTON_WIDTH (300)
#define BUTTON_HEIGHT (200)
#define TOTAL_BUTTONS (4)

enum LButtonSprite {
	MOUSE_OUT,
	MOUSE_OVER_MOTION,
	MOUSE_DOWN,
	MOUSE_UP,
	SPRITE_TOTAL
};

class LButton {
	public:
		LButton();
		void setPosition(int, int);
		void handleEvent(SDL_Event*);
		void render(LTexture*, SDL_Renderer*);
	
	private:
		SDL_Point mPosition; // Top left position
		LButtonSprite mCurrentSprite; // Currently used sprite
		SDL_Rect spriteClips[SPRITE_TOTAL]; // Clips for renderering
};

// Initialize variables and the different spriteCLips
LButton::LButton() {
	mPosition.x = 0;
	mPosition.y = 0;
	mCurrentSprite = MOUSE_OUT;	

	for (int i = 0; i < SPRITE_TOTAL; i++) {
		spriteClips[i] = {0, i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
	}
}

// Set position to given coordinates
void LButton::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

/**
 * NOTE: Mouse wheel events also exist, documentation exists for scrolling
 * using them as the mouse scrolls up/down
 */
void LButton::handleEvent(SDL_Event* e) {
	// Check that event involves the mouse
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ||
			e->type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState(&x, &y); // Get mouse coords into x, y

		bool inside = true;
		// Check if mouse within horizontal axis of button then vertical
		if (x < mPosition.x || x > mPosition.x + BUTTON_WIDTH) {
			inside = false;
		} else if (y < mPosition.y || y > mPosition.y + BUTTON_HEIGHT) {
			inside = false;
		}

		// Set sprite index according to type of event
		if (!inside) {
			mCurrentSprite = MOUSE_OUT;
		} else {
			switch(e->type) {
				case SDL_MOUSEMOTION:
					mCurrentSprite = MOUSE_OVER_MOTION;
					break;
				case SDL_MOUSEBUTTONDOWN:
					mCurrentSprite = MOUSE_DOWN;
					break;
				case SDL_MOUSEBUTTONUP:
					mCurrentSprite = MOUSE_UP;
					break;
			}
		}
	}
}

void LButton::render(LTexture* texture, SDL_Renderer* renderer) {
	// Show current button sprite
	texture->render(renderer, mPosition.x, mPosition.y, spriteClips + mCurrentSprite);
}


	
// Function declarations
bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, LButton*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int);

// Initialize SDL, Window, Renderer, and Image
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

// Load image into texture object, initialize clip values
bool loadMedia(LTexture* texture, LButton* buttons, SDL_Renderer* renderer) {
	*texture = LTexture();

	if (!texture->loadFromFile("images/button.png", renderer)) {
		return false;
	}

	buttons[0].setPosition(0, 0);
	buttons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
	buttons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
	buttons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);

	return true;
}

// Free texture memory and quit SDL and imgs
void closeSDL(SDL_Window** window, SDL_Renderer** renderer, LTexture* textures, int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

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

	LTexture texture;
	LButton buttons[TOTAL_BUTTONS];

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(&texture, buttons, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			// Handle events for each button
			for (int i = 0; i < TOTAL_BUTTONS; i++) {
				buttons[i].handleEvent(&e);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Render each button
		for (int i = 0; i < TOTAL_BUTTONS; i++) {
			buttons[i].render(&texture, renderer);
		}

		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &texture, 1);
	return 0;
}

