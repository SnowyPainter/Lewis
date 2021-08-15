#pragma once

//1주기 1족
//2주기 1~2, 13~17족
//3주기 1~2, 13~17족
#include <map>
#include <string>

using namespace std;

namespace electron {
	static map<string, int> Number = {
		{"H", 1},
		{"Li", 3},
		{"Be", 4},
		{"B", 5},
		{"C", 6},
		{"N", 7},
		{"O", 8},
		{"F", 9},
		{"Na", 11},
		{"Ma", 12},
		{"Al", 13},
		{"Si", 14},
		{"P", 15},
		{"S", 16},
		{"Cl", 17},
	};
	static map<int, float> Negativity = {
		{1, 2.2},
		{3, 0.98},
		{4, 1.54},
		{5, 2.04},
		{6, 2.55},
		{7, 3.04},
		{8, 3.44},
		{9, 3.98},
		{11, 0.93},
		{12, 1.31},
		{13, 1.61},
		{14, 1.9},
		{15, 2.19},
		{16, 2.58},
		{17, 3.16}
	};
}