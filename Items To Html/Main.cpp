#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

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
	inline const string getName() const { return name; }
	inline const string getPrice() const { return to_string(price); }
	inline const string getQty() const { return to_string(qty); }
	inline const string getTime() const { return to_string(time); }
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
	string filename = argv[2];
	ofstream ofs(filename, ios::beg);
	if (ofs.is_open())
	{
		for (auto item : iList)
			ofs << item.getLi() << '\n';
		ofs.close();
	}
	cout << "data successfully transfered from " << argv[1] << " to " << filename;
	return 0;
}