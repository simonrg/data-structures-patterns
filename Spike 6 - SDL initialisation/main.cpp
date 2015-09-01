#include <SDL.h>
#include <iostream>
using namespace std;

//screen dimensions
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;


//prototype
bool Initialise();


SDL_Window* window = NULL;
SDL_Surface* screen = NULL;


int main(int argc, char* args[]){
	bool running = true;

	//initialise the window
	if (!Initialise())
		return -1;

	SDL_Event e;				//event stack
	Uint32 bgColour = SDL_MapRGB(screen->format, 0, 0, 0);	//black screen

	while (running){
		//pop event
		while (SDL_PollEvent(&e) != 0){

			//check event
			if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
				running = false;
		}

		SDL_FillRect(screen, NULL, bgColour);
		SDL_UpdateWindowSurface(window);
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