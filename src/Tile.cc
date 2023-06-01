#include <SDL2/SDL.h>

#include "Tile.hh"
#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

#define TILE_WIDTH (80)
#define TILE_HEIGHT (80)
#define TOTAL_TILES (192)

#define DOT_WIDTH (20)
#define DOT_HEIGHT (20)
#define DOT_VEL (10)

#define LEVEL_WIDTH (1280)
#define LEVEL_HEIGHT (960)

Tile::Tile(int x, int y, TileTypes tileType) {
	mBox.x = x;
	mBox.y = y;

	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	mType = tileType;
}

void Tile::render(SDL_Renderer* renderer, SDL_Rect& camera,
									LTexture* tileTexture, SDL_Rect* tileClips) {
	// Check that tile is within the camera range before actually renderering
	if (checkCollision(camera, mBox)) {
		tileTexture->render(renderer, mBox.x - camera.x, mBox.y - camera.y,
												&tileClips[mType]);
	}
}

// Getters
TileTypes Tile::getType() {
	return mType;
}

SDL_Rect Tile::getBox() {
	return mBox;
}

TileDot::TileDot() {
	mBox = {0, 0, DOT_WIDTH, DOT_HEIGHT};
	mVelX = 0;
	mVelY = 0;
}

// Handle direction/speed of dot based on key inputs
void TileDot::handleEvent(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP: 
				mVelY -= DOT_VEL;
				break;
			case SDLK_DOWN: 
				mVelY += DOT_VEL;
				break;
			case SDLK_LEFT:
				mVelX -= DOT_VEL;
				break;
			case SDLK_RIGHT:
				mVelX += DOT_VEL;
				break;
		}
	} 
	// Stop moving dot after key is released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP:
				mVelY += DOT_VEL;
				break;
			case SDLK_DOWN:
				mVelY -= DOT_VEL;
				break;
			case SDLK_LEFT:
				mVelX += DOT_VEL;
				break;
			case SDLK_RIGHT:
				mVelX -= DOT_VEL;
				break;
		}
	}
}

// Check that dot is within the bouds of the screen and not colliding with a wall
// before moving
void TileDot::move(Tile *tiles[]) {
	mBox.x += mVelX;

	if (mBox.x < 0 || mBox.x + DOT_WIDTH > LEVEL_WIDTH || touchesWall(mBox, tiles)) {
		mBox.x -= mVelX;
	}

	mBox.y += mVelY;
	if (mBox.y < 0 || mBox.y + DOT_HEIGHT > LEVEL_HEIGHT || touchesWall(mBox, tiles)) {
		mBox.y -= mVelY;
	}
}

// Move camera so dot is centered
void TileDot::setCamera(SDL_Rect& camera) {
	// Center camera over dot
	camera.x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	if (camera.x < 0) {
		camera.x = 0;
	} else if (camera.x > LEVEL_WIDTH - camera.w) {
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y < 0) {
		camera.y = 0;
	} else if (camera.y > LEVEL_HEIGHT- camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void TileDot::render(SDL_Renderer* renderer, LTexture* dotTexture,
										 SDL_Rect& camera) {
	dotTexture->render(renderer, mBox.x - camera.x, mBox.y - camera.y);
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
	if (a.x + a.w <= b.x	|| b.x + b.w <= a.x) {
		return false;
	}
	if (a.y + a.h <= b.y || b.y + b.h <= a.y) {
		return false;
	}
	return true;
}

// Check if given rectangle collides with any wall tiles
// (Bottom left, center, top, top right, right, bottom right, bottom, left, top left)
bool touchesWall(SDL_Rect box, Tile* tiles[]) {
	for (int i = 0; i < TOTAL_TILES; i++) {
		if (tiles[i]->getType() >= TILE_CENTER && tiles[i]->getType() <= TILE_TOPLEFT) {
			if (checkCollision(box, tiles[i]->getBox())) {
				return true;
			}
		}
	}
	return false;
}
