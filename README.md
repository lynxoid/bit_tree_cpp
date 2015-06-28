# bit_tree_cpp
Bit tree implementation in C++11

Bit tree is an efficient 4-ary tree representation of a collection of nucleotide strings all of which are of the same length. Construction and binary encoding of a tree are described in detail here:

  C. Kingsford, R. Patro. "[Reference-based compression of short-read sequences using path encoding.](http://bioinformatics.oxfordjournals.org/content/31/12/1920.full)" Bioinformatics. 31(12): 1920-1928. 2015.

Oftentimes, bit tree representation is more efficient than gzipping or plzipping a kmer collection. Here is its perofrmance on non-canonical kmers of [ERR279856](http://www.ncbi.nlm.nih.gov/sra/?term=ERR279856), as counted by Jellyfish:

`plot`

You can generate the kmers and run your own tests with `run_kmers.sh` script.
