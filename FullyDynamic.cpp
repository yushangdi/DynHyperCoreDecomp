/*
Our fully dynamic algorithm.
The code includes various functional blocks. Uncomment respective parts to examine different aspects of the execution.

[To compile]
g++ -std=c++11 -O3 FullyDynamic.cpp GraphScheduler.cpp Hypergraph.cpp HypergraphCoreDecomp.cpp -o FullyDynamic -lpsapi

[To run]
FullyDynamic epsilon lambda alpha filename

[Format of input]
The file should contain an update in each line.
Hyperedge insertion: + [node IDs] timestamp
Hyperedge deletion: - [node IDs]
So be careful that the last number of a line beginning with "+" is not an endpoint of the inserted hyperedge.
The timestamps are actually useless (for our purpose). You will need them only when you want to trace the insertion time of hyperedges.
When deleting a hyperedge, make sure that exactly the same sequence appeared in an insertion update before.
Therefore, it is recommended that in each line, the node IDs are sorted in some particular order, e.g., increasing order.

[Remark]
This program is developed on Windows. "-lpsapi" is related to reporting memory usage. You may have to remove it and "OutputMemory.cpp" (which includes a function outputMemory()) and reimplement this part when you want to compile and run the code on a different operating system.
*/

#include <cstdio>
#include <cmath>
#include <ctime>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <cstring>
#include <sstream>
#include <iomanip>
#include "Hypergraph.hpp"
#include "GraphScheduler.hpp"
#include "HypergraphCoreDecomp.hpp"
// #include "OutputMemory.cpp"
#include "get_time.h"
using namespace std;

#define ROUND_NUM 1

class FullyDynamic {
public:
	FullyDynamic(double epsilon, double lambda, double alpha, char fileName[], string outFileName, bool stat, unsigned batch_size):
		epsilon(epsilon), lambda(lambda), alpha(alpha), delta(delta), scheduler(fileName), outFileName(outFileName), stat(stat), batch_size(batch_size) {
		initialize();
		if(!stat)outputFileTiming.open(outFileName + "_timing.out");
	}

	void output_answer(int cnt) {
		cout << "writing core stats to output file" << endl;
		cout << outFileName + "_" + to_string(cnt) << endl;
		ofstream outputFile;
		outputFile.open(outFileName + "_" + to_string(cnt) + ".out");
		for (int i = 0; i <= scheduler.maxNodeId; ++i){
			if(getApproxCoreVal(i) != 0) outputFile << i << " " << getApproxCoreVal(i) << endl;
		}
		outputFile.close();
		cout << "finish writing" << endl;
	}

	void output_timing(int cnt, double tt) {
		outputFileTiming << cnt << " " << tt << std::endl;
	}


