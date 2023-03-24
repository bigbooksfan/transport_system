#pragma once

#include <iostream>
#include <string>

#include "transport_catalogue.h"

/*
10
Stop Tolstopaltsevo: 55.611087, 37.208290
Stop Marushkino: 55.595884, 37.209755
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
Stop Rasskazovka: 55.632761, 37.333324
Stop Biryulyovo Zapadnoye: 55.574371, 37.651700
Stop Biryusinka: 55.581065, 37.648390
Stop Universam: 55.587655, 37.645687
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164
*/

namespace InputReader {

	class input_reader {
	private:				// fields
		const size_t num_;
		transport_catalogue* tr_cat_;

		const std::string StopKeyWord = "Stop";
		const std::string BusKeyWord = "Bus";
		const std::string NameDelimeter = ":";
		const std::string StopCoordDelimeter = ",";

	public:					// constructors
		input_reader() = delete;
		explicit input_reader(size_t num, transport_catalogue* tr_cat);

	public:					// methods

	private:				// methods
		void ReadLine();
		void ReadStop(std::string& str);
		void ReadBus(std::string& str);
		static inline void TrimSpaces(std::string& s);
	};

}			// namespace InputReader