FILE=ERR279856
DIR=/data/referee
OUTPUT=/data/common/$FILE
for X in 10 12 14 16 18 20 22 24 26:
do
	echo $X
	time jellyfish count -s 100000000 -t 3 -m $X -o $OUTPUT.$X.kmers $DIR/$FILE.fasta
	time jellyfish dump -o $OUTPUT.$X.kmers.fa $OUTPUT.$X.kmers
	wc -l $OUTPUT.$X.kmers.fa
	grep -v ">" $OUTPUT.$X.kmers.fa > $OUTPUT.$X.kmers_only
	rm $OUTPUT.$X.kmers.fa
	/usr/bin/time -lp ./bit_tree encode $OUTPUT.$X.kmers_only
done