	void run() {
		// FILE *ofpVal = fopen("StatFullyDynamicCoreValue.txt", "w");
		// FILE *ofpTime = fopen("StatFullyDynamicTime.txt", "w");
		// FILE *ofpDetail = fopen("StatFullyDynamicDetail.txt", "w");
		int cnt = 0;
		gbbs::timer t; t.start();
		// time_t t0 = clock(), totalTime = 0;
		int lastUpdTimestamp;
		while (scheduler.hasNext()) {
			EdgeUpdate edgeUpdate = scheduler.nextUpdate();
			if (edgeUpdate.updType == INS)
				// h.insertEdge(edgeUpdate.e);
				insertEdge(edgeUpdate.e);
			else
				// h.deleteEdge(edgeUpdate.e);
				deleteEdge(edgeUpdate.e);
			++cnt;

			if (cnt % batch_size == 0 || !scheduler.hasNext() ) {
				if(stat){
					output_answer(cnt);
				}else{
					output_timing(cnt, t.get_next());
				}

				// cerr << cnt << "...\t";
				// Block: print all b[t][u] and c[u] every 100000 updates
				// time_t t1 = clock();
				// fprintf(ofpTime, "%ld\n", (long)t1 - t0);
				// fprintf(ofpVal, "%d\n", cnt);


				///////////////// compute error start
				/*
				// Block: observe gracefully degrading ratios (3-dimensional figures)
				// Run the static exact algorithm
				HypergraphCoreDecomp hcd(h);
				hcd.solve();
				unordered_map<int, int> coreCount, coreCountApprox;
				// how many numbers of count
				// e.g. how many 1's in exact ...
				for (auto& p: hcd.c) {
					const Node u = p.first;
					if (hcd.c[u] > 0) {
						if (!coreCount.count(hcd.c[u]))
							coreCount[hcd.c[u]] = 1;
						else
							++coreCount[hcd.c[u]];
						if (!coreCountApprox.count(b[tau][u]))
							coreCountApprox[b[tau][u]] = 1;
						else
							++coreCountApprox[b[tau][u]];
					}
				}
				for (auto& p: coreCount)
					fprintf(ofpVal, "%d %d\n", p.first, p.second);
				fprintf(ofpVal, "-1\n");
				for (auto& p: coreCountApprox)
					fprintf(ofpVal, "%d %d\n", p.first, p.second);
				fprintf(ofpVal, "-1\n");

				vector<double> maxErr(tau + 1, 0), avgErr(tau + 1, 0);
				int cntNonZero = 0;
				for (auto& p: hcd.c) {
					const Node u = p.first;
					if (hcd.c[u] > 0) {
						++cntNonZero;
						for (int t = 1; t <= tau; ++t) {
							double err = max(((double)hcd.c[u]) / b[t][u], ((double)b[t][u]) / hcd.c[u]);
							if (maxErr[t] < err)
								maxErr[t] = err;
							avgErr[t] += err;
						}
					}
				}
				for (int t = 1; t <= tau; ++t)
					avgErr[t] /= cntNonZero;
				for (int t = 1; t <= tau; ++t)
					fprintf(ofpDetail, "%.9f%c", maxErr[t], t == tau ? '\n' : ' ');
				for (int t = 1; t <= tau; ++t)
					fprintf(ofpDetail, "%.9f%c", avgErr[t], t == tau ? '\n' : ' ');
				*/
				///////////////// compute error end
				// t0 = clock();
			}

			/*
			// Block: compare the hypergraph model with the normal graph model by examining the final snapshot in two models
			// Construct the normal graph
			Hypergraph normalG;
			for (auto p: h.edge2id) {
				Hyperedge hyperedge = p.first;
				for (Hyperedge::iterator iter1 = hyperedge.begin(); iter1 != hyperedge.end(); ++iter1) {
					for (Hyperedge::iterator iter2 = hyperedge.begin(); iter2 != iter1; ++iter2) {
						Hyperedge e;
						e.push_back(*iter2);
						e.push_back(*iter1);
						normalG.insertEdge(e);
					}
				}
			}
			// Compute core values
			HypergraphCoreDecomp hcd(h), hcdn(normalG);
			hcd.solve();
			hcdn.solve();
			char fileName[50] = "StatHyperAndNormalCoreValue.txt";
			FILE *ofpCmpModel = fopen(fileName, "w");
			for (auto& p: hcd.c) {
				fprintf(ofpCmpModel, "%d\t%d\t%d\t%d\n", p.first, p.second, getApproxCoreVal(p.first), hcdn.c[p.first]);
			}
			fclose(ofpCmpModel);
			*/
		}
		// fclose(ofpVal);
		// fclose(ofpTime);
		// fclose(ofpDetail);

		/*
		// Block: compare between different parameters
		FILE *ofpComp = fopen("StatFullyDynamic.txt", "w");
		fprintf(ofpComp, "alpha = %f\n", alpha);
		fprintf(ofpComp, "lambda = %f\n", lambda);
		fprintf(ofpComp, "total time = %d ms\n", clock());
		fprintf(ofpComp, "total number of updates = %d\n", cnt);
		double maxErr = 0, avgErr = 0;
		int cntNonZero = 0;
		HypergraphCoreDecomp hcd(h);
		hcd.solve();
		for (auto& p: hcd.c) {
			const Node u = p.first;
			assert((hcd.c[u] == 0 && b[tau][u] == 0) || (hcd.c[u] > 0 && b[tau][u] > 0));
			if (hcd.c[u] > 0) {
				++cntNonZero;
				double err = max((double)hcd.c[u] / b[tau][u], (double)b[tau][u] / hcd.c[u]);
				maxErr = max(maxErr, err);
				avgErr += err;
			}
		}
		avgErr /= cntNonZero;
		fprintf(ofpComp, "max error = %.9f\n", maxErr);
		fprintf(ofpComp, "avg error = %.9f\n", avgErr);
		fclose(ofpComp);
		*/

		/*
		// Block: show that a large tau is unnecessary
		vector<double> maxErr(tau + 1, 0), avgErr(tau + 1, 0);
		int cntNonZero = 0;
		HypergraphCoreDecomp hcd(h);
		hcd.solve();
		for (auto& p: hcd.c) {
			const Node u = p.first;
			assert((hcd.c[u] == 0 && b[tau][u] == 0) || (hcd.c[u] > 0 && b[tau][u] > 0));
			if (hcd.c[u] > 0) {
				++cntNonZero;
				for (int t = 1; t <= tau; ++t) {
					if (maxErr[t] < max((double)hcd.c[u] / b[t][u], (double)b[t][u] / hcd.c[u]))
						maxErr[t] = max((double)hcd.c[u] / b[t][u], (double)b[t][u] / hcd.c[u]);
					avgErr[t] += max((double)hcd.c[u] / b[t][u], (double)b[t][u] / hcd.c[u]);
				}
			}
		}
		for (int t = 1; t <= tau; ++t)
			avgErr[t] /= cntNonZero;

		FILE *ofpLargeTau = fopen("LargeTauIsUnnecessaryFullyDynamic.txt", "w");
		fprintf(ofpLargeTau, "%d\n", tau);
		for (int t = 1; t <= tau; ++t)
			fprintf(ofpLargeTau, "%.9f %.9f\n", maxErr[t], avgErr[t]);
		fclose(ofpLargeTau);*/
	}
	unsigned getApproxCoreVal(Node u) {
		return b[tau][u];
	}
	void debug() {
		for (int i = 1; i <= 100; ++i)
			cerr << getApproxCoreVal(i) << ' ';
		cerr << endl;
	}
	Hypergraph h;
	GraphScheduler scheduler;
	ofstream outputFileTiming;
	bool stat; // if true, output core nums
	unsigned batch_size;
private:
	double epsilon, lambda, alpha, delta;
	string outFileName;
	unsigned maxNodeId;

