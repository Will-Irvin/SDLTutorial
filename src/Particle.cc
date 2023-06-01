#include <SDL2/SDL.h>

#include "Particle.hh"
#include "LTexture.hh"

#define DOT_WIDTH (20)
#define DOT_HEIGHT (20)

#define DOT_VEL (10)

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

Particle::Particle(int x, int y, LTexture *textures) {
	// Set offsets
	mPosX = x - 5 + (rand() % 25);
	mPosY = y - 5 + (rand() % 25);

	mFrame = rand() % 5;

	mTexture = &textures[rand() % 3];
}

void Particle::render(LTexture* shimmerTexture, SDL_Renderer* renderer) {
	mTexture->render(renderer, mPosX, mPosY); // Show actual image

	if (mFrame % 2 == 0) { // Shimmer every other frame
		shimmerTexture->render(renderer, mPosX, mPosY);
	}

	mFrame++;
}

bool Particle::isDead() {
	return mFrame > 10;
}

ParticleDot::ParticleDot(LTexture* textures) {
	mPosX = 0;
	mPosY = 0;

	mVelX = 0;
	mVelY = 0;

	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		particles[i] = new Particle(mPosX, mPosY, textures);
	}
}

ParticleDot::~ParticleDot() {
	for (int i = 0; i < TOTAL_PARTICLES; i++ ) {
		delete particles[i];
	}
}

void ParticleDot::handleEvent(SDL_Event& e) {
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

void ParticleDot::move() {
	mPosX += mVelX;
	// Too far, move back
	if (mPosX < 0 || mPosX + DOT_WIDTH > SCREEN_WIDTH) {
		mPosX -= mVelX;
	}

	mPosY += mVelY;
	if (mPosY < 0 || mPosY + DOT_HEIGHT > SCREEN_HEIGHT) {
		mPosY -= mVelY;
	}
}

void ParticleDot::render(LTexture* dotTexture, LTexture* textures,
												 LTexture* shimmerTexture, SDL_Renderer* renderer) {
	dotTexture->render(renderer, mPosX, mPosY);

	renderParticles(textures, shimmerTexture, renderer);
}

void ParticleDot::renderParticles(LTexture* textures, LTexture* shimmerTexture,
																	SDL_Renderer* renderer) {
	for (int i = 0; i < TOTAL_PARTICLES; i++) { // Reset dead particles
		if (particles[i]->isDead()) {
			delete particles[i];
			particles[i] = new Particle(mPosX, mPosY, textures);
		}
	}

	for (int i = 0; i < TOTAL_PARTICLES; i++) {
		particles[i]->render(shimmerTexture, renderer);
	}
}
