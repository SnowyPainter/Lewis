#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <map>
//chemical formula parser


class chemical_formula {
private:
	static bool isupper(char c) {
		return (c >= 65 && c <= 90);
	}
	static bool islower(char c) {
		return (c >= 97 && c <= 112);
	}
public:
	static std::vector<std::tuple<std::string, int>> Parse(std::string raw) {
		std::vector<std::tuple<std::string, int>> atoms;
		for (int i = 0; i < raw.length(); i++) {
			std::string atom;
			int n = 0;
			do {
				atom.push_back(raw[i + n]);
				n++;
			} while (isupper(raw[i]) && islower(i + n));
			i += n - 1;
			std::string number;
			int m = 0;
			if (i + 1 >= raw.length() || isupper(raw[i + 1])) {
				atoms.push_back(std::make_tuple(atom, 1));
				continue;
			}
			while (i + m < raw.length() && !isupper(raw[i + m + 1])) {
				number.push_back(raw[i + m + 1]);
				m++;
			}
			
			if (number.length() > 0) {
				atoms.push_back(std::make_tuple(atom, std::stoi(number)));
				i += m;
			}
		}
		return atoms;
	}
	static std::string Frame(std::string raw) {
		std::map<char, int> symbolCount;

		for (const auto& c : raw) {
			symbolCount[c]++;
		}
		
		std::string framed = "";
		for (auto counts = symbolCount.begin(); counts != symbolCount.end();counts++) {
			framed.push_back(counts->first);
			if(counts->second > 1)
				framed += std::to_string(counts->second);
		}
		return framed;
	}
};