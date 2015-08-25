#include <iostream>
#include <ctype.h>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>

using namespace std;


//board
char board[8][8] =
{
	{ '#', '#', '#', '#', '#', '#', '#', '#' },
	{ '#', 'G', ' ', 'D', '#', 'D', ' ', '#' },
	{ '#', ' ', ' ', ' ', '#', ' ', ' ', '#' },
	{ '#', '#', '#', ' ', '#', ' ', 'D', '#' },
	{ '#', ' ', ' ', ' ', '#', ' ', ' ', '#' },
	{ '#', ' ', '#', '#', '#', '#', ' ', '#' },
	{ '#', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
	{ '#', '#', '*', '#', '#', '#', '#', '#' }
};


void show_map(){
	cout << "Welcome to GridWorld: Quantised Excitement. Fate is waiting for You!" << endl;
	cout << "Valid commands: N, S, E and W for direction. Q to quit the game." << endl;
	cout << "----------------------------------------------------------------" << endl;

	for (int row = 0; row < 8; row++){
		for (int col = 0; col < 8; col++){
			cout << board[row][col];
		}
		cout << endl;
	}
}

void change_pos(int row, int col){
	//clear old position
	for (int row = 0; row < 8; row++){
		for (int col = 0; col < 8; col++){
			if (board[row][col] == '*'){
				board[row][col] = ' ';
			}
		}
	}

	//mark new position
	board[row][col] = '*';
}

void update_model(char &input){
	int p_row, p_col = 0;

	//get the row and column of current player pos
	for (int row = 0; row < 8; row++){
		for (int col = 0; col < 8; col++){
			if (board[row][col] == '*'){
				p_row = row;
				p_col = col;
			}
		}
	}

	//check if move is valid
	switch (input){
	case 'N':
		//decrement row (up)
		--p_row;
		
		if (board[p_row][p_col] == '#')
			cout << "--Path blocked--" << endl;
		else
			change_pos(p_row, p_col);
		break;
	case 'S':
		//increment row (down)
		if (board[++p_row][p_col] == '#' || p_row > 7)	//so you can't go outside the game world
			cout << "--Path blocked--" << endl;
		else
			change_pos(p_row, p_col);
		break;
	case 'E':
		//increment column (right)
		if (board[p_row][++p_col] == '#')
			cout << "--Path blocked--" << endl;
		else
			change_pos(p_row, p_col);
		break;
	case 'W':
		//decrement column (left)
		if (board[p_row][--p_col] == '#')
			cout << "--Path blocked--" << endl;
		else
			change_pos(p_row, p_col);
		break;
	case 'Q':
		exit(0);
	}
}

bool game_over(){
	//end game condition
	if (board[1][1] == '*'){
		cout << "Wow - you've discovered a large chest filled with GOLD coins!" << endl;
		cout << "YOU WIN!" << endl;
		cout << "Thanks for playing. There probably won't be a next time." << endl;
		return false;
	}
	if (board[1][3] == '*' || board[1][5] == '*' || board[3][6] == '*'){
		cout << "Arrrrgh... you've fallen down a pit." << endl;
		cout << "YOU HAVE DIED!" << endl;
		cout << "Thanks for playing. Maybe next time." << endl;
		return false;
	}
	return true;
}


int main(){
	//variables
	char input = NULL;
	bool playing = true;

	//game loop
	while (playing == true){

		//render
		show_map();

		//input
		if (_kbhit()){
			input = toupper(_getch());
			cout << "Command entered: " << input << endl;
		}

		//update
		if (input)
			update_model(input);

		//refresh the screen
		//timer to prevent excessive screen flicker
		Sleep(0500);
		system("CLS");

		//check if the player has won or lost
		playing = game_over();
	}

	system("pause");
	return 0;
}
