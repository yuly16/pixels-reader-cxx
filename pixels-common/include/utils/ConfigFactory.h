//
// Created by yuly on 19.04.23.
//

#ifndef DUCKDB_CONFIGFACTORY_H
#define DUCKDB_CONFIGFACTORY_H

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <cstdlib>
#include "exception/InvalidArgumentException.h"
#include <fstream>
#include <sstream>

class ConfigFactory {
public:
	static ConfigFactory & Instance();
	void Print();
private:
	ConfigFactory();
	std::map<std::string, std::string> prop;
};
#endif // DUCKDB_CONFIGFACTORY_H
