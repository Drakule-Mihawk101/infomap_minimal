/*
 * Configuration.cpp
 *
 *  Created on: Apr 18, 2018
 *      Author: faysal
 */
#include "Configuration.h"
using namespace std;

string Configuration::getNetworkFileExtension()
	{
		string extension="";
		string delim=".";
		if(!networkfile.empty())
		{
			int end=networkfile.find(delim);
			extension= networkfile.substr(end,networkfile.size());
		}
		return extension;
	}

void Configuration::setInputFormat()
{
	string format = getNetworkFileExtension();
	if(format==".net")
	{
		inputFormat="pajek";
	}
}




