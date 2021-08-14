#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
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
			if (isupper(raw[i + 1]) || i+1 >= raw.length()) {
				atoms.push_back(std::make_tuple(atom, 1));
			}
			else {
				int c = raw[i + 1] - '0';
				atoms.push_back(std::make_tuple(atom, c));
				i++;
			}
		}
		return atoms;
	}
};