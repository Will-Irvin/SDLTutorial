#include <SDL2/SDL.h>

#include "LTexture.hh"

#define TOTAL_PARTICLES (20)
class Particle {
	public:
		Particle(int, int, LTexture*);
		void render(LTexture*, SDL_Renderer*);
		bool isDead();
	private:
		int mPosX, mPosY;
		int mFrame;
		LTexture *mTexture;
};

class ParticleDot {
	public:
		ParticleDot(LTexture*);
		~ParticleDot();
		void handleEvent(SDL_Event&);
		void move();
		void render(LTexture*, LTexture*, LTexture*, SDL_Renderer*);
	
	private:
		Particle* particles[TOTAL_PARTICLES];
		void renderParticles(LTexture*, LTexture*, SDL_Renderer*);

		int mPosX, mPosY;
		int mVelX, mVelY;
};