	int tau;
	vector<unsigned> succ, pred;
	vector<unordered_map<Node, unsigned>> b, sigma, rho;
	void initialize() {
		// alpha should be r * (1 + 3 * epsilon) where r is maximum edge cardinality.
		// But we can slightly reduce it when r is too large.
		// alpha is decided by input.

		// alpha = 2 * (1 + 3 * epsilon); /// !! theoretically correct alpha
		// alpha = (2 + static_cast<double>(3) / delta);
		tau = ceil(0.15 * log(scheduler.numberOfNodes) / log(1.0 + epsilon));

		// Build succ and pred
		vector<unsigned> Lambda(1, 0);
		int i = 0;
		while (Lambda[i] <= scheduler.maxDegree) {
			Lambda.push_back(max((unsigned)(Lambda[i] * (1.0 + lambda)), Lambda[i] + 1));
			++i;
		}
		succ = pred = vector<unsigned>(Lambda[i] + 1);
		for (int j = 0; j < i; ++j)
			succ[Lambda[j]] = Lambda[j + 1], pred[Lambda[j + 1]] = Lambda[j];
		b.resize(tau + 1);
		sigma.resize(tau + 1);
		rho.resize(tau + 1);
		cout << "Lambda: ";
		for (auto& lambda: Lambda) cout << lambda << ' ';
		cout << endl;
		cout << "epsilon: " << epsilon << endl;
		cout << "lambda: " << lambda << endl;
		cout << "alpha: " << alpha << endl;
		cout << "# of nodes = " << scheduler.numberOfNodes << endl;
		cout << "max node ID = " << scheduler.maxNodeId << endl;
		cout << "tau = " << tau << endl;
		cout << "max degree = " << scheduler.maxDegree << endl;
	}
	void insertEdge(const Hyperedge& e) {
	//	cerr << "Insert an edge" << endl;
		h.insertEdge(e);
		unordered_set<Node> bad, bad2;
		for (const Node u: e)
			++sigma[1][u], ++rho[1][u], bad.insert(u);
		for (unsigned t = 1; t <= tau; ++t) {
			bad2.clear();
			if (t < tau) {
				unsigned b_e = INT_MAX;
				for (const Node u: e)
					b_e = min(b_e, b[t][u]);
				for (const Node u: e) {
					if (b_e >= succ[b[t + 1][u]])
						++sigma[t + 1][u], bad2.insert(u);
					if (b_e >= b[t + 1][u])
						++rho[t + 1][u];
				}
			}
			for (const Node u: bad)
				if (sigma[t][u] >= (unsigned)(alpha * succ[b[t][u]]))
					promote(t, u, bad2);
			swap(bad, bad2);
		}
	}
	void promote(const unsigned t, const Node u, unordered_set<Node>& bad2) {
	//	cerr << "Promote " << t << ' ' << u << ' ' << newEdgeId << endl;
		unsigned old_b_t_u = b[t][u];
		b[t][u] = succ[b[t][u]];
		updateSigmaAndRho(t, u);
	//	cerr << "b[" << t << "][" << u << "] = " << b[t][u] << ", sigma[" << t << "][" << u << "] = " << sigma[t][u] << endl;
		if (t == tau) return;
		for (const unsigned eId: h.eList[u]) {
	//		cerr << "eId = " << eId << endl;
			const Hyperedge& e = h.edgePool[eId];
			unsigned old_b_e = INT_MAX;
			for (const Node v: e)
				if (v != u)
					old_b_e = min(old_b_e, b[t][v]);
			unsigned new_b_e = min(old_b_e, b[t][u]);
			old_b_e = min(old_b_e, old_b_t_u);
			if (new_b_e == old_b_e) continue;
			for (const Node v: e) {
				if (old_b_e < succ[b[t + 1][v]] && succ[b[t + 1][v]] <= new_b_e)
					++sigma[t + 1][v], bad2.insert(v);
				if (old_b_e < b[t + 1][v] && b[t + 1][v] <= new_b_e)
					++rho[t + 1][v];
			}
		}
	//	cerr << "Promote finished." << endl;
	}
	void deleteEdge(const Hyperedge& e) {
	//	cerr << "Delete an edge" << endl;
		h.deleteEdge(e);
		unordered_set<Node> bad, bad2;
		for (const Node u: e)
			--sigma[1][u], --rho[1][u], bad.insert(u);
		for (unsigned t = 1; t <= tau; ++t) {
			bad2.clear();
			if (t < tau) {
				unsigned b_e = INT_MAX;
				for (const Node u: e)
					b_e = min(b_e, b[t][u]);
				for (const Node u: e) {
					if (b_e >= succ[b[t + 1][u]])
						--sigma[t + 1][u];
					if (b_e >= b[t + 1][u])
						--rho[t + 1][u], bad2.insert(u);
				}
			}
			for (const Node u: bad)
				if (rho[t][u] < b[t][u])
					demote(t, u, bad2);
			swap(bad, bad2);
		}
	}
	void demote(const unsigned t, const Node u, unordered_set<Node>& bad2) {
	//	cerr << "Demote " << t << ' ' << u << endl;
		unsigned old_b_t_u = b[t][u];
		b[t][u] = pred[b[t][u]];
		updateSigmaAndRho(t, u);
		if (t == tau) return;
		for (const unsigned eId: h.eList[u]) {
			const Hyperedge& e = h.edgePool[eId];
			unsigned old_b_e = INT_MAX;
			for (const Node v: e)
				if (v != u)
					old_b_e = min(old_b_e, b[t][v]);
			unsigned new_b_e = min(old_b_e, b[t][u]);
			old_b_e = min(old_b_e, old_b_t_u);
			if (new_b_e == old_b_e) continue;
			for (const Node v: e) {
				if (new_b_e < succ[b[t + 1][v]] && succ[b[t + 1][v]] <= old_b_e)
					--sigma[t + 1][v];
				if (new_b_e < b[t + 1][v] && b[t + 1][v] <= old_b_e)
					--rho[t + 1][v], bad2.insert(v);
			}
		}
	}
	void updateSigmaAndRho(unsigned t, Node u) {
		sigma[t][u] = rho[t][u] = 0;
		for (const unsigned eId: h.eList[u]) {
			const Hyperedge& e = h.edgePool[eId];
			unsigned b_e = INT_MAX;
			if (t > 1)
				for (const Node v: e)
					b_e = min(b_e, b[t - 1][v]);
			if (b_e >= succ[b[t][u]]) ++sigma[t][u];
			if (b_e >= b[t][u]) ++rho[t][u];
		}
	}
};


