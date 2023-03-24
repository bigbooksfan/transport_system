#pragma once

#include <vector>
#include <string>

#include "transport_catalogue.h"

/*
in:
3
Bus 256
Bus 750
Bus 751

out:
Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length
Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length
Bus 751: not found
*/

class stat_reader {
private:			// fields
	const size_t num_;
	transport_catalogue* tr_cat_;
	std::vector<std::string> buses_;

	const std::string BusKeyWord = "Bus ";

public:				// constructors
	stat_reader() = delete;
	explicit stat_reader(size_t num, transport_catalogue* tr_cat);
	explicit stat_reader(std::string& num, transport_catalogue* tr_cat);

private:
	void CallExecution(size_t num);
	void ReadLine();
	void PrintLine(std::string& bus);
};