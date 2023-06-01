#include <SDL2/SDL.h>

#include "LTexture.hh"

enum TileTypes {
	TILE_RED,
	TILE_GREEN,
	TILE_BLUE,
	TILE_CENTER,
	TILE_TOP,
	TILE_TOPRIGHT,
	TILE_RIGHT,
	TILE_BOTTOMRIGHT,
	TILE_BOTTOM,
	TILE_BOTTOMLEFT,
	TILE_LEFT,
	TILE_TOPLEFT,
};

class Tile {
	public:
		Tile(int, int, TileTypes);
		void render(SDL_Renderer*, SDL_Rect&, LTexture*, SDL_Rect*);
		TileTypes getType();
		SDL_Rect getBox();
	
	private:
		SDL_Rect mBox;
		TileTypes mType;
};

class TileDot {
	public:
		TileDot();
		void handleEvent(SDL_Event&);
		void move(Tile *tiles[]);
		void setCamera(SDL_Rect& camera);
		void render(SDL_Renderer*, LTexture*, SDL_Rect& camera);
	
	private:
		SDL_Rect mBox; // Collision box
		int mVelX, mVelY;
};

bool checkCollision(SDL_Rect, SDL_Rect);
bool touchesWall(SDL_Rect box, Tile* tiles[]);
bool setTiles(Tile *tiles[]);
