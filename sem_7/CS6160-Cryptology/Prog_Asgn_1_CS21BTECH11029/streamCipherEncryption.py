import os

def key_gen(key_length):
    return os.urandom(key_length)

def stream_cipher_encrypt(message, key):
    return bytes([m ^ k for m, k in zip(message, key)])

def read_from_file(fname):
    with open(fname, 'r') as f:
        messages = [line.strip().encode() for line in f.readlines()]
    return messages

def main():
    key_length1 = 200
    key_length2 = 16
    key1 = key_gen(key_length1)
    key2 = key_gen(key_length2)
    
    input_name = "set.txt"
    output_name = "output.txt"

    messages = read_from_file(input_name)

    with open(output_name, 'wb') as opt:
        for message in messages:
            ciphertext1 = stream_cipher_encrypt(message[:16], key2[:16])
            ciphertext2 = stream_cipher_encrypt(message[16:], key1[:len(message) - 16])
            ciphertext = ciphertext1 + ciphertext2
            opt.write(ciphertext.hex().encode() + b'\n')

if __name__ == "__main__":
    main()
