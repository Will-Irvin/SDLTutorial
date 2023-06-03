#ifndef LTEXTURE

#define LTEXTURE

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// Texture Wrapper Class
class LTexture {
	public:
		LTexture(); // Constructor

		~LTexture(); // Destructor

		bool loadFromFile(std::string, SDL_Renderer*); // Load image into texture

		bool loadPixelsFromFile(std::string, SDL_Renderer*); // Load image into pixel buffer

		bool loadFromPixels(); //Create image from loaded pixels

		bool loadFromRenderedText(std::string, SDL_Color, SDL_Renderer*, TTF_Font*); // Image from font

		void free(); // Deallocate texture

		void setColor(Uint8, Uint8, Uint8); // Set color modulation

		void setBlendMode(SDL_BlendMode); // Set blending
		
		void setAlpha(Uint8 alpha); // Set alpha modulation

		// Render at given point
		void render(SDL_Renderer*, int, int, SDL_Rect* = NULL, double = 0,
								SDL_Point* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);

		// Dimension getters
		int getWidth();
		int getHeight();

		// Pixel getters
		Uint32* getPixels32();
		Uint32* getPitch32();

	private:
		SDL_Texture* mTexture;

		SDL_Surface* mSurfacePixels;
		int mWidth;
		int mHeight;
};

#endif
