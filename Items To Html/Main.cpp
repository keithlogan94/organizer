#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;


class Jason {
private:
	string jsonObject = "{ \n";
	vector<string> keyValuePair;
	inline void formatJson() {
		for (int i = 0; i < keyValuePair.size(); i++) {
			jsonObject += keyValuePair.at(i) + (i == keyValuePair.size() -1/*last*/ ? "" : ",") + '\n';
		}
		jsonObject += "\n }";
	}
public:
	inline const string getJson() { formatJson(); return jsonObject; }
	inline void addKeyValue(string key, string value) {
		keyValuePair.push_back(string("\"" + key + "\":" + "\"" + value + "\""));
	}
	inline void addKeyValue(string key, int value) {
		keyValuePair.push_back(string("\"" + key + "\":" + to_string(value)));
	}
	inline void addKeyValue(string key, double value) {
		keyValuePair.push_back(string("\"" + key + "\":" + to_string(value)));
	}
	inline void addKeyValue(string key, Jason json) {
		keyValuePair.push_back(string("\"" + key + "\":" + json.getJson()));
	}
	inline void addKeyValue(string key, vector<int> values) {
		string keyValue = "";
		keyValue += "\"" + key + "\": [";
		for (int i = 0; i < values.size(); i++) {
			keyValue += to_string(values.at(i)) + string((i == values.size() - 1 ? "" : ", ")) + string((i % 5 == 0 ? "\n" : ""));
		}
		keyValue += "]";
		keyValuePair.push_back(keyValue);
	}
	inline void addKeyValue(string key, vector<double> values) {
		string keyValue = "";
		keyValue += "\"" + key + "\": [";
		for (int i = 0; i < values.size(); i++) {
			keyValue += to_string(values.at(i)) + string((i == values.size() - 1 ? "" : ", ")) + string((i % 5 == 0 ? "\n" : ""));
		}
		keyValue += "]";
		keyValuePair.push_back(keyValue);
	}
	inline void addKeyValue(string key, vector<string> values) {
		string keyValue = "";
		keyValue += "\"" + key + "\": [";
		for (int i = 0; i < values.size(); i++) {
			keyValue += "\"" + values.at(i) + "\"" + string((i == values.size() - 1 ? "" : ", ")) + string((i % 5 == 0 ? "\n" : ""));
		}
		keyValue += "]";
		keyValuePair.push_back(keyValue);
	}
	inline void addKeyValue(string key, vector<Jason> values) {
		string keyValue = "";
		keyValue += "\"" + key + "\": [";
		for (int i = 0; i < values.size(); i++) {
			keyValue += values.at(i).getJson() + string((i == values.size() - 1 ? "" : ", ")) + "\n";
		}
		keyValue += "]";
		keyValuePair.push_back(keyValue);
	}
};

class Item {
private:
	string name;
	int qty = 0;
	int price= 0;
	long int time = 0;
private:
	inline string getAttribute(string attr, string value) {
		return string(attr + "=\"") + value + string("\"");
	}
	inline const string replaceSpace(char c, string str) {
		string str2;
		for (auto ch : str)
		{
			if (ch == ' ') {
				str2 += c;
				continue;
			}
			str2 += ch;
		}
		return str2;
	}
public:
	inline const string getPrice() const { return to_string(price); }
	inline const string getQty() const { return to_string(qty); }
	inline const string getTime() const { return to_string(time); }
	inline const string getName() const { return name; }
	inline void setName(string name) { this->name = name; }
	inline void setQty(int qty) { this->qty = qty; }
	inline void setPrice(int price) { this->price = price; }
	inline void setTime(int timeStamp) { this->time = timeStamp; }
	inline const string getLi() {
		return string("<li ") + 
			getAttribute(" id", replaceSpace('-', getName())) + 
			getAttribute(" min-value", getPrice()) + 
			getAttribute(" time-stamp", getTime()) +
			getAttribute(" quantity", getQty()) +
			'>'+ getName() + string("</li>");
	}
};

#define _FAIL is.setstate(ios::failbit); return is;
istream& operator>>(istream& is, Item& item) {
	char c;
	string str;
	if (!(is >> c) || c != '{') { _FAIL }
	if (!(is >> c) || c != '\"') { _FAIL }
	while (is >> c && c != '\"')
		;
	if (!(is >> c) || c != ':') { _FAIL }
	if (!(is >> c) || c != '\"') { _FAIL }
	while (is.get(c) && c != '\"')
	{
		str += c;
	}
	item.setName(str);
	str.clear();
	if (!(is >> c) || c != ',') { _FAIL }
	if (!(is >> c) || c != '\"') { _FAIL }
	while (is >> c && c != '\"')
		;
	if (!(is >> c) || c != ':') { _FAIL }
	int number;
	is >> number;
	item.setQty(number);
	number = 0;
	if (!(is >> c) || c != ',') { _FAIL }
	if (!(is >> c) || c != '\"') { _FAIL }
	while (is >> c && c != '\"')
		;
	if (!(is >> c) || c != ':') { _FAIL }
	is >> number;
	item.setPrice(number);
	number = 0;
	if (!(is >> c) || c != ',') { _FAIL }
	if (!(is >> c) || c != '\"') { _FAIL }
	while (is >> c && c != '\"')
		;
	if (!(is >> c) || c != ':') { _FAIL }
	is >> number;
	item.setTime(number);
	number = 0;
	if (!(is >> c) || c != '}') { _FAIL }
	return is;
}

int main(int argc, char ** argv) {
	if (argc != 3) {
		cerr << "error with usage: *.exe [filename(from)] [filename(to)]\n";
		return 1;
	}
	ifstream ifs(argv[1], ios::beg);
	vector<Item> iList;
	if (ifs.is_open()) 
	{
		Item item;
		while (ifs >> item)
		{
			iList.push_back(item);
		}
		ifs.close();
	}
	//we have extracted all of the items

	//each item in chronological order in its own list
	map<string, vector<Item>> itemsByName;
	for (auto item : iList) {
		itemsByName[item.getName()].push_back(item);
	}

	vector<Jason> jsons;
	for (map<string, vector<Item>>::iterator list = itemsByName.begin(); list != itemsByName.end(); list++) {
		Jason jason;
		jason.addKeyValue("itemName", list->second.at(0).getName());
		vector<Jason> prices;
		for (vector<Item>::iterator itemInList = (*list).second.begin(); itemInList != (*list).second.end(); itemInList++) {
			//prices.push_back(atoi(itemInList->getPrice().c_str()));
			Jason price;
			price.addKeyValue("timestamp", atoi(itemInList->getTime().c_str()));
			price.addKeyValue("price", atoi(itemInList->getPrice().c_str()));
			prices.push_back(price);
		}
		jason.addKeyValue("costFluctation", prices);
		jsons.push_back(jason);
	}
	Jason listOfItems;
	listOfItems.addKeyValue("list", jsons);

	//write items to file
	string filename = argv[2];
	ofstream ofs(filename, ios::beg);
	if (ofs.is_open())
	{
		ofs << listOfItems.getJson();
		ofs.close();
	}
	cout << "data successfully transfered from " << argv[1] << " to " << filename;
	return 0;
}