// BitTree library

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <memory>

#include <boost/dynamic_bitset.hpp>
// #include <boost/progress.hpp>

// #include "IOUtil.hpp"
// #include "JellyfishUtil.h"

using namespace std;

// #define kmer_t bin_kmer_t
#define kmer_t string
// #define kmer_type kmer_t

size_t getFileLength(ifstream & stream) {
  // get file length
  stream.seekg(0, stream.end);  // move to the end
  int length = stream.tellg();   // get the size in bytes?
  stream.seekg(0, stream.beg);  // move back to the beginning
  return length;
}


class BitTree {
private:

	// bitset that will contain the bittree data
	shared_ptr<boost::dynamic_bitset<>> bitstream;

	// kmer size
	int k = 0;

	//////////////////////////////////////////////////////////////////////////////////////////
	// given a range (start, end), find a subrange that has char c at position depth
	//////////////////////////////////////////////////////////////////////////////////////////
	pair<int,int> findRange(int start, int end, shared_ptr<vector<kmer_t>> kmers, int depth, char c) {
		//cerr << start << "," << end << "-" << depth << "-" << c << " " << kmers->size() << " ";
		int i = start;
		while (i < end && (*kmers)[i][depth] != c) i++;
		int a = i;
		//cerr << "(" << a << ",";
		i = a;
		while ( (i < end) && (*kmers)[i][depth] == c) {i++; /*cerr << i << " ";*/ }
		int b = i;
		//cerr << b << ") ";
		return pair<int,int>(a, b);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// 
	//////////////////////////////////////////////////////////////////////////////////////////
	void write64(ofstream & f_output, unsigned long long n) {
		f_output << (unsigned char) ((n >> 0) & (uint64_t)255);
		f_output << (unsigned char) ((n >> 8) & (uint64_t)255);
		f_output << (unsigned char) ((n >> 16) & (uint64_t)255);
		f_output << (unsigned char) ((n >> 24) & (uint64_t)255);
		f_output << (unsigned char) ((n >> 32) & (uint64_t)255);
		f_output << (unsigned char) ((n >> 40) & (uint64_t)255);
		f_output << (unsigned char) ((n >> 48) & (uint64_t)255);
	    f_output << (unsigned char) ((n >> 56) & (uint64_t)255);
	}

public:

	BitTree() {} // defualt constructor

	//////////////////////////////////////////////////////////////////////////////////////////
	// given a set of kmers (in binary), encode the trie they form in binary
	//////////////////////////////////////////////////////////////////////////////////////////
	void encode(shared_ptr<vector<kmer_t>> kmers, int const k) {
		stack<int> starts; starts.push(0);
		stack<int> ends; ends.push(kmers->size());
		stack<int> depths; depths.push(0);
		string alphabet = "TGCA";

		shared_ptr<boost::dynamic_bitset<>> bitstream( new boost::dynamic_bitset<>());
		int start, end, depth = 0;

		while (starts.size() > 0) {
			start = starts.top(); starts.pop();
			end = ends.top(); ends.pop();
			depth = depths.top(); depths.pop();
			if (start == end) {
				// cerr << "0 " << depth << " " << endl;
				bitstream->push_back(0);
			}
			else {
				//if (depth < 5)
				//	cerr << (*kmers)[start].substr(0,depth) << endl;
				// cerr << "1 " << depth << endl;
				bitstream->push_back(1);
				if (depth < k) {
					// cerr << (*kmers)[start].substr(0, depth) << endl;
					pair<int,int> p(end, end);
					for (auto c : alphabet) {
						p = findRange(start, p.first, kmers, depth, c);
						starts.push(p.first); ends.push(p.second); depths.push(depth+1);
					}
				}
				else {
					// cerr << (*kmers)[start].substr(0, depth) << endl;
					// cerr << "TODO" << endl;
				}
			}
		}
		// cerr << endl;
		// string s;
		// boost::to_string(*bitstream, s);
		// cerr << s << endl;
		this->bitstream = bitstream;
		this->k = k;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// given binary stream of a trie, reconstruct the strings
	//////////////////////////////////////////////////////////////////////////////////////////
	shared_ptr<vector<kmer_t>> decode() {
		shared_ptr<vector<kmer_t>> kmers(new vector<kmer_t>());
		
		// skip the first 1
		size_t index = bitstream->find_first() + 1;
		string alphabet = "ACGT";
		int branch = 0;
		int depth = 0;
		string prefix = "";
		// put branches index on stack as we go down the tree, pop as we go back up the tree
		stack<int> branches;


		while (index < bitstream->size() ) {
			if ( (*bitstream)[index] ) {
				prefix += alphabet.at(branch);
				if (depth == (k-1) ) {
					kmers->push_back(prefix);
					if (kmers->size() % 1000000 == 0) cerr << kmers->size() << " ";
					prefix = prefix.substr(0, prefix.size() - 1);
					
					while (branch >= 3 && !branches.empty()) {
							depth--;
							branch = branches.top(); branches.pop();
							prefix = prefix.substr(0, prefix.size() - 1);
					}
					if (branch < 3)
						branch++;
				}
				else { // descend down the tree
					branches.push(branch);
					branch = 0;
					depth++;
				}
			}
			else {
				while (branch >= 3 && !branches.empty()) {
					depth--;
					branch = branches.top(); branches.pop();
					prefix = prefix.substr(0, prefix.size() - 1);
				}
				if (branch < 3)
					branch++;
			}
			index++;
		}
		return kmers;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// write binary stream to a file
	//////////////////////////////////////////////////////////////////////////////////////////
	void write(string const & fname) {
		ofstream f_output(fname, ios::binary | ios::out);
		f_output << (char)k; // write out the value of k
		auto size = sizeof(uint64_t)*8;
		auto tail = bitstream->size() % size;
		// pad with 0s at the end so that the tail does not get messed up when writing
		bitstream->resize(bitstream->size() + size - tail);
		*bitstream <<= (size - tail);
		// string s;
		// boost::to_string(*bitstream, s);
		// cerr << s << endl;

		// convert into a vector of block_types 
		vector<boost::dynamic_bitset<>::block_type> OutIter;
		to_block_range(*bitstream, back_inserter(OutIter) );
		// iterate and write one block_type at a time
		for (boost::dynamic_bitset<>::block_type block : OutIter) {
			write64(f_output, block);
		}
		f_output.close();
		cerr << "Output written to " << fname << endl;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// read binary stream into the dynamic_bitset
	//////////////////////////////////////////////////////////////////////////////////////////
	void read(string const & fname, int & k) {
		ifstream f_in(fname, ios::binary | ios::in);
		auto fsize = getFileLength(f_in);
		f_in.read(reinterpret_cast<char *>(&k), 1);
		cerr << "k = " << k << endl;
		shared_ptr<boost::dynamic_bitset<>> bitstream(new boost::dynamic_bitset<>());
		// add buffer[1:] to the bitstream
		auto bytes_per_block = sizeof(boost::dynamic_bitset<>::block_type);
		auto num_blocks = (fsize - 1) / bytes_per_block;
		// cerr << num_blocks << endl;
		boost::dynamic_bitset<>::block_type block;
		for (int i = 0; i < num_blocks; i++) {
			f_in.read(reinterpret_cast<char *>(&block), bytes_per_block);
			bitstream->append(block);
		}
		this->bitstream = bitstream;
		this->k = k;
	}



};