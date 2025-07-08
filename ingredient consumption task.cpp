#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>


struct EntryData {
	std::string timestamp;
	int flour_dkg;
	int groat_g;
	int milk_ml;
	int egg;
};

void roundToFullHour(std::string& timestamp) {
	std::string MinutesSeconds = "00:00";
	timestamp.replace(14, 5, MinutesSeconds);
}

int main() {
	std::vector<EntryData> data = {
		{"2023-04-11 22:05:00-03:00", 130, 180, 2500, 69},
		{"2023-04-11 23:37:00-03:00", 150, 20, 2350, 38},
		{"2023-04-11 23:04:00-03:00", 120, 170, 2750, 95},
		{"2023-04-12 00:36:00-03:00", 110, 50, 2150, 61},
		{"2023-04-11 22:24:00-03:00", 180, 90, 2900, 70},
		{"2023-04-12 00:29:00-03:00", 100, 160, 2400, 99},
		{"2023-04-12 22:09:00-03:00", 160, 70, 2600, 97},
		{"2023-04-11 22:41:00-03:00", 140, 100, 2250, 37},
		{"2023-04-13 00:04:00-03:00", 170, 190, 2100, 88},
		{"2023-04-13 00:40:00-03:00", 190, 30, 2050, 64}

	};

	std::map<std::string, std::vector<EntryData>> grouped;

	for (int i = 0; i < data.size(); i++)
		roundToFullHour(data[i].timestamp);

	for (const auto& entry : data) 
		grouped[entry.timestamp].push_back(entry);
	
	std::cout << "TIMESTAMP                   FLOUR(kg)  GROAT(kg)  MILK(l)  EGG(pcs)\n";

	for (const auto& [time, entries] : grouped) {
		int total_flour = 0;
		int total_groat = 0;
		int total_milk = 0;
		int total_egg = 0;

		for (const auto& e : entries) {
			total_flour += e.flour_dkg;
			total_groat += e.groat_g;
			total_milk += e.milk_ml;
			total_egg += e.egg;
		}

		std::cout << time << "  ";
		std::cout << std::fixed;
		std::cout.precision(2);
		std::cout << total_flour / 100.0 << "         ";
		std::cout << total_groat / 1000.0 << "       ";
		std::cout << total_milk / 1000.0 << "      ";
		std::cout << total_egg << "\n";
	}

}