#include <iostream>

#include "stat_reader.h"

stat_reader::stat_reader(size_t num, transport_catalogue* tr_cat)
	: tr_cat_(tr_cat), num_(num) { 
	CallExecution(num);
}

stat_reader::stat_reader(std::string& num, transport_catalogue* tr_cat) 
	: tr_cat_(tr_cat), num_(5) {
	CallExecution(std::stoi(num));
}

void stat_reader::CallExecution(size_t num) {
	buses_.reserve(num_);
	for (size_t i = 0; i < num; ++i) {
		ReadLine();
	}
	for (std::string& bus : buses_) {
		PrintLine(bus);
	}
}

void stat_reader::ReadLine() {
	std::string tmp;
	std::getline(std::cin, tmp);
	size_t sz = tmp.size();

	if (tmp.compare(0, BusKeyWord.size(), BusKeyWord) == 0) {
		buses_.push_back(tmp.substr(BusKeyWord.size(), sz - BusKeyWord.size()));
		return;
	}
	throw std::logic_error("Incorrect string in ReadLine()");
}

void stat_reader::PrintLine(std::string& bus) {
	transport_catalogue::Route tmp = tr_cat_->GetRoute(bus);
	
	std::cout << "Bus " << bus << ": ";
	if (tmp.way_.empty()) {
		std::cout << "not found" << std::endl;
	}
	else {
		std::cout << tmp.way_.size() << " stops on route, "
			<< tmp.unique_stops_ << " unique stops, "
			<< tmp.length_ << " route length" << std::endl;
	}
}
