# bit_tree_cpp
Bit tree implementation in C++11

Bit tree is an efficient 4-ary tree representation of a collection of nucleotide strings all of which are of the same length. Construction and binary encoding of a tree are described in detail here:

  C. Kingsford, R. Patro. "[Reference-based compression of short-read sequences using path encoding.](http://bioinformatics.oxfordjournals.org/content/31/12/1920.full)" Bioinformatics. 31(12): 1920-1928. 2015.

Oftentimes, bit tree representation is more efficient than gzipping or plzipping a kmer collection. Here is its performance on non-canonical kmers of [ERR279856](http://www.ncbi.nlm.nih.gov/sra/?term=ERR279856), as counted by Jellyfish:

![plot of data in the table below](https://github.com/lynxoid/bit_tree_cpp/blob/master/size_vs_k.png )

Actual sizes are as such:

k |	as strings |	kmers lzipped |	bit tree
-----|---:|----:|-----------------------------:
10 |	9,652,797 |	1,487,340 |	172,601
12 | 42,347,292 | 11,186,877 |	1,656,921
14 | 62,011,560 | 17,287,884 | 5,221,649
16 | 73,145,781 | 20,192,268 | 9,402,177
18 | 83,236,359 | 22,351,948 | 13,711,825
20 | 93,328,410 | 24,253,686 | 18,088,297
22 | 103,522,356 | 25,973,124 | 22,519,457
24 | 113,811,100 | 27,523,363 | 26,998,329

For `k=10` (`k=12`), the size of the bit tree was 55 (25) times smaller than the size of the original kmers in plain text (or 11 times smaller than their 2-bit representation). On average, the compression ratio on these tests is 15.15 for bit tree and 4.15 for plzip.

You can generate the kmers and run your own tests with the `run_kmers.sh` script.

### Dependencies

Boost 1.54

### Compilation

make
