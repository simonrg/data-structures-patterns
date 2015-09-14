#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
using namespace std;

//screen dimensions
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;


//prototype
bool Initialise();


SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

Mix_Chunk* effect1 = NULL;
Mix_Chunk* effect2 = NULL;
Mix_Chunk* effect3 = NULL;
Mix_Music* music = NULL;


int main(int argc, char* args[]){
	bool running = true;

	//initialise the window
	if (!Initialise())
		return -1;

	Uint32 bgColour = SDL_MapRGB(screen->format, 0, 0, 0);
	SDL_Event e;

	//initialise mixer
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	music = Mix_LoadMUS("synth.wav");
	effect1 = Mix_LoadWAV("cannon_x.wav");
	effect2 = Mix_LoadWAV("boing_x.wav");
	effect3 = Mix_LoadWAV("bubbling2.wav");

	while (running){
		SDL_PollEvent(&e);

		if (e.key.keysym.sym == SDLK_ESCAPE)
			running = false;

		//select sound effect
		if (e.type == SDL_KEYDOWN){
			switch (e.key.keysym.sym){
			case SDLK_1:
				Mix_PlayChannel(-1, effect1, 0);
				break;
			case SDLK_2:
				Mix_PlayChannel(-1, effect2, 0);
				break;
			case SDLK_3:
				Mix_PlayChannel(-1, effect3, 0);
				break;
			case SDLK_0:
				if (Mix_PausedMusic()){
					Mix_ResumeMusic();
					break;
				}
				else if (Mix_PlayingMusic()){
					Mix_PauseMusic();
					break;
				}
				else{
					Mix_PlayMusic(music, -1);
					break;
				}
			}
		}

		SDL_FillRect(screen, NULL, bgColour);
		SDL_UpdateWindowSurface(window);
	}

	//cleanup
	Mix_FreeChunk(effect1);
	Mix_FreeChunk(effect2);
	Mix_FreeChunk(effect3);
	Mix_FreeMusic(music);
	Mix_CloseAudio();

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
	return 0;
}


//create window using specified dimensions
//apply a surface to the window
bool Initialise(){
	//initialise video and audio
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		cout << "Error initialising SDL: " << SDL_GetError() << endl;
		return false;
	}

	//create the window
	window = SDL_CreateWindow("My SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL){
		cout << "Error creating window: " << SDL_GetError() << endl;
		return false;
	}

	//apply a surface to the window
	screen = SDL_GetWindowSurface(window);
	return true;
}