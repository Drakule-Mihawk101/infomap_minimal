/*
 * Config_se.h
 *
 *  Created on: Apr 18, 2018
 *      Author: faysal
 */

#ifndef CONFIG_SE_H_
#define CONFIG_SE_H_

#include <vector>
#include "Date.h"

using namespace std;

struct Config_se
{
	Config_se()
	:	parsedArgs(""),
		networkFile(""),
	 	inputFormat(""),
	 	withMemory(false),
		bipartite(false),
		skipAdjustBipartiteFlow(false),
		multiplexAddMissingNodes(false),
		hardPartitions(false),
	 	nonBacktracking(false),
	 	parseWithoutIOStreams(false),
		zeroBasedNodeNumbers(false),
		includeSelfLinks(false),
		ignoreEdgeWeights(false),
		completeDanglingMemoryNodes(false),
		nodeLimit(0),
		weightThreshold(0),
		preClusterMultiplex(false),
	 	clusterDataFile(""),
	 	noInfomap(false),
	 	twoLevel(false),
		directed(false),
		undirdir(false),
		outdirdir(false),
		rawdir(false),
		recordedTeleportation(false),
		teleportToNodes(false),
		teleportationProbability(0.15),
		selfTeleportationProbability(-1),
		markovTime(1.0),
		variableMarkovTime(false),
		preferredNumberOfModules(0),
		multiplexRelaxRate(-1),
		multiplexJSRelaxRate(-1),
		multiplexJSRelaxLimit(-1),
		multiplexRelaxLimit(-1),
		seedToRandomNumberGenerator(123),
		numTrials(1),
		minimumCodelengthImprovement(1.0e-10),
		minimumSingleNodeCodelengthImprovement(1.0e-16),
		randomizeCoreLoopLimit(true),
		coreLoopLimit(10),
		levelAggregationLimit(0),
		tuneIterationLimit(0),
		minimumRelativeTuneIterationImprovement(1.0e-5),
		fastCoarseTunePartition(true),
		alternateCoarseTuneLevel(false),
		coarseTuneLevel(1),
		fastHierarchicalSolution(0),
		fastFirstIteration(false),
		lowMemoryPriority(0),
		innerParallelization(false),
		resetConfigBeforeRecursion(false),
		outDirectory("."),
		outName(""),
		originallyUndirected(false),
		printTree(false),
		printFlowTree(false),
		printMap(false),
		printClu(false),
		printNodeRanks(false),
		printFlowNetwork(false),
		printPajekNetwork(false),
		printStateNetwork(false),
		printBinaryTree(false),
		printBinaryFlowTree(false),
		printExpanded(false),
		noFileOutput(false),
		verbosity(0),
		verboseNumberPrecision(6),
		silent(false),
		benchmark(false),
		maxNodeIndexVisible(0),
		showBiNodes(false),
		minBipartiteNodeIndex(0)
		//version(INFOMAP_VERSION)
	{
	}


	bool isUndirected() const { return !directed && !undirdir && !outdirdir && !rawdir; }
	void setUndirected() { directed = undirdir = outdirdir = rawdir = false; }

		bool isUndirectedFlow() const { return !directed && !outdirdir && !rawdir; } // isUndirected() || undirdir

		bool printAsUndirected() const { return originallyUndirected; }

		bool parseAsUndirected() const { return originallyUndirected; }

		bool useTeleportation() const { return 	directed; }

		bool is3gram() const { return inputFormat == "3gram"; }
		bool isMultiplexNetwork() const { return inputFormat == "multilayer" || inputFormat == "multiplex" || additionalInput.size() > 0; }
		bool isStateNetwork() const { return inputFormat == "states"; }
		bool isBipartite() const { return inputFormat == "bipartite"; }

		bool isMemoryInput() const { return isStateNetwork() || is3gram() || isMultiplexNetwork(); }

		bool isMemoryNetwork() const { return withMemory || nonBacktracking || isMemoryInput(); }

		bool isSimulatedMemoryNetwork() const { return (withMemory || nonBacktracking) && !isMemoryInput(); }

