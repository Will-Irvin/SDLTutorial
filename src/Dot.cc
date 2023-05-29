#include <SDL2/SDL.h>

#include "Dot.hh"
#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

bool checkCollision(SDL_Rect, SDL_Rect);

// Constructor
Dot::Dot() {
	mPosX = 0;
	mPosY = 0;
	mVelX = 0;
	mVelY = 0;

	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;
}

void Dot::handleEvent(SDL_Event& e) {
	// Start moving dot
	// e.key.repeat makes sure we only look at the first key press
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

// Move freely
void Dot::move() {
	mPosX += mVelX;
	// Too far, move back
	if (mPosX < 0 || mPosX + DOT_WIDTH > SCREEN_WIDTH) {
		mPosX -= mVelX;
	}

	mPosY += mVelY;
	if (mPosY < 0 || mPosY + DOT_HEIGHT > SCREEN_HEIGHT) {
		mPosY -= mVelY;
	}

	mCollider.x = mPosX;
	mCollider.y = mPosY;

}

// Move while checking that there is no collision with the given rectangle
void Dot::move(SDL_Rect& wall, bool checkCollision(SDL_Rect, SDL_Rect)) {
	mPosX += mVelX;
	mCollider.x = mPosX;
	// Too far, move back
	if (mPosX < 0 || mPosX + DOT_WIDTH > SCREEN_WIDTH || checkCollision(mCollider, wall)) {
		mPosX -= mVelX;
		mCollider.x = mPosX;
	}

	mPosY += mVelY;
	mCollider.y = mPosY;
	if (mPosY < 0 || mPosY + DOT_HEIGHT > SCREEN_HEIGHT || checkCollision(mCollider, wall)) {
		mPosY -= mVelY;
		mCollider.y = mPosY;
	}
}

void Dot::render(SDL_Renderer* renderer, LTexture* texture_ptr) {
	texture_ptr->render(renderer, mPosX, mPosY);
}
