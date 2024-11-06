#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

// A node of the Huffman tree
struct HuffmanNode
{
    char data;
    int freq;
    HuffmanNode *left, *right;

    HuffmanNode(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

struct com{
    bool operator()(HuffmanNode *l, HuffmanNode *r){
        return l->freq > r->freq;
    }
};

 

// Recursive function to generate Huffman codes
void generateCodes(HuffmanNode *root, string code, unordered_map<char, string> &huffmanCodes)
{
    if (!root)
        return;

    // Leaf node contains a character
    if (!root->left && !root->right)
    {
        huffmanCodes[root->data] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

// Main function to build the Huffman Tree and generate codes
void huffmanEncoding(const unordered_map<char, int> &freq)
{
    // Create a priority queue to store nodes of the Huffman tree
    priority_queue<HuffmanNode *, vector<HuffmanNode *>, com> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto pair : freq)
    {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Combine nodes until there is only one node left in the queue
    while (pq.size() > 1)
    {
        HuffmanNode *left = pq.top();
        pq.pop();
        HuffmanNode *right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        HuffmanNode *newNode = new HuffmanNode('\0', sum);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    // The root of the Huffman Tree
    HuffmanNode *root = pq.top();

    // Generate the Huffman codes
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Display the codes
    cout << "Huffman Codes:\n";
    for (auto pair : huffmanCodes)
    {
        cout << pair.first << " : " << pair.second << endl;
    }
}

// Utility function to calculate frequency of characters in a string
unordered_map<char, int> calculateFrequency(const string &text)
{
    unordered_map<char, int> freq;
    for (char ch : text)
    {
        freq[ch]++;
    }
    return freq;
}

int main()
{
    string text;
    cout << "Enter text to encode: ";
    getline(cin, text);

    unordered_map<char, int> freq = calculateFrequency(text);
    huffmanEncoding(freq);

    return 0;
}
