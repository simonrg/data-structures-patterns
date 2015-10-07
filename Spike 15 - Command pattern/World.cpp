#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

#include "World.h"
#include "Split.h"

Split s;

void World::add_locations(string locationstr){
	Location loc = Location(s.split(locationstr, '|'));
	locations.emplace(loc.getId(), loc);
}

void World::update_location(int location_id, Location newlocationinfo){
	map<int, Location>::iterator it;

	for (it = locations.begin(); it != locations.end(); it++){
		if (location_id == it->first){
			locations.erase(location_id);
			locations.emplace(location_id, newlocationinfo);
			return;
		}
	}
}

void World::add_items(string itemstr){
	Item i = Item(s.split(itemstr, '|'));
	items.emplace(i.getId(), i);
}

Location World::currentLocation(){
	//access the location using the int pos (key) of the player object
	return locations[player.getLocationID()];
}

Item World::worldItems(Location l, int id){
	//from the ids in the location
	int item_id = l.getItems().at(id);

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
	for (string bit : s.split(bits[4], ',')){
		dir = s.split(bit, '=')[0];
		loc_id = stoi(s.split(bit, '=')[1]);
		edges[dir] = loc_id;
	}
	//items in this location
	if (!bits[5].find(' '))
		items.push_back(stoi(bits[5]));			//only one item
	else{
		for (string itemids : s.split(bits[5], ','))
			items.push_back(stoi(itemids));		//more than one item
	}
}

void Location::removeItem(int itemid){
	for (size_t i = 0; i < items.size(); i++){
		//find and remove it
		if (items.at(i) == itemid){
			items.erase(items.begin() + i);
			return;
		}
	}
}



Item::Item(vector<string> bits){
	//initialise item fields
	id = stoi(bits[1]);
	name = bits[2];
	description = bits[3];
}


Item Player::SingleInventoryItem(string item){
	Item current;
	//iterate through items in inventory
	for (size_t i = 0; i < inventory.size(); i++){
		//found it
		if (inventory[i].getName() == item){
			current = inventory[i];
			break;
		}
	}
	return current;
}

void Player::removeItem(string item){
	for (size_t i = 0; i < inventory.size(); i++){
		//find it
		if (inventory[i].getName() == item){
			inventory.erase(inventory.begin() + i);
			return;
		}
	}
}