#include <iostream>
#include <string>
#include <conio.h>
using namespace std;


class AGameState;

//statemachine class
class GameStateMachine{
private:
	//states
	AGameState* mainPage;
	AGameState* chooseAdventurePage;
	AGameState* gameplayPage;
	AGameState* helpPage;
	AGameState* aboutPage;
	AGameState* hofPage;
	AGameState* newScorePage;

	AGameState* currentState;
public:
	GameStateMachine();
	AGameState* getCurrentState() { return this->currentState; }
	void setCurrentState(AGameState* newState) { this->currentState = newState; }

	//core methods
	void Draw();
	void Input();
	void Update();

	//accessors for every page
	AGameState* getMainPageState(){ return mainPage; }
	AGameState* getChooseAdventurePageState() { return chooseAdventurePage; }
	AGameState* getGameplayPageState(){ return gameplayPage; }
	AGameState* getHelpPageState(){ return helpPage; }
	AGameState* getAboutPageState(){ return aboutPage; }
	AGameState* getHofPageState(){ return hofPage; }
	AGameState* getNewHighScorePageState(){ return newScorePage; }
};



//abstract state
class AGameState{
protected:
	GameStateMachine* gsmRef;
public:
	AGameState(GameStateMachine* gsmMain);
	virtual void Draw() = 0;
	virtual void Input() { 
		int input = 0;
		//ASCII codes for ESC and ENTER
		while (input != 27 && input != 13){	
			if (_kbhit())
				input = _getch();
		}
		gsmRef->setCurrentState(gsmRef->getMainPageState());
	}
	void Update(){}
};



//concrete states
class MainMenu : public AGameState{
public:
	MainMenu(GameStateMachine* gsmMain) : AGameState(gsmMain){
		gsmRef = gsmMain;
	}
	void Draw(){
		system("CLS");
		cout << "Welcome to Zorkish Adventures" << endl;
		cout << "1. Select Adventure and Play" << endl;
		cout << "2. Hall Of Fame" << endl;
		cout << "3. Help" << endl;
		cout << "4. About" << endl;
		cout << "5. Quit" << endl << endl;
		cout << "Select 1-5:> ";
	}
	void Input(){
		char input = NULL;
		bool valid = false;

		while (valid == false){
			//get the input
			if (_kbhit())
				input = _getch();
			
			//if an input is detected
			if (input){
				switch (input){
				case '1':
					gsmRef->setCurrentState(gsmRef->getChooseAdventurePageState());
					valid = true;
					break;
				case '2':
					gsmRef->setCurrentState(gsmRef->getHofPageState());
					valid = true;
					break;
				case '3':
					gsmRef->setCurrentState(gsmRef->getHelpPageState());
					valid = true;
					break;
				case '4':
					gsmRef->setCurrentState(gsmRef->getAboutPageState());
					valid = true;
					break;
				case '5':
					gsmRef->setCurrentState(NULL); 
					valid = true;
					break;
				}
			}
		}
	}
};

class Gameplay : public AGameState{
public:
	Gameplay(GameStateMachine* gsmMain) : AGameState(gsmMain){
		gsmRef = gsmMain;
	}
	void Draw(){
		system("CLS");
		cout << "Welcome to Zorkish: ---world?---" << endl;
		cout << "This world is simple and pointless. Used it to test Zorkish phase 1 spec." << endl;
		cout << ":> ";
	}
	void Input(){
		string input;
		cin >> input;

		//simple command processing
		if (input == "hiscore"){
			gsmRef->setCurrentState(gsmRef->getNewHighScorePageState());
		}
		if (input == "quit"){
			cout << "Your adventure has ended without fame or fortune." << endl;
			gsmRef->setCurrentState(NULL);
		}
	}
};

