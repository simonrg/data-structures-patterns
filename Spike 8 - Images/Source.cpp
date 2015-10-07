#include <SDL.h>
#include <iostream>
using namespace std;

//screen dimensions
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;


//prototype
bool Initialise();
bool randomiseSprite(SDL_Rect, bool, bool, SDL_Rect);


SDL_Window* window = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* image = NULL;
SDL_Surface* player_img = NULL;


int main(int argc, char* args[]){
	//initialise the window
	if (!Initialise())
		return -1;

	bool running = true;
	bool show_bg = false;
	bool show_sprite = false;

	SDL_Event e;
	Uint32 bgColour = SDL_MapRGB(screen->format, 0, 0, 0);

	//load images
	image = SDL_LoadBMP("field.bmp");
	player_img = SDL_LoadBMP("dude.bmp");

	if (image == NULL || player_img == NULL){
		cout << "Error loading image: " << SDL_GetError() << endl;
		return -1;
	}

	//bounding area for sprites (x,y,w,h)
	//source divided into 3 parts
	SDL_Rect rect1 = { 0, 0, 100, 150 };
	SDL_Rect rect2 = { 110, 0, 110, 150 };
	SDL_Rect rect3 = { 220, 0, 120, 150 };

	//random bounding cell of sprite
	//moved by randomised x,y
	SDL_Rect cell = { 0, 0, 100, 150 };

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
				if (!show_bg){
					SDL_BlitSurface(image, NULL, screen, NULL);
					show_bg = true;
				}
				else{
					SDL_FillRect(screen, NULL, 0x000000);
					show_bg = false;
				}
				break;
			case SDLK_1:
				show_sprite = randomiseSprite(rect1, show_sprite, show_bg, cell);
				break;
			case SDLK_2:
				show_sprite = randomiseSprite(rect2, show_sprite, show_bg, cell);
				break;
			case SDLK_3:
				show_sprite = randomiseSprite(rect3, show_sprite, show_bg, cell);
				break;
			}
		}
		else if (e.type != SDL_KEYUP && !show_bg && !show_sprite)
			SDL_FillRect(screen, NULL, 0x000000);

		SDL_UpdateWindowSurface(window);
	}

	//clean up
	SDL_FreeSurface(screen);
	SDL_FreeSurface(image);
	SDL_FreeSurface(player_img);

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
	return 0;
}

//show the sprite using rectangles randomised location
bool randomiseSprite(SDL_Rect rect, bool show_sprite, bool background, SDL_Rect cell){
	//call the background again if still toggled on
	if (background)
		SDL_BlitSurface(image, NULL, screen, NULL);

	//if no sprite draw it
	//show/hide sprite when bg off, show/hide sprite when bg on
	if (!show_sprite){
		cell.x = rand() % SCREEN_WIDTH;
		cell.y = rand() % SCREEN_HEIGHT;

		SDL_BlitSurface(player_img, &rect, screen, &cell);
		show_sprite = true;
	}
	else if (!background && show_sprite){
		SDL_FillRect(screen, NULL, 0x000000);
		show_sprite = false;
	}
	else if (background && show_sprite){
		SDL_BlitSurface(image, NULL, screen, NULL);
		show_sprite = false;
	}

	return show_sprite;
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