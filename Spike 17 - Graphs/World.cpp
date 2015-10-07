#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

#include "World.h"

void World::add_locations(string locationstr){
	Location loc = Location(split(locationstr, '|'));
	locations.emplace(loc.getId(), loc);
}

void World::add_items(string itemstr){
	Item i = Item(split(itemstr, '|'));
	items.emplace(i.getId(), i);
}

Location World::current_location(){
	//access the location using the int pos (key) of the player object
	return locations[player.getLocationID()];
}

Item World::item_id_name(Location l, int id){
	int item_id = NULL;
	//items in the current location

	//a location contains a vector of item ids
	//traverse the vector and get the value (e.g. 1, 2, 5)
	//the item id must be matched to the World::map<int, Items> key to obtain the item name and description

	//traverse vector of ids and return the value
	item_id = l.getItems().at(id);

	//only returns the one item name for now as locations don't have more than one item
	return items[item_id];
}


Location::Location(){}

Location::Location(vector<string> bits){
	//initialise location fields
	id = stoi(bits[1]);
	name = bits[2];
	description = bits[3];
	//edge map of this location (e.g. n=1,e=3)
	string dir;
	int loc_id;
	for (string bit : split(bits[4], ',')){
		dir = split(bit, '=')[0];
		loc_id = stoi(split(bit, '=')[1]);
		edges[dir] = loc_id;
	}
	//items in this location
	items.push_back(stoi(bits[5]));
}



Item::Item(vector<string> bits){
	//initialise item fields
	id = stoi(bits[1]);
	name = bits[2];
	description = bits[3];
}