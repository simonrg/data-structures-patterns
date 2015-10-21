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
		p = w.getPlayer();
		location = w.LocationManager().current_location(p.Position().position_id());
		edges = location.LocationManager().getEdges();
	}
	void Process(vector<string> args){
		//a 'move' command requires a direction
		if (args.size() == 1){
			cout << "Move where??" << endl;
			return;
		}

		//move this way
		dir = args.at(1);

		for (it = edges.begin(); it != edges.end(); it++){
			if (it->first == dir){
				//player pos calculated from edge location id
				p.Position().update_position(it->second);

				w.setPlayer(p);										//update this world's player object
				w.Resources().getLocations().find(it->second);

				//new location, new paths
				location = w.LocationManager().current_location(p.Position().position_id());
				edges = location.LocationManager().getEdges();

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
	Player p;
public:
	CommandLook(World world) : ACommand(world){ 
		w = world;
		p = w.getPlayer();
		location = w.LocationManager().current_location(p.Position().position_id());
		edges = location.LocationManager().getEdges();
	}
	void Process(vector<string> args){
		//look modes
		//environment
		if (args.size() == 1){
			//the location description
			cout << location.Info().Description() << endl;

			//edges
			cout << "There are paths leading:" << endl;
			for (it = edges.begin(); it != edges.end(); it++){
				cout << "- " << it->first << endl;
			}

			//items
			if (!location.LocationManager().getItems().empty()){
				cout << "You can also see a:" << endl;
				for (size_t i = 0; i < location.LocationManager().getItems().size(); i++){
					//item is retrieved via item id in the location
					Item item = w.Resources().item_current_location(location, i);
					cout << "- " << item.Info().Name() << endl;
				}
			}
		}
		//object
		else{
			//the item to look at
			object = args.at(2);

			//item is in the location
			for (size_t i = 0; i < location.LocationManager().getItems().size(); i++){
				Item item = w.Resources().item_current_location(location, i);

				if (item.Info().Name() == object){
					cout << item.Info().Description() << endl;
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
		exit(0);
	}
};

class CommandInventory : public ACommand{
private:
	vector<Item> player_items;
	vector<Item>::iterator it;
public:
	CommandInventory(World world) : ACommand(world){ 
		w = world;
		player_items = w.getPlayer().Items().entity_collection();
	}
	void Process(vector<string>){
		if (!player_items.empty()){
			cout << "I'm carrying" << endl;
			for (it = player_items.begin(); it != player_items.end(); it++){
				cout << "- " << it->Info().Name() << "; it's " << it->Info().Description() << endl;
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
		p = w.getPlayer();
		location = w.LocationManager().current_location(p.Position().position_id());
	}
	void Process(vector<string> args){
		//a 'take' command requires an item to take
		if (args.size() == 1 || location.LocationManager().getItems().empty()){
			cout << "Theres nothing to take!" << endl;
			return;
		}

		//assuming 'take [item]'
		item_name = args.at(1);

		//iterate through items in the location
		for (size_t i = 0; i < location.LocationManager().getItems().size(); i++){
			//take an item from the current location
			Item item = w.Resources().item_current_location(location, i);
			
			//take the item
			if (item.Info().Name() == item_name){
				cout << "I picked up the [" << item.Info().Name() << "]." << endl;

				//add item to player inventory
				p.Items().update_items(item);
				w.setPlayer(p);

				//remove item from location
				location.LocationManager().removeItem(item.Info().Id());

				//update the location
				w.LocationManager().update_location(location.Info().Id(), location);
				return;
			}
		}
		cout << "Not sure what you're looking at.." << endl;
	}
};

class CommandPut : public ACommand{
private:
	string drop_item;
	Player p;
	Location location;
	Item selecteditem;
public:
	CommandPut(World world) : ACommand(world){
		w = world;
		p = w.getPlayer();
		location = w.LocationManager().current_location(p.Position().position_id());
	}
	void Process(vector<string> args){
		if (args.size() == 1 || p.Items().entity_collection().empty()){
			cout << "Theres nothing to drop!" << endl;
			return;
		}

		//the item to drop
		drop_item = args.at(1);
		
		//iterate through items in the inventory
		for (size_t i = 0; i < p.Items().entity_collection().size(); i++){
			Item item = p.Items().entity_collection().at(i);

			//drop the item
			if (p.Items().entity_collection()[i].Info().Name() == drop_item){
				cout << "I dropped the [" << drop_item << "]." << endl;

				//remove from player inventory
				p.Items().update_items(item);
				w.setPlayer(p);

				//return the item to the location
				location.LocationManager().addItem(item.Info().Id());
				//location.addItem(item.Info().Id());

				//update the location
				w.LocationManager().update_location(location.Info().Id(), location);
				return;
			}
		}		
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