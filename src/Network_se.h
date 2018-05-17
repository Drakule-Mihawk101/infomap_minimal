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
	unsigned int m_indexOffset=1;

public:
	typedef std::map<unsigned int, std::map<unsigned int, double> >	LinkMap;
	LinkMap m_links;
	unsigned int m_numNodesFound;
	unsigned int m_numAggregatedLinks;
	// Helpers
	istringstream m_extractor;
	double m_totalSelfLinkWeight; // On whole network
	unsigned int m_numLinksIgnoredByWeightThreshold;
	double m_totalLinkWeightIgnored;
	bool m_addSelfLinks;
	unsigned int m_numAdditionalLinks;
	unsigned int m_sumAdditionalLinkWeight;
	// Checkers
	unsigned int m_maxNodeIndex; // On links
	unsigned int m_minNodeIndex; // On links
	unsigned int m_numSelfLinksFound;
	unsigned int m_numSelfLinks;
	unsigned int m_numLinksFound;
	unsigned int m_numLinks; //this is for total number of edges in the graph
	unsigned int m_totalLinkWeight; //summation of the weights of every link
	set<unsigned int> m_nodes;

	void setConfiuration(Configuration& config);

	void readInputData(string filename);

	void parsePajekNetwork(string filename);

	string skipUntilHeader(ifstream& file);

	string parseVertices(ifstream& file, bool required);
	string parseVertices(ifstream& file, string header, bool required);
	void parseLink(const std::string& line, unsigned int& n1, unsigned int& n2, double& weight);
	bool addLink(unsigned int n1, unsigned int n2, double weight);
	bool insertLink(unsigned int n1, unsigned int n2, double weight);
	bool insertNode(unsigned int nodeIndex);



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
