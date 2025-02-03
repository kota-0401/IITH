#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Converts a hexadecimal character to its decimal equivalent.
// '0'-'9' are mapped to 0-9, 'a'-'f' are mapped to 10-15.
int find(char c) {
    if ('0' <= c && c <= '9') {
        return c - '0';
    }
    return c - 'a' + 10;  // For 'a' to 'f'
}

// Converts a decimal value (0-15) to its hexadecimal equivalent.
// 0-9 are converted to '0'-'9', 10-15 are converted to 'a'-'f'.
char get(int c) {
    if (c < 10) {
        return c + '0';  // For 0-9
    }
    return 'a' + c - 10;  // For 10-15
}

// Converts a string of ASCII characters to its hexadecimal representation.
// Each character in the string is represented by two hexadecimal digits.
string convert_to_hexa(string &s) {
    string res = "";
    for (int i = 0; i < s.size(); i++) {
        int a = s[i];  // Get the ASCII value of the character
        res += get(a / 16);  // First hexadecimal digit
        res += get(a % 16);  // Second hexadecimal digit
    }
    return res;
}

// Converts a hexadecimal string to its ASCII representation.
// Each pair of hexadecimal digits is converted to a character.
string hexa_to_decimal(string &s) {
    string msg = "";
    for (int i = 0; i < s.size(); i += 2) {
        // Convert two hexadecimal digits back to decimal (ASCII value)
        int l = find(s[i]) * 16 + find(s[i + 1]);
        
        // Check if the character is valid (a lowercase letter, space, or period)
        if (l == ' ' || l == '.' || 'a' <= l && l <= 'z') {
            msg += l;  // Add valid characters to the message
        }
        else {
            return "invalid";  // If an invalid character is found, return "invalid"
        }
    }
    return msg;
}

// XORs two hexadecimal strings and returns the result as a hexadecimal string.
// The XOR is performed on each pair of corresponding bytes in the input strings.
string xor_bytes(string& a, string& b) {
    string msg = "";
    int size = min(a.size(), b.size());  // Determine the smaller size to avoid out-of-bounds access
    for (int i = 0; i < size; i += 2) {
        // Convert the corresponding hexadecimal pairs to decimal
        int l = find(a[i]) * 16 + find(a[i + 1]);
        int k = find(b[i]) * 16 + find(b[i + 1]);
        
        // XOR the two values and convert the result back to hexadecimal
        l ^= k;
        msg += get(l / 16);  // First hexadecimal digit
        msg += get(l % 16);  // Second hexadecimal digit
    }
    return msg;
}

// Checks if the given message is present in the dictionary.
// The function returns true if the message is found, otherwise false.
bool is_in_dictionary(const vector<string>& dictionary, const string& message) {
    return find(dictionary.begin(), dictionary.end(), message) != dictionary.end();
}


