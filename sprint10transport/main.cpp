#include <iostream>
#include <string>

#include "input_reader.h"
#include "stat_reader.h"
#include "tests.h"

//#define TESTMODE

int main() {
	//tests::AllTests();

	std::string n;
	//std::cin >> n;
	std::getline(std::cin, n);
	transport_catalogue Catalogue(n);
	std::getline(std::cin, n);
	stat_reader reader(n, &Catalogue);

	return 0;
}