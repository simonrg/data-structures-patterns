#include <vector>

class Item;
class Player;
class Location;


//component objects

//stores and handles meta data of an entity
//description info for any type e.g. a location, an item
class MetaComponent{
private:
	int id;
	string name;
	string description;
public:
	int Id(){ return id; }
	string Name(){ return name; }
	string Description(){ return description; }

	void entity_info(int id, string name, string description);
};

//stores and handles the entity's health
class HealthComponent{
private:
	int health;
public:
	HealthComponent(){ health = 100; }

	int health_status(){ return health; }

	//add or subtract health from total pool
	void change_health(int new_hp);
};

//stores and handles the entity position
//edges, items for a specific area
class PositionComponent{
private:
	static int locationid;
public:
	PositionComponent(){}

	int position_id(){ return locationid; }

	void update_position(int new_id);
};

//stores a collection of items
//handles adding/removing items from an objects collection
//e.g. a player, an enemy, a container, a location
class CollectionComponent{
private:
	vector<Item> collection;
public:
	CollectionComponent(){}

	vector<Item> entity_collection(){ return collection; }

	void update_items(Item item);
};

//maintains collections of all world related stuff
class WorldResourcesComponent{
private:
	map<int, Location> locations;
	map<int, Item> items;
public:
	WorldResourcesComponent(){};

	map<int, Location> getLocations(){ return locations; }
	map<int, Item> getItems(){ return items; }

	Item item_current_location(Location l, int id);

	void add_locations(string filestring);
	void add_items(string filestring);
};

//manages location information
//e.g. edges, item ids, updating location state
class LocationManagerComponent{
private:
	map<int, Location> locations;
	map<string, int> edges;
	vector<int> item_ids;
	PositionComponent p;
public:
	LocationManagerComponent(){};
	LocationManagerComponent(map<int, Location> worldlocations);

	map<string, int>& getEdges(){ return edges; }
	vector<int>& getItems(){ return item_ids; }

	Location current_location(int id);
	void update_location(int, Location);
	void addItem(int itemid);
	void removeItem(int itemid);
};