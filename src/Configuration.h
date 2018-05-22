/*
 * Configuration.h
 *
 *  Created on: Apr 23, 2018
 *      Author: faysal
 */
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include<string>
using namespace std;

class Configuration {

private:
	string inputFormat;
	string networkfile;
	string outputdirectory;


public:
	unsigned int nodeLimit;
	double weightThreshold;
	bool includeSelfLinks = false;
	bool parseAsUndirected = false;
	bool zeroBasedNodeNumbers = false;
	bool rawdir = false;
	bool directed = false;


	string getNetworkFile() {
		return networkfile;
	}
	void setNetworkFile(string filename) {
		networkfile = "/home/faysal/Downloads/infomap_serial_minimal/src/ninetriangles.net";
	}

	void setInputFormat();

	string getInputFormat() {
		return inputFormat;
	}

	string getNetworkFileExtension();

	string getOutputDirectory() {
		return outputdirectory;
	}
	void setOutputDirectory(string directoryname) {
		outputdirectory = directoryname;
	}
};

#endif /* CONFIGURATION_H_ */
