from Crypto.Cipher import AES

def expand_key(short_key):
    # Simulated expansion (use provided implementation if available)
    return short_key[:-1]  # Remove last 4 bits for effective key

def brute_force_attack(plaintext, ciphertext, secret_ciphertext):
    for key in range(2**20):
        short_key = key.to_bytes(3, 'big') + b'\x00'  # Pad to 24 bits
        expanded_key = expand_key(short_key)
        cipher = AES.new(expanded_key, AES.MODE_ECB)
        if cipher.encrypt(plaintext) == ciphertext:
            # Key found
            print(f"Key found: {key}")
            decrypted_secret = cipher.decrypt(secret_ciphertext)
            print(f"Decrypted secret plaintext: {decrypted_secret}")
            return key, decrypted_secret
    print("Key not found.")
    return None, None

# Example Usage
plaintext = b"Counterclockwise"  # Replace with provided plaintext
ciphertext = b"fe0f42ae809fe1e2ff5b59725ef52048"  # Replace with provided ciphertext
secret_ciphertext = b"ca6889853e3ddfaf621b87ee4966e274"  # Replace with provided secret ciphertext

brute_force_attack(plaintext, ciphertext, secret_ciphertext)
