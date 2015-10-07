#include <map>
#include <vector>

//L|id|name|desc|edges|item ids
class Location{
private:
	int id;
	string name;
	string description;
	map<string, int> edges;
	vector<int> items;
public:
	Location();
	Location(vector<string>);

	//get information about the location
	int getId(){ return id; }
	string getDescription(){ return description; }
	map<string, int> getEdges(){ return edges; }
	vector<int> getItems(){ return items; }

	//add and remove item indexes
	void addItem(int itemid){ items.push_back(itemid); }
	void removeItem(int itemid);
};


class Item{
private:
	int id;
	string name;
	string description;
public:
	Item(){};
	Item(vector<string>);

	int getId(){ return id; }
	string getName(){ return name; }
	string getDescription(){ return description; }
};


class Player{
private:
	int location_id;
	vector<Item> inventory;
	int steps = 0;
	int gold = 0;
	bool playing = true;
public:
	int getLocationID(){ return location_id; }
	void setLocationID(int new_id){ location_id = new_id; }

	bool getPlayState(){ return playing; }
	void setPlayState(bool playornot){ playing = playornot; }

	vector<Item> getItems(){ return inventory; }

	//add and remove items
	void addItem(Item item){ inventory.push_back(item); }
	void removeItem(string item);

	//retrieve a single item by name
	Item SingleInventoryItem(string itemname);
};


class World{
private:
	string name;
	string description;
	map<int, Location> locations;
	map<int, Item> items;
	Player player = Player();
public:
	//to access all the stuff in the world
	void setWorldName(string worldname){ name = worldname; }
	void setWorldDesc(string worlddesc){ description = worlddesc; }
	string getWorldName(){ return name; }
	string getWorldDesc(){ return description; }

	map<int, Location> getLocations(){ return locations; }

	void setPlayer(Player player_instance){ player = player_instance; }
	Player getPlayer(){ return player; }

	//methods
	void add_locations(string);
	void update_location(int, Location);
	void add_items(string);
	Location currentLocation();
	Item worldItems(Location, int);
};