//Main Function
int main() {
    // Read dictionary from file
    ifstream file1("dictionary.txt");
    vector<string> dictionary;
    string line;
    if (file1.is_open()) {
        while (getline(file1, line)) {
            dictionary.push_back(line);
        }
        file1.close();
    } 
    else {
        cerr << "Unable to open file dictionary.txt" << endl;
        return 1;
    }
    
    // Read ciphertexts from file
    ifstream file2("Set1_streamciphertexts.txt");
    vector<string> ciphers;
    if (file2.is_open()) {
        while (getline(file2, line)) {
            ciphers.push_back(line);
        }
        file2.close();
    } 
    else {
        cerr << "Unable to open file Set1_streamciphertexts.txt" << endl;
        return 1;
    }

    // Open output file for writing
    ofstream output_file("Prog_Asgn_1_CS21BTECH11029.txt");
    if (!output_file.is_open()) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    // Initialize 2D vector to store plaintexts
    vector<vector<int> > plain_texts(12);

    // Determine the maximum possible size for K2
    int K2_size = 0;
    for (int i = 0; i < 12; i++) {
        int size = ciphers[i].size() / 2;  // Size in bytes (each hex pair represents a byte)
        plain_texts[i].resize(size, 256);  // Initialize with 256 (default value for unknown bytes)
        K2_size = max(K2_size, size - 16);  // Update K2_size to be the largest size needed
    }

    // Initialize K2 with the maximum size found
    vector<int> K2(K2_size, 256);  // 256 is used to denote unknown values

    // Iterate through each dictionary entry to find valid K1
    for (int j = 0; j < dictionary.size(); j++) {
        string plain_text = dictionary[j];
        string hexa_plain_text = convert_to_hexa(plain_text);  // Convert the plaintext to hexadecimal
        string K1 = xor_bytes(hexa_plain_text, ciphers[0]); // Calculate K1 using the first cipher

        vector<string> msgs;
        msgs.push_back(plain_text);
        bool valid = true;

        // Validate the K1 key with all other ciphertexts
        for (int i = 1; i < ciphers.size(); i++) {
            string hexa_message = xor_bytes(K1, ciphers[i]);  // Decrypt the cipher text using K1
            string message = hexa_to_decimal(hexa_message);  // Convert hex to ASCII

            // Check if the decrypted message is valid and in the dictionary
            if (message == "invalid" || !is_in_dictionary(dictionary, message)) {
                valid = false;
                break;
            }
            msgs.push_back(message);
        }
        
        // If all messages are valid, write K1 and plaintexts to the output file
        if (valid) {
            output_file << "K1 = " << K1 << endl;
            for (int i = 0; i < 12; i++) {
                for (int j = 0; j < 16; j++) {
                    int a = msgs[i][j];  // Get the character from the decrypted message
                    plain_texts[i][j] = a;  // Store the character in the plaintexts vector
                }
            }
            break;  // Exit the loop after finding a valid key and plaintexts
        }
    }

    // Vector to store pairwise XOR results of ciphertexts
    for (int i = 0; i < 12; i++) {
        vector<string> xor_pairs;
        
        // Calculate pairwise XORs of ciphertexts
        for (int j = 0; j < 12; j++) {
            if (i != j) {
                xor_pairs.push_back(xor_bytes(ciphers[i], ciphers[j]));
            }
        }
        
        // Deduce the last byte of K2 corresponding to '.' 
        // Assuming the last character in plaintexts is a period ('.')
        int last_byte_index = (ciphers[i].size() / 2) - 1;
        K2[last_byte_index - 16] = '.';
        K2[last_byte_index - 16] ^= (16 * find(ciphers[i][2 * last_byte_index]) + find(ciphers[i][2 * last_byte_index + 1])); // XOR to deduce the key byte
        
        // Analyze the XOR results to deduce the rest of K2
        for (int k = 16; k < (ciphers[i].size() / 2) - 1; k++) {
            bool consistent = true;
            
            // Check XOR results for consistency at this specific position
            for (int l = 0; l < 11; l++) {
                if (xor_pairs[l].size() < 2 * k) {
                    continue;
                }
                int xor_value = find(xor_pairs[l][2 * k]) * 16 + find(xor_pairs[l][2 * k + 1]);
                
                // Check if it's a valid character (either 'A' - 'Z' or 0 for space)
                if (xor_value != 0 && !(xor_value >= 'A' && xor_value <= 'Z')) {
                    consistent = false;
                    break;
                }
            }
            
            // If all XOR results are consistent, deduce that K2[k] is likely a space
            if (consistent) {
                K2[k - 16] = ' ';  // Deduce this position is a space for the i-th plaintext
                K2[k - 16] ^= (16 * find(ciphers[i][2 * k]) + find(ciphers[i][2 * k + 1])); // Deduce the key byte
            }
        }
    }

    /*
    1. I assumed the plaintexts contain no uppercase letters and always end with a period, as they are sentences.
    2. The code provided helped me deduce around 85 percent of Key2 and the corresponding plaintexts.
    3. Since message 9 is the longest, I focused on debugging it by forming meaningful sentences from all 12 plaintexts.
    4. Finding the word "mercy" was particularly challenging.
    5. I believe the keys and corresponding plaintexts I've derived are valid.
    */

    // Assuming m9 is the plaintext corresponding to the longest ciphertext
    string m9 = "hangman varnish gleamed on the newly refurbished gallows, a grim sight indeed that spoke volumes about the town's priorities and its callous approach to justice in an age that should have mercy.";

    // Write K2 and deduced plaintexts to the output file
    output_file << "K2 = ";

    for (int k = 0; k < K2_size; k++) {
        // Check if the index is within bounds of m9
        if (k + 16 < m9.size()) {
            // Deduce K2[k] based on the known plaintext m9
            K2[k] = m9[k + 16];
            K2[k] ^= find(ciphers[8][2 * (k + 16)]) * 16 + find(ciphers[8][2 * (k + 16) + 1]);
            
            // Process all ciphertexts to deduce the plaintext
            for (int i = 0; i < 12; i++) {
                if (ciphers[i].size() <= 2 * (k + 16)) {
                    continue;
                }
                int xor_value = find(ciphers[i][2 * (k + 16)]) * 16 + find(ciphers[i][2 * (k + 16) + 1]);
                xor_value ^= K2[k];
                plain_texts[i][k + 16] = xor_value;
            }
            
            // Write the hexadecimal representation of K2[k]
            output_file << get(K2[k] / 16) << get(K2[k] % 16);
        }
        else if (K2[k] != 256) {
            // Handle cases where K2[k] is already deduced
            for (int i = 0; i < 12; i++) {
                if (ciphers[i].size() <= 2 * (k + 16)) {
                    continue;
                }
                int xor_value = find(ciphers[i][2 * (k + 16)]) * 16 + find(ciphers[i][2 * (k + 16) + 1]);
                xor_value ^= K2[k];
                plain_texts[i][k + 16] = xor_value;
            }
            output_file << get(K2[k] / 16) << get(K2[k] % 16);
        }
        else {
            // Placeholder for unknown K2 values
            output_file << "_" << "_";
        }
    }

    // End the K2 line
    output_file << endl;

    // Output the plaintexts
    for (int i = 0; i < 12; i++) {
        output_file << "m" << i + 1 << " = ";
        for (int j = 0; j < plain_texts[i].size(); j++) {
            if (plain_texts[i][j] == 256) {
                // Placeholder for unknown plaintext characters
                output_file << "_";
            } else {
                // Output the actual plaintext character
                char c = plain_texts[i][j];
                output_file << c;
            }
        }
        output_file << endl;
    }

    return 0;
}
