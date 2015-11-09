#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

const int BOX_WIDTH = 50;
const int BOX_HEIGHT = 50;
const int BOX_SPEED = 10;
const int BOX_COUNT = 100;

const int TEST_TIME = 3 * 1000; // ie, 3 seconds

enum BoxState { CONTACT_NO, CONTACT_YES };

struct CrashBox {
	int x, y; // pos 
	int dx, dy; // vel
	int w, h; // size
	BoxState state; 
};

// Global variables. (Apparently evil.) 

SDL_Surface* screen = NULL;
SDL_Event event;

CrashBox boxes[BOX_COUNT];

void (*crash_test_all_ptr)(); // function pointer!


//-----------------------------------------------------------------------------


void init_boxes()
{
	// seed value - Set explicitly if you want repeatable results!!
	srand((unsigned)time(0));
	// set each box to a random position, size, and velocity
	for (int i = 0; i < BOX_COUNT; i++) {
		// position
		boxes[i].x = rand()%SCREEN_WIDTH;
		boxes[i].y = rand()%SCREEN_HEIGHT;
		// size
		boxes[i].w = (rand()%BOX_WIDTH)+1;
		boxes[i].h = (rand()%BOX_HEIGHT)+1;
		// velocity (both positive and negative "delta" values)
		boxes[i].dx = (rand()%(BOX_SPEED*2))-(BOX_SPEED/2);
		boxes[i].dy = (rand()%(BOX_SPEED*2))-(BOX_SPEED/2);
	}
}


void render_box(CrashBox box, int color) //TODO: try &box
{
	SDL_Rect r = { box.x, box.y, box.w, box.h}; //TODO: try cached rect's
	SDL_FillRect(screen, &r, color);
}

void render_boxes()
{
	for (int i = 0; i < BOX_COUNT; i++) {
		if (boxes[i].state == CONTACT_NO) {
			render_box(boxes[i], 0xFFFFFF); // white
		}
		else {
			render_box(boxes[i], 0xFF0000); // (blood) red
		}
	}
}

//-----------------------------------------------------------------------------

bool crash_test_A(int i, int j) // via index 
{   //yeap - lazyfoo style!

	//The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

	CrashBox A, B;
	A = boxes[i];
	B = boxes[j];

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;

    //If none of the sides from A are outside B
    return true;
}

bool crash_test_B(CrashBox A, CrashBox B) // struct (copy)
{
	//The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;
    //If none of the sides from A are outside B
    return true;
}

bool crash_test_C(CrashBox &A, CrashBox &B) // via struct (ref!)
{
	//The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;

    //If none of the sides from A are outside B
    return true;
}

bool crash_test_D(CrashBox &A, CrashBox &B)
{
    if ((A.y + A.h) <= B.y) return false;
    if (A.y >= (B.y + B.h)) return false;
	if ((A.x + A.w) <= B.x) return false;
	if (A.x >= (B.x + B.w)) return false;
    return true;
}

bool crash_test_E(CrashBox A, CrashBox B)
{
	if ((A.y + A.h) <= B.y) return false;
	if (A.y >= (B.y + B.h)) return false;
	if ((A.x + A.w) <= B.x) return false;
	if (A.x >= (B.x + B.w)) return false;
	return true;
}

//-----------------------------------------------------------------------------

void crash_test_all_A1() 
{
	// check i against j
	for (int i = 0; i < BOX_COUNT; i++) {
		for (int j = 0; j < BOX_COUNT; j++) {
			if (crash_test_A(i,j)) {
				if (i != j) {
					boxes[i].state = CONTACT_YES;
					boxes[j].state = CONTACT_YES;
				}
			}
		}
	}
}

void crash_test_all_A2() 
{
	// check i against j
	for (int i = 0; i < BOX_COUNT; i++) {
		for (int j = i+1; j < BOX_COUNT; j++) {
			if (crash_test_A(i,j)) {
				boxes[i].state = CONTACT_YES;
				boxes[j].state = CONTACT_YES;
			}
		}
	}
}

void crash_test_all_B() 
{
	// check i against j
	for (int i = 0; i < BOX_COUNT; i++) {
		for (int j = i+1; j < BOX_COUNT; j++) {
			if (crash_test_B(boxes[i],boxes[j])) {
				boxes[i].state = CONTACT_YES;
				boxes[j].state = CONTACT_YES;
			}
		}
	}
}

void crash_test_all_C() 
{
	// check i against j
	for (int i = 0; i < BOX_COUNT; i++) {
		for (int j = i+1; j < BOX_COUNT; j++) {
			if (crash_test_C(boxes[i],boxes[j])) {
				boxes[i].state = CONTACT_YES;
				boxes[j].state = CONTACT_YES;
			}
		}
	}
}

