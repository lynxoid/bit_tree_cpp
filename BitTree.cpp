// bit tree
#include "BitTree.hpp"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<vector<kmer_t>> parseStrings(string const filename, int & k) {
	ifstream in_file(filename);
	if (!in_file) {
		cerr << "[ERROR] File does not exist: " << filename << endl;
		exit(0);
	}
	string kmer;
	shared_ptr<vector<kmer_t>> kmers(new vector<kmer_t>());
	while ( getline(in_file, kmer) ) {
		k = kmer.size();
		kmers->push_back(kmer);
	}
	return kmers;
}

//////////////////////////////////////////////////////////////////////////////////////////
struct Params {
	string mode = "encode";
	string input_file;
	bool sorted = false;
};

//////////////////////////////////////////////////////////////////////////////////////////
void usage() {
	cerr << "Usage:" << endl << endl;
	cerr << "\t./bit_tree action input_file [0|1]" << endl << endl;
	cerr << "\taction:\t\tencode, decode" << endl;
	cerr << "\tinput_file:\ta collection of kmer strings separated by newlines" << endl;
	cerr << "\t0|1:\t\t0 indicates that kmers are already sorted lexicographically" << endl <<
			"\t\t\t1 (default) -- kmers will be sorted before encoding them" << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////
Params parseOptions(int argc, char * argv []) {
	Params p;
	if (argc < 3) {
		cerr << "[ERROR] Missing required options" << endl;
		usage();
		exit(1);
	}
	p.mode = argv[1];
	p.input_file = argv[2];
	if (argc > 3)
		p.sorted = stoi(argv[3]);
	return p;
}

/****************************************************************
**
**
**
****************************************************************/
int main(int argc, char * argv []) {
	Params p = parseOptions(argc, argv);
	int k = 0;
	shared_ptr<vector<kmer_t>> kmers(new vector<kmer_t>());

	cerr << "============================" << endl;
	cerr << "==== BitTree, C++11 ========" << endl;
	cerr << "============================" << endl;

	BitTree bit_tree;
	// DAWGS: https://code.google.com/p/dawgdic/
	if (p.mode.compare("encode") == 0) {
		cerr << "Encoding " << argv[2] << endl;
		{
		// boost::timer t;
		kmers = parseStrings(p.input_file, k);
		// cerr << "reading took: " << t.elapsed() << "s" << endl;
		}
		assert(k > 0 && k <= 32);
		assert(kmers->size() > 0);

		cerr << "Parsed " << kmers->size() << " kmers" << endl;
		if (!p.sorted) { // sort the little guys
			std::sort(kmers->begin(), kmers->end());
			cerr << "sorted" << endl;
		}

		bit_tree.encode(kmers, k);
		cerr << "Writing to a binary file..." << endl;
		{
		// boost::timer t;
		bit_tree.write(p.input_file + ".bt");
		// cerr << "Writing took " << t.elapsed() << "s" << endl;
		}
	}
	else if (p.mode.compare("decode") == 0 ) {
		cerr << "Decoding " << p.input_file << endl;
		
		bit_tree.read(p.input_file, k);
		assert(k > 0 && k <= 32);
		{
		// boost::timer t;
		kmers = bit_tree.decode();
		// cerr << "Decoding took " << t.elapsed() << "s" << endl;
		}
		cerr << "Recovered " << kmers->size() << " kmers" << endl;
		cerr << "Writing to stdin..." << endl;
		for (auto kmer : *kmers) {
			cout << kmer << endl;
		}
	}
	else {
		cerr << "Unknown mode: " << p.mode << ". Stopping." << endl;
	}
	cerr << "Done." << endl;
	cerr << "============================" << endl;
}
