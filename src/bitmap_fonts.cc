#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

class LBitmapFont {
	public:
		LBitmapFont();
		bool buildFont(std::string, SDL_Window*, SDL_Renderer*);
		void free();
		void renderText(SDL_Renderer*, int, int, std::string);
	
	private:
		LTexture mFontTexture;

		// Characters in surface
		SDL_Rect mChars[256];

		// Spacing
		int mNewLine;
		int mSpace;
};

LBitmapFont::LBitmapFont() {
	mNewLine = 0;
	mSpace = 0;
}

void LBitmapFont::free() {
	mFontTexture.free();
	mNewLine = 0;
	mSpace = 0;
}

bool LBitmapFont::buildFont(std::string path, SDL_Window* window, SDL_Renderer* renderer) {
	free();

	if (!mFontTexture.loadPixelsFromFile(path, window)) {
		return false;
	}
	Uint32 bgColor = mFontTexture.getPixel32(0, 0);

	// Set dimentions
	int cellW = mFontTexture.getWidth() / 16;
	int cellH = mFontTexture.getHeight() / 16;

	// New line vars
	int top = cellH;
	int baseA = cellH;

	int currChar = 0;

	// Characters will be arranged in evenly divided cells each with an ASCII character
	// We will iterate over the cells and set the clips for each cell
	for (int rows = 0; rows < 16; rows++) {
		for (int cols = 0; cols < 16; cols++) {
			mChars[currChar].x = cellW * cols;
			mChars[currChar].y = cellH * rows;
			mChars[currChar].w = cellW;
			mChars[currChar].h = cellH;
			// Find left side
			for (int pCol = 0; pCol < cellW; pCol++) {
				for (int pRow = 0; pRow < cellH; pRow++) {
					// Go through each pixel
					int pX = (cellW * cols) + pCol;
					int pY = (cellH * rows) + pRow;

					// Non coler key pixel
					if (mFontTexture.getPixel32(pX, pY) != bgColor) {
						// Set x based on location of that pixel
						mChars[currChar].x = pX;
						// Escape loops
						pCol = cellW;
						pRow = cellH;
					}
				}
			}
			// Find right side
			for (int pColW = cellW - 1; pColW >= 0; pColW--) {
				for (int pRow = 0; pRow < cellH; pRow++) {
					int pX = (cellW * cols) + pColW;
					int pY = (cellH * rows) + pRow;
					// Set width based on location of right pixel
					if (mFontTexture.getPixel32(pX, pY) != bgColor) {
						mChars[currChar].w = (pX - mChars[currChar].x) + 1;
						pColW = -1;
						pRow = cellH;
					}
				}
			}

			// Find top
			for (int pRow = 0; pRow < cellH; pRow++) {
				for (int pCol = 0; pCol < cellW; pCol++) {
					int pX = cellW * cols + pCol;
					int pY = cellH * rows + pRow;

					if (mFontTexture.getPixel32(pX, pY) != bgColor) {
						// Set top to lower value when a lower top is found
						if (pRow < top) {
							top = pRow;
						}
						pCol = cellW;
						pRow = cellH;
					}
				}
			}

			// Find bottom of A
			if (currChar == 'A') {
				for (int pRow = cellH - 1; pRow >= 0; pRow--) {
					for (int pCol = 0; pCol < cellW; pCol++) {
						int pX = cellW * cols + pCol;
						int pY = cellH * rows + pRow;
						
						if (mFontTexture.getPixel32(pX, pY) != bgColor) {
							baseA = pRow;
							pCol = cellW;
							pRow = -1;
						}
					}
				}
			}
			currChar++;
		}
	}

	mSpace = cellW / 2;
	mNewLine = baseA - top;

	// Get rid of extra top pixels
	for (int i = 0; i < 256; i++) {
		mChars[i].y += top;
		mChars[i].h -= top;
	}

	if (!mFontTexture.loadFromPixels(renderer)) {
		return false;
	}
	return true;
}

void LBitmapFont::renderText(SDL_Renderer* renderer, int x, int y, std::string text) {
	if (mFontTexture.getWidth() > 0) {
		int curX = x;
		int curY = y;
		for (size_t i = 0; i < text.length(); i++) {
			if (text[i] == ' ') { // Shift over horizontally
				curX += mSpace;
			} else if (text[i] == '\n') { // Reset on new line
				curX = x;
				curY += mNewLine;
			} else { // Render the character
				int ascii = (unsigned char) text[i];

				mFontTexture.render(renderer, curX, curY, &mChars[ascii]);
				curX += mChars[ascii].w + 1;
			}
		}
	}
}

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
	return true;
}

bool loadMedia(LBitmapFont* bitmap, SDL_Window* window, SDL_Renderer* renderer) {
	if (!bitmap->buildFont("fonts/lazyfont.png", window, renderer)) {
		return false;
	}
	return true;
}

void closeSDL(SDL_Window** window, SDL_Renderer** renderer, LBitmapFont* bitmap) {
	SDL_DestroyWindow(*window);
	*window = NULL;
	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;

	bitmap->free();

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char** argv) {
	LBitmapFont bitmap;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(&bitmap, window, renderer)) {
		return -1;
	}

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		bitmap.renderText(renderer, 0, 0,
											"Bitmap Font:\nABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789");
		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &bitmap);
	return 0;
}
