import random
import os

amount = [3, 4, 5, 6, 7]

def generate_kmers(k, num_kmers, existing_kmers):
    generated_kmers = set()
    while len(generated_kmers) < num_kmers:
        kmer = ''.join(random.choices('ACGT', k=k))
        if kmer not in existing_kmers:
            generated_kmers.add(kmer)
    return list(generated_kmers)

for am in amount:
    file_directory = 'data/len_' + str(am)
    os.makedirs(file_directory, exist_ok=True)
    sequence = ''.join(random.choice(['A', 'C', 'G', 'T']) for _ in range(10**am))
    sequence_file = os.path.join(file_directory, 'sequence.fna')
    with open(sequence_file, 'w') as file:
        file.write(sequence)
    
    for k in [10, 20, 50, 100, 200]:
        existing_kmers = []
        for i in range(0, len(sequence) - k + 1, k):
            existing_kmers.append(sequence[i:i+k])

        num_kmers_needed = 100
        new_kmers = generate_kmers(k, num_kmers_needed, existing_kmers)
        kmer_file = os.path.join(file_directory, 'new_kmers_{}_{}.txt'.format(am, k))
        with open(kmer_file, 'w') as kmer_file:
            for kmer in new_kmers:
                kmer_file.write(kmer + '\n')
    
file_directory = 'data/e_coli_genome.fna'

sequence = ''
with open('data/e_coli_genome.fna', 'r') as file:
    for line in file:
        sequence += line

for k in [10, 20, 50, 100, 200]:
    existing_kmers = []
    for i in range(0, len(sequence) - k + 1, k):
        existing_kmers.append(sequence[i:i+k])

    num_kmers_needed = 100
    new_kmers = generate_kmers(k, num_kmers_needed, existing_kmers)
    kmer_file = os.path.join('data', 'new_kmers_sim_{}.txt'.format(k))
    with open(kmer_file, 'w') as kmer_file:
        for kmer in new_kmers:
            kmer_file.write(kmer + '\n')