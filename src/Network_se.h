/*
 * Network_se.h
 *
 *  Created on: Apr 23, 2018
 *      Author: faysal
 */

#ifndef NETWORK_SE_H_
#define NETWORK_SE_H_

#include "Configuration.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class Network {
private:
	Configuration m_config;
	ifstream input;
	unsigned int m_numNodes;
	vector<std::string> m_nodeNames;
	vector<double> m_nodeWeights;
	double m_sumNodeWeights;
	unsigned int m_indexOffset;

public:
	unsigned int m_numNodesFound;


	void setConfiuration(Configuration& config);

	void readInputData(string filename);

	void parsePajekNetwork(string& filename);

	string skipUntilHeader(ifstream& file);

	string parseVertices(string& file, bool required);
	string parseVertices(string& file, string header, bool required);

	template<typename T>
	inline string stringify(T x)
	{
		std::ostringstream o;
		if (!(o << x))
			throw runtime_error("stringify error...");
		return o.str();
	}

};

#endif /* NETWORK_SE_H_ */
