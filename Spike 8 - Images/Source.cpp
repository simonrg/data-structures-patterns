#include <SDL.h>
#include <iostream>
using namespace std;

//screen dimensions
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;


//prototype
bool Initialise();
bool randomiseSprite(SDL_Rect, bool);


SDL_Window* window = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* image = NULL;
SDL_Surface* regions_img = NULL;
SDL_Surface* player_img = NULL;

//bounding cell for the sprite
//rectangles are moved using (x,y)
SDL_Rect rect = { 0, 0, 100, 150 };


int main(int argc, char* args[]){
	//initialise the window
	if (!Initialise())
		return -1;

	bool running = true;
	bool show_img = false;
	bool show_sprite = false;

	SDL_Event e;
	Uint32 bgColour = SDL_MapRGB(screen->format, 0, 0, 0);

	//load images
	image = SDL_LoadBMP("sample.bmp");
	regions_img = SDL_LoadBMP("regions.bmp");
	player_img = SDL_LoadBMP("dude.bmp");

	if (image == NULL || regions_img == NULL || player_img == NULL){
		cout << "Error loading image: " << SDL_GetError() << endl;
		return -1;
	}


	//main loop
	while (running){
		SDL_PollEvent(&e);

		//close program
		if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
			running = false;

		//input
		if (e.type == SDL_KEYDOWN){
			switch (e.key.keysym.sym){
			case SDLK_0:
				if (!show_img){
					SDL_BlitSurface(image, NULL, screen, NULL);
					show_img = true;
				}
				else{
					SDL_BlitSurface(regions_img, NULL, screen, NULL);
					show_img = false;
				}
				break;
			case SDLK_1:
				rect.x = rand() % SCREEN_WIDTH;
				rect.y = rand() % SCREEN_HEIGHT/5;

				show_sprite = randomiseSprite(rect, show_sprite);
				break;
			case SDLK_2:
				rect.x = rand() % SCREEN_WIDTH;
				rect.y = rand() % (SCREEN_HEIGHT / 5) + (SCREEN_HEIGHT / 3);

				show_sprite = randomiseSprite(rect, show_sprite);
				break;
			case SDLK_3:
				rect.x = rand() % SCREEN_WIDTH;
				rect.y = rand() % (SCREEN_HEIGHT / 5) + 
					((SCREEN_HEIGHT / 5) + (SCREEN_HEIGHT / 3));

				show_sprite = randomiseSprite(rect, show_sprite);
				break;
			default:
				SDL_BlitSurface(image, NULL, screen, NULL);
			}
		}
		else if (e.type != SDL_KEYUP && !show_img && !show_sprite)
			SDL_BlitSurface(regions_img, NULL, screen, NULL);

		SDL_UpdateWindowSurface(window);
	}

	//clean up
	SDL_FreeSurface(screen);
	SDL_FreeSurface(image);
	SDL_FreeSurface(regions_img);
	SDL_FreeSurface(player_img);

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
	return 0;
}

//show the sprite using rectangles randomised location
bool randomiseSprite(SDL_Rect rect, bool is_shown){
	if (!is_shown){
		SDL_BlitSurface(player_img, NULL, screen, &rect);
		is_shown = true;
	}
	else{
		SDL_BlitSurface(regions_img, NULL, screen, NULL);
		is_shown = false;
	}
	return is_shown;
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