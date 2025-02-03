import os
import random
import math
import numpy as np

# Constants
n = 5                        # Length of random key or seed
power = 2 ** n               # Calculate 2^n
m = 6                        # Length of passcode or ciphertext or keystream
byte_values = 256            # Possible byte values (0 to 255)
digit_values = 10            # Possible digit values (0 to 9)
total_keys = 2 ** 24         # Total number of unique keys
ciphertexts_per_file = 4096  # Number of ciphertexts per file
total_files = 4096           # Total number of files
start = 48                   # ASCII value for character '0'

# Key Schedule Algorithm
def key_schedule_algorithm(K):
    S = list(range(power))
    j = 0
    l = len(K)

    for i in range(power):
        j = (j + S[i] + K[i % l]) % power
        S[i], S[j] = S[j], S[i]

    return S

# Pseudo Random Generation
def pseudo_random_generation(S, m):
    i = 0
    j = 0
    key_stream = [0] * m

    for k in range(m):
        i = (i + 1) % power
        j = (j + S[i]) % power
        S[i], S[j] = S[j], S[i]
        key_stream[k] = S[(S[i] + S[j]) % power]

    return key_stream

# Generate Key Stream Distribution
def generate_key_stream_distribution(total_keys):
    Z = [[0] * byte_values for _ in range(m)]
    unique_keys = set()

    while len(unique_keys) < total_keys:
        K = random.sample(range(256), n)
        key_tuple = tuple(K)

        if key_tuple not in unique_keys:
            unique_keys.add(key_tuple)
            S = key_schedule_algorithm(K)
            key_stream = pseudo_random_generation(S, m)

            for r in range(m):
                Z[r][key_stream[r]] += 1

    return Z

# Extract integers from a cipher
def extract_integers(line):
    return [int(value.strip()) for value in line.strip()[1:-1].split(',')]

# Read ciphertexts from all files
def read_ciphertexts():
    all_ciphertexts = []

    # Loop over all files
    for file_index in range(1, total_files + 1):
        filename = f"Ciphertexts/ciphertexts_{file_index}.txt"
        #print(f"Trying to open file: {filename}")

        if not os.path.isfile(filename):
            print(f"Error opening file: {filename}")
            continue
        
        # Read each ciphertext from the file
        with open(filename, 'r') as file:
            for i in range(ciphertexts_per_file):
                line = file.readline()
                if line:
                    ciphertext = extract_integers(line)
                    if len(ciphertext) == m:
                        all_ciphertexts.append(ciphertext)
                    else:
                        print(f"Invalid ciphertext length in line {i + 1} of file {filename}")

    return all_ciphertexts


def estimate_plaintext_byte(ciphertexts, Z):
    """
    Estimate plaintext bytes from ciphertexts using Single-byte Bias-Attack.
    
    Args:
        ciphertexts (list): List of ciphertexts to estimate plaintext from.
        Z (list): Count occurrences of each byte for each position in keystream.
        
    Returns:
        list: Estimated plaintext bytes for each position.
    """
    count = np.zeros((m, byte_values), dtype=int)

    # Step 1: Count occurrences of each byte at each position
    for ciphertext in ciphertexts:
        for pos in range(m):
            count[pos][ciphertext[pos]] += 1

    # Step 2: Estimate plaintext byte using the bias-adjusted counts
    estimated_plaintext = [0] * m

    for pos in range(m):
        lambda_values = [0.0] * digit_values  # λ values for each possible byte (µ)

        # Calculate λ for each possible byte value (µ)
        for mu in range(digit_values):
            for k in range(byte_values):
                biased_count = count[pos][k ^ (mu + start)] # Count of byte k XORed with (µ + start)

                if Z[pos][k] > 0:
                    log_prob = Z[pos][k]
                    log_prob /= total_keys
                    log_prob = math.log(log_prob)
                else:
                    log_prob = pow(10, -20) # Log probability set to -∞ that is pow(10, -20)

                lambda_values[mu] += biased_count * log_prob # Weighted sum of biased counts

        # Step 3: Select the µ that maximizes λ as the estimated byte
        max_mu = 0
        max_lambda = lambda_values[0]

        for mu in range(1, digit_values):
            if lambda_values[mu] > max_lambda:
                max_lambda = lambda_values[mu]
                max_mu = mu

        estimated_plaintext[pos] = max_mu
        #print(f"\nChosen plaintext byte for position {pos}: {max_mu}\n")

    return estimated_plaintext

# Main
if __name__ == "__main__":

    # Generating key stream distribution
    Z_counts = generate_key_stream_distribution(total_keys)

    # Reading all ciphertexts
    all_ciphertexts = read_ciphertexts()
    #print(f"Total ciphertexts extracted: {len(all_ciphertexts)}")

    # Estimating plaintext bytes using Single-byte Bias Attack
    estimated_bytes = estimate_plaintext_byte(all_ciphertexts, Z_counts)

    # Converting estimated plaintext bytes to a string representation of the passcode
    passcode = ''.join(str(byte) for byte in estimated_bytes)

    # Display the estimated plaintext
    print(f"Estimated Passcode: {passcode}\n")
