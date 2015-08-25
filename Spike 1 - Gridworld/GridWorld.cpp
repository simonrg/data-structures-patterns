#include <iostream>
#include <ctype.h>
#include <cstdlib>

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
	for (int row = 0; row < 8; row++){
		for (int col = 0; col < 8; col++){
			cout << board[row][col];
		}
		cout << endl;
	}
}

void player_move(char &input){
	cout << "Enter a command: ";

	//get player input
	cin >> input;
	input = toupper(input);
}

void clear_previous_pos(){
	//clear
	for (int row = 0; row < 8; row++){
		for (int col = 0; col < 8; col++){
			if (board[row][col] == '*'){
				board[row][col] = ' ';
			}
		}
	}
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
		if (board[--p_row][p_col] == '#'){
			cout << "--Path blocked--" << endl;
		}
		else{
			clear_previous_pos();
			board[p_row][p_col] = '*';
		}
		break;
	case 'S':
		//increment row (down)
		if (board[++p_row][p_col] == '#' || p_row > 7){	//so you can't go outside the game world
			cout << "--Path blocked--" << endl;
		}
		else{
			clear_previous_pos();
			board[p_row][p_col] = '*';
		}
		break;
	case 'E':
		//increment column (right)
		if (board[p_row][++p_col] == '#'){
			cout << "--Path blocked--" << endl;
		}
		else{
			clear_previous_pos();
			board[p_row][p_col] = '*';
		}
		break;
	case 'W':
		//decrement column (left)
		if (board[p_row][--p_col] == '#'){
			cout << "--Path blocked--" << endl;
		}
		else{
			clear_previous_pos();
			board[p_row][p_col] = '*';
		}
		break;
	case 'Q':
		exit(0);
	default:
		cout << "Invalid command. 'N' to move north, 'E' to move east, 'S' to move south, 'W' to move west or 'Q' to quit." << endl;
		break;
	}
}


int main(){
	//variables
	bool gameover = false;
	char input;

	//instructions
	cout << "Welcome to GridWorld: Quantised Excitement. Fate is waiting for You!" << endl;
	cout << "Valid commands: N, S, E and W for direction. Q to quit the game." << endl;

	//game loop
	while (gameover == false){
		//output
		show_map();

		//input
		player_move(input);

		//update model
		update_model(input);

		//end game condition
		if (board[1][1] == '*'){
			cout << "Wow - you've discovered a large chest filled with GOLD coins!" << endl;
			cout << "YOU WIN!" << endl;
			cout << "Thanks for playing. There probably won't be a next time." << endl;
			gameover = true;
		}
		if (board[1][3] == '*' || board[1][5] == '*' || board[3][6] == '*'){
			cout << "Arrrrgh... you've fallen down a pit." << endl;
			cout << "YOU HAVE DIED!" << endl;
			cout << "Thanks for playing. Maybe next time." << endl;
			gameover = true;
		}
	}

	system("pause");
	return 0;
}
