#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

#include "Adventure.h"
#include "States.h"


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
	virtual void Update(){
	}
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

class WelcomeWorld : public AGameState{
private:
	World game_world;
public:
	WelcomeWorld(GameStateMachine* gsmMain) : AGameState(gsmMain){
		   gsmRef = gsmMain;
	}
	void Draw(){
		cout << "Welcome to Zorkish: " << gsmRef->getAdventure()->getWorld().getWorldName() << endl;
		cout << gsmRef->getAdventure()->getWorld().getWorldDesc() << endl << endl;
	}
};

class Gameplay : public AGameState{
private:
	World game_world;
	Player player;
public:
	Gameplay(GameStateMachine* gsmMain) : AGameState(gsmMain){
		gsmRef = gsmMain;
	}
	void Draw(){
		cout << ":> ";
	}
	void Input(){
		//world and location data for current area
		game_world = gsmRef->getAdventure()->getWorld();
		Location currentlocation = game_world.current_location();
		map<string, int> edges = currentlocation.getEdges();
		bool valid_dir = false;
		//game_world.find_item(currentlocation);
		//get command entered
		string input;
		cin >> input;

		//commands
		if (input == "look"){
			//description of current location
			cout << currentlocation.getDescription() << endl;

			//print edge paths for current location
			map<string, int>::iterator it;
			cout << "There are paths leading:" << endl;
			for (it = edges.begin(); it != edges.end(); it++){
				cout << "- " << it->first << endl;
			}

			//items in this location
			cout << "You can also see a:" << endl;
			for (size_t i = 0; i < currentlocation.getItems().size(); i++){
				//item description using item id from the location
				Item item = game_world.item_id_name(currentlocation, i);
				cout << "- " << item.getName() << endl;
			}
		}
		else if (input == "s" || input == "n" || input == "e" || input == "w"){
			//edges are available paths from the current pos
			map<string, int>::iterator it;

			for (it = edges.begin(); it != edges.end(); it++){
				//if the command matches a valid direction
				if (it->first == input){
					player.setLocationID(it->second);	//set location of local player instance
					game_world.setPlayer(player);		//add the player to the world of the current adventure
					game_world.getLocations().find(it->second);

					//new location, new paths
					currentlocation = game_world.current_location();
					edges = currentlocation.getEdges();

					//update information in the current adventure
					gsmRef->getAdventure()->setWorld(game_world);

					cout << "You moved " << input << endl;
					valid_dir = true;
					break;
				}
			}
			if (valid_dir == false)
				cout << "There are no paths in that direction." << endl;
		}
		else if (input == "hiscore"){
			gsmRef->setCurrentState(gsmRef->getNewHighScorePageState());
		}
		else if (input == "quit"){
			cout << "Your adventure has ended without fame or fortune." << endl;
			gsmRef->setCurrentState(NULL);
		}
		else{
			cout << "Invalid command." << endl;
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
private:
	//a world instance to initialise the adventure world
	World game_world;
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
				//change the page state
				gsmRef->setCurrentState(gsmRef->getGameplayPageState());

				//set some info about the world from selected adventure
				game_world.setWorldName("Mountain World");
				game_world.setWorldDesc("Lush rolling meadows with a picturesque mountain range in the distance.");
				gsmRef->getAdventure()->setWorld(game_world);

				//load adventure data for this world from the textfile
				gsmRef->getAdventure()->Load("demoworldnew.txt");

				//leave the state
				valid = true;
				system("CLS");
				gsmRef->getWorldWelcomeHeader()->Draw();
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
	welcomeWorldHeader = new WelcomeWorld(this);
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