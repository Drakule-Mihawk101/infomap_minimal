#ifndef FLOWNETWORK_SE_H_
#define FLOWNETWORK_SE_H_

#include <string>
#include <map>
#include <vector>
#include "Network_se.h"

using namespace std;

class FlowNetwork
{
public:
	struct Link
	{
		Link(unsigned int sourceIndex = 0, unsigned int targetIndex = 0, double weight = 0.0) :
			source(sourceIndex),
			target(targetIndex),
			weight(weight),
			flow(weight)
		{}
		unsigned int source;
		unsigned int target;
		double weight;
		double flow;
	};

	typedef Network::LinkMap									LinkMap;
	typedef vector<Link>										LinkVec;

	FlowNetwork() {}
	virtual ~FlowNetwork() {}

	virtual void calculateFlow(const Network& network, const Configuration& config);

	const vector<double>& getNodeFlow() const { return m_nodeFlow; }
	const vector<double>& getNodeTeleportRates() const { return m_nodeTeleportRates; }
	const LinkVec& getFlowLinks() const { return m_flowLinks; }

protected:

	void finalize(const Network& network, const Configuration& config, bool normalizeNodeFlow = false);

	vector<double> m_nodeFlow;
	vector<double> m_nodeTeleportRates;
	LinkVec m_flowLinks;

};


#endif /* FLOWNETWORK_H_ */
