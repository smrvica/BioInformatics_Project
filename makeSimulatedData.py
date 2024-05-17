import random


with open('simulatedData.fna', 'w') as file:
        for _ in range(200 * 1000):
            sequence = random.choice(['A', 'C', 'G', 'T'])
            file.write(sequence)