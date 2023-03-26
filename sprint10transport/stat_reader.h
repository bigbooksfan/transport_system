#pragma once

#include <vector>
#include <string>
#include <string_view>

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
	std::vector<std::string> querries_;

	const std::string BusKeyWord = "Bus ";
	const std::string StopKeyWord = "Stop ";

	bool is_test_ = false;
public:				// constructors
	stat_reader() = delete;
	explicit stat_reader(size_t num, transport_catalogue* tr_cat, bool is_test = false);
	explicit stat_reader(std::string& num, transport_catalogue* tr_cat, bool is_test = false);

private:
	void CallExecution(size_t num);
	void ReadLine();

	void PrintBus(std::string_view bus);
	template <typename OS>
	void PrintBusInner(OS& os, std::string_view bus);

	void PrintStop(std::string_view stop);
	template <typename OS>
	void PrintStopInner(OS& os, std::string_view stop);
};			// class stat_reader

template <typename OS>
void stat_reader::PrintBusInner(OS& os, std::string_view bus) {
	transport_catalogue::Route tmp = tr_cat_->GetRoute(std::string(bus));
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

template<typename OS>
void stat_reader::PrintStopInner(OS& os, std::string_view stop) {
	std::string tmp = std::string(stop);
	os << "Stop " << stop << ": "; 
	try {
		tr_cat_->FindStop(tmp);
	}
	catch (std::logic_error&) {
		os << "not found" << std::endl;
		return;
	}
	std::set<std::string> buses = tr_cat_->GetBusesOfStop(tmp);
	
	if (buses.empty()) {
		os << "no buses";
	}
	else {
		os << "buses";
		for (const std::string bus : buses) {
			os << " " << bus;
		}
	}
	os << std::endl;
}