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

	string getNetworkFile() {
		return networkfile;
	}
	void setNetworkFile(string filename) {
		networkfile = filename;
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
