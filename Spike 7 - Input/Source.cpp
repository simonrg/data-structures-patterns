#include <SDL.h>
#include <iostream>
using namespace std;

//screen dimensions
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;	//800


//prototype
bool Initialise();


SDL_Window* window = NULL;
SDL_Surface* screen = NULL;


int main(int argc, char* args[]){
	//initialise the window
	if (!Initialise())
		return -1;

	bool running = true;
	char state[10] = { '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' };	//0-9
	Uint32 bgColour = SDL_MapRGB(screen->format, 0, 0, 0);	//screen colour

	//main loop
	while (running){
		SDL_Event e;
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		//pop events
		while (SDL_PollEvent(&e)){

			if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
				running = false;

			//keys pressed down or released
			switch (e.type){
				case SDL_KEYDOWN:
					if (currentKeyStates[SDL_SCANCODE_1]){ state[0] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_2]){ state[1] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_3]){ state[2] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_4]){ state[3] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_5]){ state[4] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_6]){ state[5] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_7]){ state[6] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_8]){ state[7] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_9]){ state[8] = 'X'; }
					if (currentKeyStates[SDL_SCANCODE_0]){ state[9] = 'X'; }
					break;
				case SDL_KEYUP:
					for (int i = 0; i < sizeof(state); i++)
						state[i] = '-';
					break;
			}
		}

		SDL_FillRect(screen, NULL, bgColour);
		SDL_UpdateWindowSurface(window);

		//print keystate model
		SDL_Delay(100);
		for (int i = 0; i < sizeof(state); i++)
			cout << state[i];
		cout << endl;
	}

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