class NewHighScore : public AGameState{
public:
	NewHighScore(GameStateMachine* gsmMain) : AGameState(gsmMain){
		gsmRef = gsmMain;
	}
	void Draw(){
		system("CLS");
		cout << "Congratulations!" << endl << endl;
		cout << "You have made it to the Zorkish Hall Of Fame" << endl << endl;
		cout << "Adventure:[the adventure world played]" << endl;
		cout << "Score : [the players score]" << endl;
		cout << "Moves : [number of moves player made]" << endl;
		cout << "Please type your name and press enter";
		cout << ":> ";
	};
};

class MainAdventure : public AGameState{
public:
	MainAdventure(GameStateMachine* gsmMain) : AGameState(gsmMain){
		gsmRef = gsmMain;
	}
	void Draw(){
		system("CLS");
		cout << "Choose your adventure:" << endl;
		cout << "1. Mountain World" << endl;
		cout << "2. Water World" << endl;
		cout << "3. Box World" << endl;
		cout << "Select 1-3:> ";
	}
	void Input(){
		char input = NULL;
		bool valid = false;

		while (valid == false){
			//get input
			if (_kbhit())
				input = _getch();

			//parse input
			//set the state for the new context
			if (input == '1' || input == '2' || input == '3'){
				gsmRef->setCurrentState(gsmRef->getGameplayPageState());
				valid = true;
			}
			else
				input = NULL;
		}
	}
};

class HallOfFame : public AGameState{
public:
	HallOfFame(GameStateMachine* gsmMain) : AGameState(gsmMain){
	gsmRef = gsmMain;
	}
	void Draw(){
		system("CLS");
		cout << "Top 10 Zorkish Adventure Champions" << endl;
		cout << "1. Fred, Mountain World, 5000" << endl;
		cout << "2. Mary, Mountain World, 4000" << endl;
		cout << "3. Joe, Water World, 3000" << endl;
		cout << "4. Henry, Mountain World, 2000" << endl;
		cout << "5. Susan, Mountain World, 1000" << endl;
		cout << "6. Alfred, Water World, 900" << endl;
		cout << "7. Clark, Mountain World, 800" << endl;
		cout << "8. Harold, Mountain World, 500" << endl; 
		cout << "9. Julie, Water World, 300" << endl;
		cout << "10. Bill, Box World, -5" << endl << endl;

		cout << "Press ESC or Enter to return to the Main Menu" << endl;
	}
};

class Help : public AGameState{
public:
	Help(GameStateMachine* gsmMain) : AGameState(gsmMain){
		gsmRef = gsmMain;
	}
	void Draw(){
		system("CLS");
		cout << "The following commands are supported:" << endl;
		cout << "quit," << endl;
		cout << "hiscore (for testing)" << endl << endl;

		cout << "Press ESC or Enter to return to the Main Menu" << endl;
	}
};

class About : public AGameState{
public:
	About(GameStateMachine* gsmMain) : AGameState(gsmMain){
		gsmRef = gsmMain;
	}
	void Draw(){
		system("CLS");
		cout << "Written by: Simon Greene" << endl << endl;

		cout << "Press ESC or Enter to return to the Main Menu" << endl;
	}
};




//gamestatemachine constructor and methods
GameStateMachine::GameStateMachine(){
	//initialise states
	mainPage = new MainMenu(this);
	hofPage = new HallOfFame(this);
	helpPage = new Help(this);
	aboutPage = new About(this);
	gameplayPage = new Gameplay(this);
	chooseAdventurePage = new MainAdventure(this);
	newScorePage = new NewHighScore(this);

	currentState = mainPage;
}
void GameStateMachine::Draw(){ currentState->Draw(); }
void GameStateMachine::Input(){ currentState->Input(); }
void GameStateMachine::Update(){ currentState->Update(); }

//gamestate constructor
AGameState::AGameState(GameStateMachine* gsmMain){
	gsmRef = gsmMain;
}


int main(){
	GameStateMachine gm;
	
	while (gm.getCurrentState() != NULL){
		gm.Draw();
		gm.Input();
	}

	return 0;
}