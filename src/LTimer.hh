#ifndef LTIMER
#define LTIMER
class LTimer {
	public:
		LTimer();
		void start();
		void stop();
		void pause();
		void unpause();
		Uint32 getTicks();
		bool isStarted();
		bool isPaused();

	private:
		Uint32 mStartTicks; // Check when started
		Uint32 mPauseTicks; // Keep track of time paused

		// Timer status
		bool mPaused;
		bool mStarted;
};
#endif
