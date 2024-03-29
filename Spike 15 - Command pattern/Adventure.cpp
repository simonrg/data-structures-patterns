#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "Adventure.h"
#include "States.h"
//#include "Command.h"


Adventure::Adventure(){
	world = World();
}

void Adventure::Load(string filepath){
	//open file
	ifstream file;
	file.open(filepath);

	if (!file.is_open()){
		cout << "Incorrect file path." << endl;
		return;
	}

	//read lines
	string line;
	for (int i = 0; getline(file, line); i++){
		if (line[0] == 'L'){		//locations in the world
			world.add_locations(line);
		}
		else if (line[0] == 'I'){	//items in the world
			world.add_items(line);
		}
	}

	file.close();
}

void Adventure::Play(){
	GameStateMachine gm;
	gm.setAdventure(this);

	while (world.getPlayer().getPlayState() == true){
		gm.Update();
		gm.Draw();
		gm.Input();
	}
}