inline string precisionString(double a){
	stringstream precisionValue;
	precisionValue << std::setprecision(3);
  	precisionValue << a;
	return precisionValue.str();
}


int main(int argc, char **argv) {
	double epsilon = atof(argv[1]); // take in our paramers and compute for sun's
	double lambda = atof(argv[2]);
	// double alpha = atof(argv[3]);
	// char *fileName = argv[4];
	char *fileName = argv[3];
	char *outFileNameArg = argv[4];
	int stat = atof(argv[5]); // if 1 output core num
	int batch_size = atof(argv[6]);

	// string alphaS = precisionString(alpha);
	string lambdaS = precisionString(lambda);
	string epsilonS = precisionString(epsilon);
	// string deltaS = precisionString(delta);

	string suffix = "_" + epsilonS + "_" + lambdaS;
	string outFileName(outFileNameArg);
	outFileName.append(suffix);
    std::cout << "output to " << outFileName << std::endl;
	cout << "start running ..." << endl;

	double epsilon_sun =1.0/2.0/lambda*(1+epsilon) + epsilon/3;// ((2+3/lambda)*(1+epsilon)/2-1)/3 ;//
	// cout << epsilon_sun << endl; exit(1);
	double alpha = 2 * (1 + 3 * epsilon_sun); /// theoretically correct alpha

	
	for (int round = 0; round < ROUND_NUM; round++){
		string outFileNameRound = outFileName;
		string roundS = precisionString(round);
		outFileNameRound.append("_round_"+roundS);
		FullyDynamic fullyDynamic(epsilon_sun, epsilon_sun, alpha, fileName, outFileNameRound, stat == 1, batch_size);
		gbbs::timer t; t.start();
		fullyDynamic.run();
		cout << endl;
		t.next("Finished!!!");
		fullyDynamic.scheduler.reset();
	}

	return 0;
}
