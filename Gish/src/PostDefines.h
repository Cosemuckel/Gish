#pragma once

//This file contains functions and variables that are before Gish.hpp, but use elements of Gish.hpp.

int operator+ (Value&& value) {
	GishClient::valueList.push_back(value);
	return GishClient::valueList.size() - 1;
}

std::vector<Token>* Token::defaultTokenList = &GishClient::tokenList;
std::vector<Value>* Token::defaultValueList = &GishClient::valueList;