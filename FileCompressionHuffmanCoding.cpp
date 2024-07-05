#include <iostream>
#include<bits/stdc++.h>
using namespace std;

//A class for Binary tree
class BinaryTree {
public:
    char value;
    int freq;
    BinaryTree* left;
    BinaryTree* right;

    BinaryTree(char val, int f) 
    {
        value = val;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    bool operator<(const BinaryTree& other) const 
    {
        return freq > other.freq;
    }

    bool operator==(const BinaryTree& other) const 
    {
        return freq == other.freq;
    }
};

//A class for HuffmanCode, most of the functionalities including compression and decompression reside here
class Huffmancode {
private:
    string path;
    vector<BinaryTree*> heap;
    unordered_map<char, string> code;
    unordered_map<string, char> reverseCode;

    //Function for storing frequency of each letter of text passed as an argument
    unordered_map<char, int> __frequency_from_text(const string& text) 
    {
        unordered_map<char, int> freq_dict;
        for (char c : text) 
        {
            if (freq_dict.find(c) == freq_dict.end()) 
                freq_dict[c] = 1;
            else 
                freq_dict[c] += 1;
        }
        return freq_dict;
    }

    //Function for creating a min heap
    void __Build_heap(const unordered_map<char, int>& frequency_dict) 
    {
        for (auto entry : frequency_dict) 
        {
            char key = entry.first;
            int frequency = entry.second;
            BinaryTree* binary_tree_node = new BinaryTree(key, frequency);
            heap.push_back(binary_tree_node);
        }
        make_heap(heap.begin(), heap.end(), [](const BinaryTree* a, const BinaryTree* b) { return *a < *b; });
    }

    //Function for creating a binary tree from the min heap
    void __Build_Binary_Tree() 
    {
        while (heap.size() > 1) 
        {
            BinaryTree* binary_tree_node1 = heap.front();
            pop_heap(heap.begin(), heap.end(), [](const BinaryTree* a, const BinaryTree* b) { return *a < *b; });
            heap.pop_back();

            BinaryTree* binary_tree_node2 = heap.front();
            pop_heap(heap.begin(), heap.end(), [](const BinaryTree* a, const BinaryTree* b) { return *a < *b; });
            heap.pop_back();

            int sum_of_frequency = binary_tree_node1->freq + binary_tree_node2->freq;
            BinaryTree* newnode = new BinaryTree('\0', sum_of_frequency);
            newnode->left = binary_tree_node1;
            newnode->right = binary_tree_node2;

            heap.push_back(newnode);
            push_heap(heap.begin(), heap.end(), [](const BinaryTree* a, const BinaryTree* b) { return *a < *b; });
        }
    }

    //Helper for __Build_Binary_Code
    void __Build_Binary_Code_Helper(BinaryTree* root, string curr_bits) 
    {
        if (root == nullptr)
            return;

        if (root->value != '\0') 
        {
            code[root->value] = curr_bits;
            reverseCode[curr_bits] = root->value;
            return;
        }
        __Build_Binary_Code_Helper(root->left, curr_bits + "0");
        __Build_Binary_Code_Helper(root->right, curr_bits + "1");
    }

    //Function to assign Binary code to the characters
    void __Build_Binary_Code() 
    {
        BinaryTree* root = heap.front();
        __Build_Binary_Code_Helper(root, "");
    }

    //Function for creating code from the text
    string __Build_encoded_text(const string& text) 
    {
        string encoded_text = "";
        for (char c : text)
            encoded_text += code[c];
        return encoded_text;
    }

    //Function for padding the encoded text
    string __Build_Padded_Text(string& encoded_text) 
    {
        int padding_value = 8 - encoded_text.length() % 8;
        for (int i = 0; i < padding_value; i++)
            encoded_text+= "0";

        string padded_info = bitset<8>(padding_value).to_string();
        string padded_text = padded_info + encoded_text;
        return padded_text;
    }

    //Function for converting the padded text as final array(which will be created as a .bin file in compression function)
    vector<unsigned char> _Build_Bytes_Array(const string& padded_text) 
    {
        vector<unsigned char> array;
        for (size_t i = 0; i < padded_text.length(); i += 8) 
        {
            string byte = padded_text.substr(i, 8);
            array.push_back(stoul(byte, nullptr, 2));
        }
        return array;
    }

    //Function to remove the padding from the text
    string __Remove_Padding(const string& text) 
    {
        string padded_info = text.substr(0, 8);
        int padded_value = stoi(padded_info, nullptr, 2);
        string text_without_padding = text.substr(8);
        text_without_padding.erase(text_without_padding.end() - padded_value, text_without_padding.end());
        return text_without_padding;
    }

    //Function for decoding the text using reverseCode
    string __Decoded_text(const string& text) 
    {
        string curr_bit = "";
        string decoded_text = "";
        for (char bit : text) 
        {
            curr_bit += bit;
            if (reverseCode.find(curr_bit) != reverseCode.end()) 
            {
                decoded_text += reverseCode[curr_bit];
                curr_bit = "";
            }
        }
        return decoded_text;
    }
    /*
    void printMap()
    {
        for(auto it : code)
            cout << it.first << ":" << it.second << endl;
        for(auto it : reverseCode)
            cout << it.first << ":" << it.second << endl;
    }
    */
public:
    Huffmancode(const string& p) 
    {
        path = p;
    }

    string compression() 
    {
        // To access the file and extract text from that file
        cout << "Compressing..." << endl;
        string filename = path.substr(0, path.find_last_of('.'));
        string output_path = filename + ".bin";
        ifstream file(path);
        ofstream output(output_path, ios::binary);
        string text((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));

        //Storing frequency of each letter in dictionary
        unordered_map<char, int> frequency_dict = __frequency_from_text(text);

        //Building min heap to store the frequency stored in dictionary
        __Build_heap(frequency_dict);

        //Contruct Binary tree from heap
        __Build_Binary_Tree();

        //Assign Binary code to the characters
        __Build_Binary_Code();

        //Creating code from text
        string encoded_text = __Build_encoded_text(text);

        //Padding of encoded text
        string padded_text = __Build_Padded_Text(encoded_text);

        //Returning the padded value as a file
        vector<unsigned char> bytes_array = _Build_Bytes_Array(padded_text);
        output.write(reinterpret_cast<const char*>(&bytes_array[0]), bytes_array.size());
        output.close();

        cout << "Compressed Successfully." << endl;
        //printMap();
        return output_path;
    }

    string decompression(const string& input_path) 
    {
        string filename = input_path.substr(0, input_path.find_last_of('.'));
        string output_path = filename + "_decompressed.txt";
        ifstream file(input_path, ios::binary);
        ofstream output(output_path);

        string bit_string;
        unsigned char byte;
        while (file.read(reinterpret_cast<char*>(&byte), sizeof(byte)))
            bit_string += bitset<8>(byte).to_string();

        string text_after_removing_padding = __Remove_Padding(bit_string);
        string actual_text = __Decoded_text(text_after_removing_padding);
        output << actual_text;
        output.close();

        return output_path;
    }
};

int main() 
{
    string path;
    //Taking file name as input
    cout << "Enter Path to the file to be compressed: ";
    getline(cin, path);
    Huffmancode h(path);
    string compressed_file = h.compression();
    string decompressed_file = h.decompression(compressed_file);
    return 0;
}