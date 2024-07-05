A Project on Text File Compression and Decompression using Huffman Coding in C++

Huffman Coding is a lossless data compression algorithm. The key idea is to use variable-length codes for encoding characters, where more frequent characters get shorter codes and less frequent characters get longer codes. This is achieved using a binary tree (Huffman tree).

Key Functionalities

1. Frequency Calculation: Compute the frequency of each character in the input text.
2. Heap Construction: Build a min-heap of nodes where each node contains a character and its frequency.
3. Huffman Tree Construction: Construct a Huffman tree from the min-heap.
4. Code Generation: Generate Huffman codes for each character based on the Huffman tree.
5. Text Encoding: Encode the input text using the generated Huffman codes.
6. Padding and Byte Conversion: Pad the encoded text to ensure its length is a multiple of 8, then convert it to a byte array.
7. File Writing: Write the byte array to a binary file.
8. Text Decoding: Decode the binary file back to the original text by reversing the above steps.

Process Flow

1. Compression:
   - Read the input file and calculate character frequencies.
   - Build a min-heap from the frequency dictionary.
   - Construct the Huffman tree from the heap.
   - Generate Huffman codes by traversing the Huffman tree.
   - Encode the input text using the Huffman codes.
   - Pad the encoded text and convert it to a byte array.
   - Write the byte array to a binary file.

2. Decompression:
   - Read the binary file and convert it to a bit string.
   - Remove the padding from the bit string.
   - Decode the bit string to the original text using the reverse mapping of Huffman codes.
   - Write the decoded text to an output file.

Some test cases and their results -->
Original File Size | Compressed File Size
129 B              | 73 B
18 KB              | 10 KB
33 KB              | 19 KB
68 KB              | 39 KB
180 KB             | 104 KB
280 KB             | 160 KB

We get an average of 45% compression on varying file sizes.

The test cases are stored in "Input" Folder, the resulting compressed bin files are stored in "Binary Compressed Files" Folder, and the decompressed files from the compressed ones are stored in "Decompressed Files" folder.

This implementation efficiently compresses and decompresses text files using the Huffman coding algorithm, leveraging binary trees, heaps, and hash maps for optimal performance.
