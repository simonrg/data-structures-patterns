#include <map>
#include <vector>

//not sure i like this here..
class Split{
public:
	vector<string> split(string rawstr, char split_on){
		vector<string> bits;

		int size = 0;
		int start = 0;
		for (size_t i = 0; i < rawstr.length(); i++){
			if (rawstr[i] == split_on){
				size = i - start;
				bits.push_back(rawstr.substr(start, size));
				start = i + 1;
			}
			if (i == rawstr.length() - 1){
				size = i - start;
				bits.push_back(rawstr.substr(start, size + 1));
			}
		}
		return bits;
	}
};

//L|id|name|desc|edges|item_ids
class Location : public Split{
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
};


class Item : public Split{
private:
	int id;
	string name;
	string description;
public:
	Item(){};
	Item(vector<string>);

	string getName(){ return name; }
	int getId(){ return id; }
};


class Player{
private:
	int location_id;
	vector<Item> items;
	int steps = 0;
	int gold = 0;
	bool playing = true;
public:
	int getLocationID(){ return location_id; }
	void setLocationID(int new_id){ location_id = new_id; }
};


class World : public Split{
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

	//methods
	void add_locations(string);
	void add_items(string);
	Location current_location();
	Item item_id_name(Location, int);
};