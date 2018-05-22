/*
 * FlowNetwork_se.cpp
 *
 *  Created on: May 18, 2018
 *      Author: faysal
 */
#include "FlowNetwork_se.h"
#include <iostream>
#include <cmath>
using namespace std;


void FlowNetwork::calculateFlow(const Network& network, const Configuration& config)
{
	cout << "Calculating global flow... " << std::flush;

	// Prepare data in sequence containers for fast access of individual elements
	unsigned int numNodes = network.numNodes();
	const vector<double>& nodeOutDegree = network.outDegree();
	const vector<double>& sumLinkOutWeight = network.sumLinkOutWeight();
	m_nodeFlow.assign(numNodes, 0.0);
	m_nodeTeleportRates.assign(numNodes, 0.0);

	const LinkMap& linkMap = network.linkMap();
	unsigned int numLinks = network.numLinks();
	m_flowLinks.resize(numLinks);
	double totalLinkWeight = network.totalLinkWeight();
	double sumUndirLinkWeight = 2 * totalLinkWeight - network.totalSelfLinkWeight();
	unsigned int linkIndex = 0;

	for (LinkMap::const_iterator linkIt(linkMap.begin()); linkIt != linkMap.end(); ++linkIt)
	{
		unsigned int linkEnd1 = linkIt->first;
		const std::map<unsigned int, double>& subLinks = linkIt->second;
		for (std::map<unsigned int, double>::const_iterator subIt(subLinks.begin()); subIt != subLinks.end(); ++subIt, ++linkIndex)
		{
			unsigned int linkEnd2 = subIt->first;
			double linkWeight = subIt->second;

			m_nodeFlow[linkEnd1] += linkWeight / sumUndirLinkWeight;
			m_flowLinks[linkIndex] = Link(linkEnd1, linkEnd2, linkWeight);

			if (linkEnd1 != linkEnd2)
				m_nodeFlow[linkEnd2] += linkWeight / sumUndirLinkWeight;
		}
	}

	if (config.rawdir)
	{
		// Treat the link weights as flow (after global normalization) and
		// do one power iteration to set the node flow
		m_nodeFlow.assign(numNodes, 0.0);
		for (LinkVec::iterator linkIt(m_flowLinks.begin()); linkIt != m_flowLinks.end(); ++linkIt)
		{
			Link& link = *linkIt;
			link.flow /= totalLinkWeight;
			m_nodeFlow[link.target] += link.flow;
		}
		cout << "\n  -> Using directed links with raw flow.";
		cout << "\n  -> Total link weight: " << totalLinkWeight << ".";
		cout << std::endl;
		//finalize(network, config, true);
		return;
	}
	if (!config.directed)
	{
		for (unsigned int i = 0; i < numLinks; ++i)
			m_flowLinks[i].flow /= sumUndirLinkWeight;
			//finalize(network, config);
		return;
	}
}
