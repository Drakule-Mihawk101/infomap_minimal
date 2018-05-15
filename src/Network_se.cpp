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
#include <string>

using namespace std;

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

void Network::parsePajekNetwork(string& filename) {

	ifstream input(filename.c_str());
	// Parse the vertices and return the line after
	string line = parseVertices(filename, false);

	// Read links in format "from to weight", for example "1 3 2" (all integers) and each undirected link only ones (weight is optional).
	while (!getline(input, line).fail()) {
		if (line.length() == 0)
			continue;
		//parseLink(line, n1, n2, weight);

		//addLink(n1, n2, weight);
	}

}

string Network::parseVertices(string& file, bool required) {
	string line;

	ifstream input(file.c_str());

	// First skip lines until header
	while (!getline(input, line).fail()) {
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

string Network::parseVertices(string& file, string header, bool required) {
	ifstream input(file.c_str());
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
	while (!std::getline(input, line).fail()) {
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
			throw runtime_error("The node id from line doesn't follow a consequitive order");

		}

		m_sumNodeWeights += weight;
		m_nodeWeights[nodeIndex] = weight;
		m_nodeNames[nodeIndex] = name;
		++numNodesParsed;
	}

	if (line[0] == '*' && numNodesParsed == 0) {
		// Short pajek version (no nodes defined), set node number as name
		for (unsigned int i = 0; i < m_numNodes; ++i) {
			m_nodeWeights[i] = 1.0;
			m_nodeNames[i] = stringify(i + 1);
		}
		m_sumNodeWeights = m_numNodes * 1.0;
	}

	if (didEarlyBreak) {
		//line = skipUntilHeader(file);
	}

	return line;
}

string Network::skipUntilHeader(ifstream& file)
{
	std::string line;

	// First skip lines until header
	while(!std::getline(file, line).fail())
	{
		if (line.length() == 0 || line[0] == '#')
			continue;
		if (line[0] == '*')
			break;
	}

	return line;
}
