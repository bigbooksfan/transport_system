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

	bool is_test_ = false;
public:				// constructors
	stat_reader() = delete;
	explicit stat_reader(size_t num, transport_catalogue* tr_cat, bool is_test = false);
	explicit stat_reader(std::string& num, transport_catalogue* tr_cat, bool is_test = false);

private:
	void CallExecution(size_t num);
	void ReadLine();
	void PrintLine(std::string& bus);
	template <typename OS>
	void PrintLineInner(OS& os, std::string& bus);
};			// class stat_reader

template <typename OS>
void stat_reader::PrintLineInner(OS& os, std::string& bus) {
	transport_catalogue::Route tmp = tr_cat_->GetRoute(bus);
	os << "Bus " << bus << ": ";
	if (tmp.way_.empty()) {
		os << "not found" << std::endl;

	}
	else {
		os << tmp.way_.size() << " stops on route, "
			<< tmp.unique_stops_ << " unique stops, "
			<< tmp.length_ << " route length" << std::endl;
	}
}
