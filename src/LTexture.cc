#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "LTexture.hh"

#include <string>
#include <cstdio>
#include <iostream>

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = -1;
	mHeight = -1;
}

LTexture::~LTexture() {
	free();
}

/**
 * Load image from specified path into this texture wrapper object
 * Set the appropriate attributes including color keying and height/width of
 * image
 */
bool LTexture::loadFromFile(std::string path, SDL_Renderer* renderer) {
	free(); // Remove previous texture (only one at a time)

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Image load error (Path: " << path << "): " << SDL_GetError() << '\n';
		return false;
	}

	// Color key image
	// SetColorKey(surface, bool enable color key, pixel to color with)
	// Original color_key tutorial maps cyan with max blue and green
	// Adjusted to be white with everything max
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xff, 0xff, 0xff));

	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == NULL) {
		std::cout << "Texture conversion error: " << SDL_GetError() << '\n';
		return false;
	}
	// Get image dimensions from the surface
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);
	mTexture = newTexture;
	return true;
}

/**
 * Create a texture based on TTF using the passed in text, color, and font
 */
bool LTexture::loadFromRenderedText(std::string text, SDL_Color color, 
																		SDL_Renderer* renderer, TTF_Font* font) {
	free();

	// Generate surface for text using TTF method
	// Creates based on given text, font, and color
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == NULL) {
		std::cout << "Text Surface Creation Error: " << TTF_GetError() << '\n';
		return false;
	}

	mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (mTexture == NULL) {
		std::cout << "Text Texture Creation Error: " << SDL_GetError() << '\n';
		return false;
	}

	mWidth = textSurface->w;
	mHeight = textSurface->h;

	SDL_FreeSurface(textSurface);
	return true;
}
// Free any memory associated with the texture if it exists
void LTexture::free() {
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = -1;
		mHeight = -1;
	}
}

/**
 * Multiplies a color value onto the the values that are currently displayed
 * For example, if you give 128 (half of 255) if will half the current r, g,
 * or b value for that color
 */
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture, blending);
}

// Works similar to color but for alpha value
void LTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

/**
 * Set rectangle space and render
 * If clip is specified use those dimensions instead of the default
 * If angle and center are given, rotate texture the specified angle over the
 * given central point.
 * If a renderer flip structure is given, flip the image as specified
 */
void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip,
											double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect rect = {x, y, mWidth, mHeight};
	if (clip != NULL) {
		rect.w = clip->w;
		rect.h = clip->h;
	}
	// Use clip rectangle as source rectangle to get particular portion of the image
	// Use RenderCopyEx to rotate the given angle/center/flip as needed
	SDL_RenderCopyEx(renderer, mTexture, clip, &rect, angle, center, flip);
}

// Dimension getters
int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}
