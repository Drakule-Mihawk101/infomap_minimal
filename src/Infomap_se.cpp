/*
 * Infomap_se.cpp
 *
 *  Created on: Apr 18, 2018
 *      Author: faysal
 */
#include <iostream>
#include "Configuration.h"
#include "Network_se.h"

using namespace std;

Configuration conf;

int run()
{
	try
	{

		Network network;
		network.setConfiuration(conf);
		network.readInputData(conf.getNetworkFile());

	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}


int main(int argc, char* argv[])
{
	cout<<argv[1]<<endl;
	conf.setNetworkFile(argv[1]);
	conf.setOutputDirectory(argv[2]);

	run();

	return 0;
}

