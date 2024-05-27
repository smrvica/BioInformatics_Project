g++ -std=c++11 test/testOnData.cpp -o testOnData

numbers=(10 20 50 100 200)
exponents=(3 4 5 6 7)
bits=(8 16)

echo "===============================" > test.txt
echo "SIMULATING ON E. COLI GENOME" >> test.txt
echo "===============================" >> test.txt

echo "" >> test.txt

for number in "${numbers[@]}"
do
    for b in "${bits[@]}"
    do
        ./testOnData data/ecoli/e_coli_genome.fna data/ecoli/new_kmers_sim_ "$number" "$b" >> test.txt
    done
done

for exp in "${exponents[@]}"
do 

    echo "===============================" >> test.txt
    echo "SIMULATING ON SIMULATED DATA 10^$exp" >> test.txt
    echo "===============================" >> test.txt

    echo "" >> test.txt

    for number in "${numbers[@]}"
    do
        for b in "${bits[@]}"
        do
            ./testOnData data/len_$exp/sequence.fna data/len_$exp/new_kmers_$exp\_ "$number" "$b" >> test.txt
        done
    done

done