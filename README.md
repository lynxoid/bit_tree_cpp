# bit_tree_cpp
Bit tree implementation in C++11

Bit tree is an efficient 4-ary tree representation of a collection of nucleotide strings all of which are of the same length. Construction and binary encoding of a tree are described in detail here:

  C. Kingsford, R. Patro. "[Reference-based compression of short-read sequences using path encoding.](http://bioinformatics.oxfordjournals.org/content/31/12/1920.full)" Bioinformatics. 31(12): 1920-1928. 2015.

Oftentimes, bit tree representation is more efficient than gzipping or plzipping a kmer collection. We ran an experiment were we compared results of plzipping kmer strings (lz line) vs building a bit tree on those same kmers (bit tree line). Some performance numbers on kmers are below (counted by Jellyfish):

[SRR037452](http://www.ncbi.nlm.nih.gov/sra/?term=SRR037452)

![data for SRR037452](https://github.com/lynxoid/bit_tree_cpp/blob/master/SRR037452_size_vs_k.png)


Actual sizes are:

k |	kmers lzipped |	bit tree | bit tree + lz
-----|---:|----:|-----------------------------:
10 |	1,267,692 |		157,377 |		6,083
12 |		16,768,251 |		2,497,689 |		285,137
14 |		209,338,609 |		27,138,873 |		20,177,568
16 |		460,208,443 |		102,499,425 |		86,198,199
18 |		549,068,270 |		195,150,889 |		158,596,795
20 |		578,979,167 |		277,692,945 |		217,172,798
22 |		583,681,685 |		342,331,145 |		258,201,259
24 |		566,885,773 |		385,700,657 |		282,575,128

For `k=10` (`k=12`), the size of the bit tree was 55 (25) times smaller than the size of the original kmers in plain text (or 11 times smaller than their 2-bit representation). On average, the compression ratio on these tests is 15.15 for bit tree and 4.15 for plzip.

You can generate the kmers and run your own tests with the `run_kmers.sh` script.

### Dependencies

Boost 1.54

### Compilation

make

### TODO

- [ ] using binary kmer representations -- do they improve perofrmance?
- [ ] using RRR vectors to further compress bit tree binary output
- [ ] can bit tree data structure be used for kmer access (currently only storage & transmission)
