/*
 * Network_se.cpp
 *
 *  Created on: Apr 18, 2018
 *      Author: faysal
 */

#include "Network_se.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;


Network::Network()
{
	m_numNodes=0;
	m_sumNodeWeights=0.0;
	m_indexOffset=1;
	m_numNodesFound=0;
	m_numAggregatedLinks=0;
	m_totalSelfLinkWeight=0.0; // On whole network
	m_numLinksIgnoredByWeightThreshold=0;
	m_totalLinkWeightIgnored=0.0;
	m_addSelfLinks=0;
	m_numAdditionalLinks=0;
	m_sumAdditionalLinkWeight=0;
	m_maxNodeIndex=0; // On links
	m_minNodeIndex=0; // On links
	m_numSelfLinksFound=0;
	m_numSelfLinks=0;
	m_numLinksFound=0;
	m_numLinks=0; //this is for total number of edges in the graph
	m_totalLinkWeight = 0; //summation of the weights of every link
	m_numDanglingNodes=0;
}
void Network::setConfiuration(Configuration& config) {
	m_config = config;
	m_config.setInputFormat();
}

void Network::readInputData(string filename) {
	if (!filename.empty())
		//string format = m_config.getInputFormat();
		if (m_config.getInputFormat() == "pajek")
			parsePajekNetwork(filename);
}

void Network::finalizeAndCheckNetwork() {
	unsigned int zeroMinusOne = 0;
		--zeroMinusOne;
		if (m_maxNodeIndex == zeroMinusOne)
			throw runtime_error("Integer overflow, be sure to use zero-based node numbering if the node numbers start from zero.");
		if (m_maxNodeIndex >= m_numNodes)
			throw runtime_error("At least one link is defined with node numbers that exceeds the number of nodes.");
		if (m_minNodeIndex == 1 && m_config.zeroBasedNodeNumbers)
			cout << "(Warning: minimum link index is one, check that you don't use zero based numbering if it's not true.) ";

		initNodeDegrees();
}

void Network::initNodeDegrees()
{
	m_outDegree.assign(m_numNodes, 0.0);
	m_sumLinkOutWeight.assign(m_numNodes, 0.0);
	m_numDanglingNodes = m_numNodes;
	for (LinkMap::iterator linkIt(m_links.begin()); linkIt != m_links.end(); ++linkIt)
	{
		unsigned int n1 = linkIt->first;
		std::map<unsigned int, double>& subLinks = linkIt->second;
		for (std::map<unsigned int, double>::iterator subIt(subLinks.begin()); subIt != subLinks.end(); ++subIt)
		{
			unsigned int n2 = subIt->first;
			double linkWeight = subIt->second;
			if (m_outDegree[n1] == 0)
				--m_numDanglingNodes;
			++m_outDegree[n1];
			m_sumLinkOutWeight[n1] += linkWeight;
			if (n1 != n2 && m_config.parseAsUndirected)
			{
				if (m_outDegree[n2] == 0)
					--m_numDanglingNodes;
				++m_outDegree[n2];
				m_sumLinkOutWeight[n2] += linkWeight;
			}
		}
	}
}

void Network::initNodeNames()
{
	unsigned int indexOffset = m_config.zeroBasedNodeNumbers? 0 : 1;
		if (m_nodeNames.size() < numNodes())
		{
			// Define nodes
			unsigned int oldSize = m_nodeNames.size();
			m_nodeNames.resize(numNodes());
			for (unsigned int i = oldSize; i < numNodes(); ++i)
				m_nodeNames[i] =  stringify(i + indexOffset);

		}
}

void Network::parsePajekNetwork(string filename) {

	ifstream input(filename.c_str());
	// Parse the vertices and return the line after
	string line = parseVertices(input, false);

	// Read links in format "from to weight", for example "1 3 2" (all integers) and each undirected link only ones (weight is optional).
	while (!getline(input, line).fail()) {
		if (line.length() == 0)
			continue;
		unsigned int n1, n2;
		double weight;
		parseLink(line, n1, n2, weight);

		addLink(n1, n2, weight);
	}

	finalizeAndCheckNetwork();
}

bool Network::addLink(unsigned int n1, unsigned int n2, double weight) {
	++m_numLinksFound;

	if (m_config.nodeLimit > 0
			&& (n1 >= m_config.nodeLimit || n2 >= m_config.nodeLimit))
		return false;

	if (weight < m_config.weightThreshold) {
		++m_numLinksIgnoredByWeightThreshold;
		m_totalLinkWeightIgnored += weight;
		return false;
	}

	if (n2 == n1) {
		++m_numSelfLinksFound;
		if (!m_config.includeSelfLinks)
			return false;
		++m_numSelfLinks;
		m_totalSelfLinkWeight += weight;
	} else if (m_config.parseAsUndirected && n2 < n1) // minimize number of links
		std::swap(n1, n2);

	m_maxNodeIndex = std::max(m_maxNodeIndex, std::max(n1, n2));
	m_minNodeIndex = std::min(m_minNodeIndex, std::min(n1, n2));

	insertLink(n1, n2, weight);

	return true;
}

