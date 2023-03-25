#include <iostream>

#include "stat_reader.h"

stat_reader::stat_reader(size_t num, transport_catalogue* tr_cat, bool is_test)
	: num_(num), tr_cat_(tr_cat), is_test_(is_test) { 
	CallExecution(num);
}

stat_reader::stat_reader(std::string& num, transport_catalogue* tr_cat, bool is_test)
	: num_(std::stoi(num)), tr_cat_(tr_cat), is_test_(is_test) {
	CallExecution(std::stoi(num));
}

void stat_reader::CallExecution(size_t num) {
	buses_.reserve(num_);
	if (is_test_)
		tr_cat_->outrows_.reserve(num);
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
	if (is_test_) {
		std::stringstream os;
		PrintLineInner(os, bus);
		tr_cat_->outrows_.push_back(os.str());			
	}
	else {
		PrintLineInner(std::cout, bus);
	}
}