void crash_test_all_D()
{
	// check i against j
	for (int i = 0; i < BOX_COUNT; i++) {
		for (int j = i+1; j < BOX_COUNT; j++) {
			if (crash_test_D(boxes[i],boxes[j])) {
				boxes[i].state = CONTACT_YES;
				boxes[j].state = CONTACT_YES;
			}
		}
	}
}

void crash_test_all_E()
{
	// check i against j
	for (int i = 0; i < BOX_COUNT; i++) {
		for (int j = i + 1; j < BOX_COUNT; j++) {
			if (crash_test_E(boxes[i], boxes[j])) {
				boxes[i].state = CONTACT_YES;
				boxes[j].state = CONTACT_YES;
			}
		}
	}
}

//-----------------------------------------------------------------------------

void update_boxes()
{
	// First move all boxes
	for (int i = 0; i < BOX_COUNT; i++) {
		// update position using current velocity
		boxes[i].x = boxes[i].x + boxes[i].dx;
		boxes[i].y = boxes[i].y + boxes[i].dy;
		// check for wrap-around condition
		if (boxes[i].x >= SCREEN_WIDTH) boxes[i].x -= SCREEN_WIDTH;
		if (boxes[i].x < 0) boxes[i].x += SCREEN_WIDTH;
		if (boxes[i].y >= SCREEN_HEIGHT) boxes[i].y -= SCREEN_HEIGHT;
		if (boxes[i].y < 0) boxes[i].y += SCREEN_HEIGHT;
		//TODO: try else if logic
	}

	// 1. mark all boxes as not collided //TODO: put this in the move loop?
	for (int i = 0; i < BOX_COUNT; i++) 
		boxes[i].state = CONTACT_NO;
	// 2. call whatever function has been set to test all i against j boxes
	crash_test_all_ptr();
}


//-----------------------------------------------------------------------------

int run_test(char * title, void (*function_ptr)())
{ 
	// SETUP  

	// get SDL to setup all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) { return 1; }
	// prepare a screen surface for drawing to...
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if( screen == NULL ) { return 1; }
	// set pretty window caption
    SDL_WM_SetCaption( title, NULL );
	// get the standard output back to the console
	freopen( "CON", "wt", stdout );
	freopen( "CON", "wt", stderr );

	// some pretty test output to the console
	printf("-- New Test: %s\n", title);
	crash_test_all_ptr = function_ptr;
	// sanity check that the crash test function pointer has been set
	if (crash_test_all_ptr == NULL) {
		printf("EH? Set the crash_test_all_ptr first!\n");
		return 1;
	}

	// initialise each crashbox
	init_boxes();

	// initialise test count/time values 
	Uint32 loop_count = 0;
	Uint32 tick_start = SDL_GetTicks(); // start time == now! 
	Uint32 tick_target = tick_start + TEST_TIME; // when to stop

	// CLASSIC GAME LOOP
	bool running = true;
	while (running)	{
		// UPDATE
		// 1. check for quit event
        while( SDL_PollEvent( &event ) ) {
            if( event.type == SDL_QUIT ) running = false;
        }
		// 2. count...
		loop_count++;
		// 3. check for test time finished
		if (SDL_GetTicks() >= tick_target) running = false;
		// 4. move all the crash boxes and check for collisions
		update_boxes();

		// RENDER
		if (false) {
			// 1. clear the background
			SDL_FillRect(screen, NULL, 0x000000);
			// 2. render all boxes
			render_boxes();
			// 3. fill the screen to show it
			SDL_Flip(screen);
		}
		
		//SDL_Delay(50); // uncomment to see at slow speed.
	}
	// note the end time
	Uint32 tick_end = SDL_GetTicks();

	// CLEAN UP TIME (and close the SDL window)
	SDL_Quit();

	// SHOW STATS
	printf("Loops: %d\n", loop_count);
	printf("Time: %d (ms)\n", (tick_end - tick_start));
	printf("Loops/Second: %f\n", (float(loop_count) / (tick_end - tick_start)*1000.0));

	return 0;
}

int main( int argc, char* args[])
{
	run_test("Test A1", crash_test_all_A1); //TODO: not paying attention to return values. :(
	run_test("Test A2", crash_test_all_A2);
	run_test("Test B", crash_test_all_B);
	run_test("Test C", crash_test_all_C);
	run_test("Test D", crash_test_all_D);
	run_test("Test E", crash_test_all_E);

	printf("\n<press any key>\n");
	getchar();

	return 0;
}

