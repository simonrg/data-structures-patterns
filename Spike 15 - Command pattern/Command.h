#include <vector>

#include "Split.h"

//abstract command class
class ACommand{
protected:
	//all derived classes have a World reference
	World w;
public:
	ACommand(World world){};
	virtual void Process(vector<string>) = 0;

	//used to get the world after its been modified
	virtual World UpdatedWorld(){ return w; };
};


//concrete command classes
class CommandMove : public ACommand{
private:
	Location location;
	map<string, int> edges;
	map<string, int>::iterator it;
	string dir;
	Player p;
public:
	CommandMove(World world) : ACommand(world){ 
		w = world;
		location = w.currentLocation();
		edges = location.getEdges();
		p = w.getPlayer();
	}
	void Process(vector<string> args){
		//a 'move' command requires a direction
		if (args.size() == 1){
			cout << "Move where??" << endl;
			return;
		}

		dir = args.at(1);

		for (it = edges.begin(); it != edges.end(); it++){
			if (it->first == dir){
				p.setLocationID(it->second);		//set player pos using edge location id
				w.setPlayer(p);						//add the player to this world
				w.getLocations().find(it->second);

				//new location, new paths
				location = w.currentLocation();
				edges = location.getEdges();

				cout << "You moved " << dir << endl;
				return;
			}
		}
		cout << "There are no paths in that direction." << endl;	//single words that aren't in the look up arent a direction
	};
};

class CommandLook : public ACommand{
private:
	Location location;
	map<string, int> edges;
	map<string, int>::iterator it;
	string object;
public:
	CommandLook(World world) : ACommand(world){ 
		w = world;
		location = w.currentLocation();
		edges = location.getEdges();
	}
	void Process(vector<string> args){
		//look modes
		//environment
		if (args.size() == 1){
			//the location description
			cout << location.getDescription() << endl;

			//edges
			cout << "There are paths leading:" << endl;
			for (it = edges.begin(); it != edges.end(); it++){
				cout << "- " << it->first << endl;
			}

			//items
			if (!location.getItems().empty()){
				cout << "You can also see a:" << endl;
				for (size_t i = 0; i < location.getItems().size(); i++){
					//item is retrieved via item id in the location
					Item item = w.worldItems(location, i);
					cout << "- " << item.getName() << endl;
				}
			}
		}
		//object
		else{
			//the item to look at
			object = args.at(2);

			//item is in the location
			for (size_t i = 0; i < location.getItems().size(); i++){
				Item item = w.worldItems(location, i);

				if (item.getName() == object){
					cout << item.getDescription() << endl;
					return;
				}
			}

			//cant look at something not in the area obviously
			cout << "I don't see a [" << object << "] here." << endl;
		}
	};
};

class CommandQuit : public ACommand{
private:
	Player p;
public:
	CommandQuit(World world) : ACommand(world){ w = world; }
	void Process(vector<string>){
		cout << "Your adventure has ended without fame or fortune." << endl;
		p.setPlayState(false);
		w.setPlayer(p);
	}
};

class CommandInventory : public ACommand{
private:
	vector<Item> player_items;
	vector<Item>::iterator it;
public:
	CommandInventory(World world) : ACommand(world){ 
		w = world;
		player_items = w.getPlayer().getItems();
	}
	void Process(vector<string>){
		if (!player_items.empty()){
			cout << "I'm carrying" << endl;
			for (it = player_items.begin(); it != player_items.end(); it++){
				cout << "- " << it->getName() << "; it's " << it->getDescription() << endl;
			}
		}
		else{
			cout << "My inventory is empty." << endl;
			return;
		}
	}
};

class CommandTake : public ACommand{
private:
	string item_name;
	Location location;
	Player p;
public:
	CommandTake(World world) : ACommand(world){ 
		w = world;
		location = w.currentLocation();
		p = w.getPlayer();
	}
	void Process(vector<string> args){
		//a 'take' command requires an item to take
		if (args.size() == 1 || location.getItems().empty()){
			cout << "Theres nothing to take!" << endl;
			return;
		}

		//assuming 'take [item]'
		item_name = args.at(1);

		//iterate through items in the location
		for (size_t i = 0; i < location.getItems().size(); i++){
			//match item id with the id in the location's items collection
			//e.g. location has item 2, matches with the item with id 2
			Item item = w.worldItems(location, i);
			
			//take the item
			if (item.getName() == item_name){
				cout << "I picked up the [" << item.getName() << "]." << endl;

				//add item to player inventory
				p.addItem(item);
				w.setPlayer(p);

				//remove item from location
				location.removeItem(item.getId());
				w.update_location(location.getId(), location);
				return;
			}
		}
		cout << "Not sure what you're looking at.." << endl;
	}
};

class CommandPut : public ACommand{
private:
	string item;
	Player p;
	Location location;
	Item selecteditem;
public:
	CommandPut(World world) : ACommand(world){
		w = world;
		p = w.getPlayer();
		location = w.currentLocation();
	}
	void Process(vector<string> args){
		if (args.size() == 1 || p.getItems().empty()){
			cout << "Theres nothing to drop!" << endl;
			return;
		}

		//the item to drop
		item = args.at(1);
		selecteditem = p.SingleInventoryItem(item);

		//find it in the inventory and remove it
		p.removeItem(item);
		w.setPlayer(p);
		cout << "I dropped the [" << item << "]." << endl;

		//return it to the location
		location.addItem(selecteditem.getId());
		w.update_location(location.getId(), location);
	}
};

//organises commands
class CommandManager : public ACommand{
private:
	map<string, ACommand*> commands;
	map<string, ACommand*>::iterator it;
public:
	//the child initialises the world
	//because acommand is abstract
	CommandManager(World world) : ACommand(world){
		w = world;

		//concrete commands
		commands["move"] = new CommandMove(w);
		commands["go"] = commands["move"];

		commands["look"] = new CommandLook(w);
		commands["examine"] = commands["look"];

		commands["inventory"] = new CommandInventory(w);
		commands["i"] = commands["inventory"];
		commands["inv"] = commands["inventory"];

		commands["take"] = new CommandTake(w);
		commands["pick"] = commands["take"];
		commands["grab"] = commands["take"];
		commands["get"] = commands["take"];

		commands["put"] = new CommandPut(w);
		commands["drop"] = commands["put"];
		commands["remove"] = commands["put"];

		commands["quit"] = new CommandQuit(w);
		commands["exit"] = commands["quit"];
		commands["q"] = commands["quit"];
		commands["end"] = commands["quit"];
		commands["finish"] = commands["quit"];
	}
	void Process(vector<string> args){
		//each word in the command string
		for (it = commands.begin(); it != commands.end(); it++){

				
			//a direction is not a predefined command
			//becomes a directional command when move is appended
			if ((args.size() == 1) && (commands.find(args.at(0)) == commands.end())){
				args.insert(args.begin(), "move");
			}

			//is the first word a recognised command?
			if (it->first == args.at(0)){
				commands[args.at(0)]->Process(args);

				//the world passed back to the state will be updated
				w = commands[args.at(0)]->UpdatedWorld();
				return;
			}
			//its empty
			else if (args.empty())
				return;
		}

		cout << "Invalid command." << endl;
		return;
	}
};