		bool haveModularResultOutput() const
			{
				return printTree ||
						printFlowTree ||
						printMap ||
						printClu ||
						printBinaryTree ||
						printBinaryFlowTree;
			}

		void adaptDefaults()
		{
			if (!haveModularResultOutput())
				printTree = true;

			originallyUndirected = isUndirected();
			if (isMemoryNetwork())
			{
				if (isMultiplexNetwork())
				{
					// Include self-links in multiplex networks as layer and node numbers are unrelated
					includeSelfLinks = true;
					if (!isUndirected())
					{
						// teleportToNodes = true;
						recordedTeleportation = false;
					}
				}
				else
				{
					// teleportToNodes = true;
					recordedTeleportation = false;
					if (isUndirected())
						directed = true;
				}
				if (is3gram()) {
					// Teleport to start of physical chains
					teleportToNodes = true;
				}
			}
			if (isBipartite())
			{
				bipartite = true;
			}
		}

	// Input
		std::string parsedArgs;
		std::string networkFile;
		std::vector<string> additionalInput;
		std::string inputFormat; // 'pajek', 'link-list', '3gram' or 'multiplex'
		bool withMemory;
		bool bipartite;
		bool skipAdjustBipartiteFlow;
		bool multiplexAddMissingNodes;
		bool hardPartitions;
		bool nonBacktracking;
		bool parseWithoutIOStreams;
		bool zeroBasedNodeNumbers;
		bool includeSelfLinks;
		bool ignoreEdgeWeights;
		bool completeDanglingMemoryNodes;
		unsigned int nodeLimit;
		double weightThreshold;
		bool preClusterMultiplex;
		string clusterDataFile;
		bool noInfomap;

		// Core algorithm
		bool twoLevel;
		bool directed;
		bool undirdir;
		bool outdirdir;
		bool rawdir;
		bool recordedTeleportation;
		bool teleportToNodes;
		double teleportationProbability;
		double selfTeleportationProbability;
		double markovTime;
		bool variableMarkovTime;
		unsigned int preferredNumberOfModules;
		double multiplexRelaxRate;
		double multiplexJSRelaxRate;
		double multiplexJSRelaxLimit;
		int multiplexRelaxLimit;
		unsigned long seedToRandomNumberGenerator;

		// Performance and accuracy
		unsigned int numTrials;
		double minimumCodelengthImprovement;
		double minimumSingleNodeCodelengthImprovement;
		bool randomizeCoreLoopLimit;
		unsigned int coreLoopLimit;
		unsigned int levelAggregationLimit;
		unsigned int tuneIterationLimit; // num iterations of fine-tune/coarse-tune in two-level partition)
		double minimumRelativeTuneIterationImprovement;
		bool fastCoarseTunePartition;
		bool alternateCoarseTuneLevel;
		unsigned int coarseTuneLevel;
		unsigned int fastHierarchicalSolution;
		bool fastFirstIteration;
		unsigned int lowMemoryPriority; // Prioritize memory efficient algorithms before fast if > 0
		bool innerParallelization;
		bool resetConfigBeforeRecursion; // If true, flags only affect building up super modules.

		// Output
		std::string outDirectory;
		std::string outName;
		bool originallyUndirected;
		bool printTree;
		bool printFlowTree;
		bool printMap;
		bool printClu;
		bool printNodeRanks;
		bool printFlowNetwork;
		bool printPajekNetwork;
		bool printStateNetwork;
		bool printBinaryTree;
		bool printBinaryFlowTree; // tree including horizontal links (hierarchical network)
		bool printExpanded; // Print the expanded network of memory nodes if possible
		bool noFileOutput;
		unsigned int verbosity;
		unsigned int verboseNumberPrecision;
		bool silent;
		bool benchmark;

		unsigned int maxNodeIndexVisible;
		bool showBiNodes;
		unsigned int minBipartiteNodeIndex;

		// Other
		Date startDate;
		std::string version;
	};


#endif /* CONFIG_SE_H_ */