bool Network::insertLink(unsigned int n1, unsigned int n2, double weight) {
	++m_numLinks;
	m_totalLinkWeight += weight;
	insertNode(n1);
	insertNode(n2);

	// Aggregate link weights if they are definied more than once
	LinkMap::iterator firstIt = m_links.lower_bound(n1);//faysal: maybe i will change the m_links to some other data structure like vector
	if (firstIt != m_links.end() && firstIt->first == n1) // First linkEnd already exists, check second linkEnd
			{
		std::pair<std::map<unsigned int, double>::iterator, bool> ret2 =
				firstIt->second.insert(std::make_pair(n2, weight));
		if (!ret2.second) {
			ret2.first->second += weight;
			++m_numAggregatedLinks;
			--m_numLinks;
			return false;
		}
	} else {
		m_links.insert(firstIt,
				std::make_pair(n1, std::map<unsigned int, double>()))->second.insert(
				std::make_pair(n2, weight));
	}

	return true;
}

bool Network::insertNode(unsigned int nodeIndex) {
	return m_nodes.insert(nodeIndex).second;
}

void Network::parseLink(const std::string& line, unsigned int& n1,
		unsigned int& n2, double& weight) {
	m_extractor.clear();
	cout << "line :" << line << endl;
	m_extractor.str(line);
	if (!(m_extractor >> n1 >> n2))
		throw runtime_error("Can't parse link data from line");
	(m_extractor >> weight) || (weight = 1.0);
	n1 -= m_indexOffset;
	n2 -= m_indexOffset;
}

string Network::parseVertices(ifstream& file, bool required) {
	string line;
	// First skip lines until header
	while (!getline(file, line).fail()) {
		if (line.length() == 0 || line[0] == '#')
			continue;
		if (line[0] == '*')
			break;
	}
	if (line.length() == 0 || line[0] != '*') {
		throw runtime_error("No matching header for vertices found.");
	}
	//throw FileFormatError("No matching header for vertices found.");
	//return line;
	return parseVertices(file, line, required);
}

string Network::parseVertices(ifstream& file, string header, bool required) {

	istringstream ss;
	string buf;
	ss.str(header);
	ss >> buf;
	if (buf == "*Vertices" || buf == "*vertices" || buf == "*VERTICES") {
		if (!(ss >> m_numNodesFound))
			throw runtime_error(
					"Can't parse an integer after as the number of nodes.");
	} else {
		if (!required)
			return header;
		throw runtime_error(
				"The header doesn't match *Vertices (case insensitive).");
	}

	if (m_numNodesFound == 0)
		throw runtime_error("The number of vertices cannot be zero.");

	bool checkNodeLimit = m_config.nodeLimit > 0;
	m_numNodes = checkNodeLimit ? m_config.nodeLimit : m_numNodesFound;

	m_nodeNames.resize(m_numNodes);
	m_nodeWeights.assign(m_numNodes, 1.0);
	m_sumNodeWeights = 0.0;

	string line;
	unsigned int numNodesParsed = 0;
	bool didEarlyBreak = false;

	// Read node names and optional weight, assuming id 1, 2, 3, ... (or 0, 1, 2, ... if zero-based node numbering)
	while (!std::getline(file, line).fail()) {
		if (line.length() == 0 || line[0] == '#')
			continue;

		if (line[0] == '*')
			break;

		if (m_config.nodeLimit > 0 && numNodesParsed == m_config.nodeLimit) {
			didEarlyBreak = true;
			break;
		}

		// parseVertice(line, id, name, weight);
		ss.clear();
		ss.str(line);

		unsigned int id = 0;
		if (!(ss >> id))
			throw runtime_error("Can't parse node id from line");

		unsigned int nameStart = line.find_first_of("\"");
		unsigned int nameEnd = line.find_last_of("\"");
		std::string name("");
		if (nameStart < nameEnd) {
			name = std::string(line.begin() + nameStart + 1,
					line.begin() + nameEnd);
			line = line.substr(nameEnd + 1);
			ss.clear();
			ss.str(line);
		} else {
			if (!(ss >> name))
				throw runtime_error("Can't parse node id from line");
		}
		double weight = 1.0;
		if ((ss >> weight)) {
			// TODO: Check valid weight here?
		}
		unsigned int nodeIndex = static_cast<unsigned int>(id - m_indexOffset);
		if (nodeIndex != numNodesParsed) {
//			throw BadConversionError(
//					io::Str() << "The node id from line '" << line
//							<< "' doesn't follow a consequitive order"
//							<< ((m_indexOffset == 1 && id == 0) ?
//									".\nBe sure to use zero-based node numbering if the node numbers start from zero." :
//									"."));
			throw runtime_error(
					"The node id from line doesn't follow a consequitive order");
		}

		m_sumNodeWeights += weight;
		m_nodeWeights[nodeIndex] = weight;
		m_nodeNames[nodeIndex] = name;
		++numNodesParsed;
	}

	cout << "number of nodes parsed" << numNodesParsed << endl;

	if (line[0] == '*' && numNodesParsed == 0) {
		// Short pajek version (no nodes defined), set node number as name
		for (unsigned int i = 0; i < m_numNodes; ++i) {
			m_nodeWeights[i] = 1.0;
			m_nodeNames[i] = stringify(i + 1);
		}
		m_sumNodeWeights = m_numNodes * 1.0;
	}

	if (didEarlyBreak) {
		line = skipUntilHeader(input);
	}

	return line;
}

string Network::skipUntilHeader(ifstream& file) {
	std::string line;

	// First skip lines until header
	while (!std::getline(file, line).fail()) {
		if (line.length() == 0 || line[0] == '#')
			continue;
		if (line[0] == '*')
			break;
	}

	return line;
}
