#include "input_reader.h"

#include <stdexcept>

namespace InputReader {
	input_reader::input_reader(size_t num, transport_catalogue* tr_cat) 
		: num_(num), tr_cat_(tr_cat) {
		for (size_t i = 0; i < num; ++i) {
			ReadLine();
		}
	}

	void input_reader::ReadLine() {
		std::string tmp;
		std::getline(std::cin, tmp);
		//TrimSpaces(tmp);		// do i need it?

		if (tmp.compare(0, StopKeyWord.size(), StopKeyWord) == 0) {
			ReadStop(tmp);
			return;
		}
		if (tmp.compare(0, BusKeyWord.size(), BusKeyWord) == 0) {
			ReadBus(tmp);
			return;
		}
		throw std::logic_error("No BUS or STOP specifier in ReadLine()");		
	}

	void input_reader::ReadBus(std::string& str) {
		size_t EndOfName = str.find(NameDelimeter, 3);

		std::string name = str.substr(4, EndOfName - 4);
		std::string raw_description = str.substr(EndOfName + 2);

		tr_cat_->AddBus(name, raw_description);
	}

	void input_reader::ReadStop(std::string& str) {		
		size_t EndOfName = str.find(NameDelimeter, 4);
		size_t CoordDelim = str.find(StopCoordDelimeter, EndOfName);

		std::string name = str.substr(5, EndOfName - 5);
		double lat = std::stod(str.substr(EndOfName + 2, CoordDelim));
		double lon = std::stod(str.substr(CoordDelim + 1));

		tr_cat_->AddStop(name, lat, lon);
	}

	void input_reader::TrimSpaces(std::string& s) {
		// left trim
		s.erase(
			s.begin(), std::find_if(s.begin(), s.end(),
				[](unsigned char ch) {
					return !std::isspace(ch);
				}
		));

		// right trim
		s.erase(
			std::find_if(s.rbegin(), s.rend(),
				[](unsigned char ch) {
					return !std::isspace(ch);
				}
			).base(), s.end()
		);
	}
}			// namespace InputReader