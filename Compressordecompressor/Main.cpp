#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include "priority_queue.h"
#include "HashMap.h"
#include<cstdio>
#include "HashNode.h"
using namespace std;
struct Tree
{
    int frequency;
    unsigned char character;
    Tree* left = NULL;
    Tree* right = NULL;
};

class TreeComparator
{
public:
    bool operator()(Tree* a, Tree* b)
    {
        return a->frequency < b->frequency;
    }
};

template<typename K, typename V>
void print_map(const HashMap<K, V>& mp) {
    for (int i = 0; i < 7; i++) {
        auto ptr = mp.table[i];
        if (ptr == NULL)
            continue;
        while (ptr != NULL) {
            cout << ptr->key << " " << ptr->value << endl;
            ptr = ptr->next;
        }
    }
}


Tree* buildHuffmanTree(vector<pair<unsigned char, int> > freqtable)
{
    priority_queue<Tree*, vector<Tree*>, TreeComparator> huffqueue;
    for (int i = 0; i < freqtable.size(); i++)
    {
        Tree* node = new Tree();
        node->frequency = freqtable[i].second;
        node->character = freqtable[i].first;

        huffqueue.push(node);
    }

    std::cout << "Size Of Huffman Queue: " << huffqueue.size() << endl;
    while (huffqueue.size() > 1)
    {
        Tree* a, * b;
        a = huffqueue.top();
        huffqueue.pop();

        b = huffqueue.top();
        huffqueue.pop();
        Tree* c = new Tree();
        c->frequency = a->frequency + b->frequency;
        c->left = a;
        c->right = b;
        huffqueue.push(c);
    }
    return huffqueue.top();
}

string toBinary(unsigned  char a)
{
    string output = "";
    while (a != 0)
    {
        string bit = a % 2 == 0 ? "0" : "1";
        output += bit;
        a /= 2;
    }

    if (output.size() < 8)
    {
        int deficit = 8 - output.size();
        for (int i = 0; i < deficit; i++)
        {
            output += "0";
        }
    }

    reverse(output.begin(), output.end());
    return output;

}

void traverseHuffmanTree(Tree* root, string prev, string toAppend, HashMap<unsigned char, string>& codemap)
{

    prev += toAppend;
    // cout << "Traversing Done" << endl;
    if (root->right == NULL && root->left == NULL)
    {
        codemap.put(root->character, prev);
    }
    if (root->right != NULL)
    {
        traverseHuffmanTree(root->right, prev, "1", codemap);
    }

    if (root->left != NULL)
    {
        traverseHuffmanTree(root->left, prev, "0", codemap);
    }
}

unsigned char* readfilefrombuf(char* path, int& sz)
{
    FILE* fp = fopen(path, "rb");
    sz = 0;
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char* buffer = (unsigned char*)malloc(sz);
    fread(buffer, 1, sz, fp);
    // cout << "reading" << endl;
    fclose(fp);
    return buffer;
}

vector<pair<unsigned char, int> > conversion(HashMap<unsigned char, int>& codes)
{
    vector<pair<unsigned char, int> > codesV;

    /*for (int i = 0; i < codes.size(); ++i) {
        string ans = codes.find(i+1);
        int sz = ans.size();
        unsigned char c = ans[0];
        int s = std::stoi(ans.substr(2, sz - 1));
        codesv.push_back(make_pair(c,s));
    }*/

    //HashNode<unsigned char, int>** table;
    HashNode<unsigned char, int>* e = codes.table[0];
    // int size = codes.size();
    for (int bucket = 0; bucket < 7; bucket++) {
        auto ptr = codes.table[bucket];
        if (ptr == NULL)
            continue;
        while (ptr != NULL) {
            codesV.push_back(make_pair(ptr->key, ptr->value));
            ptr = ptr->next;
        }
    }

    return codesV;
}

string getHuffmanBitstring(unsigned char* buffer, HashMap<unsigned char, string>& codes, int sz, int& paddedBits)
{
    print_map(codes);
    string outputBuffer = "";
    for (int i = 0; i < sz; i++)
    {
         auto val = codes.search(buffer[i]);
         outputBuffer = outputBuffer + val;
        // outputBuffer = outputBuffer + codes[buffer[i]];
    }
    cout << outputBuffer.size() << endl;
    if (outputBuffer.size() % 8 != 0)
    {
        int deficit = 8 * ((outputBuffer.size() / 8) + 1) - outputBuffer.size();
        paddedBits = deficit;
        // cout << "Padded bits Before: " << paddedBits << endl;
        for (int i = 0; i < deficit; i++)
        {
            outputBuffer += "0";
        }
    }
    return outputBuffer;
}

unsigned char* getBufferFromString(string bitstring, vector<unsigned char>& outputBuffer, int& sz)
{
    int interval = 0;
    unsigned char bit = 0;

    for (int i = 0; i < sz; i++)
    {
        bit = (bit << 1) | (bitstring[i] - '0');

        interval++;
        if (interval == 8)
        {
            interval = 0;
            outputBuffer.push_back(bit);
            bit = 0;
        }
    }
    sz = outputBuffer.size();
    return outputBuffer.data();
}

string getStringFromBuffer(unsigned char* buffer, int sz)
{
    string bitstring = "";
    for (int i = 0; i < sz; i++)
    {
        bitstring += toBinary(buffer[i]);
    }

    return bitstring;
}

unsigned char* getDecodebuf(string bitstring, vector<unsigned char>& buffer, HashMap<unsigned char, string>& codes, int& sz, int paddedBits)
{
    string bit = "";
    HashMap<string, unsigned char> reversecodes;
    //for (int i = 0; i < codes.size(); ++i) {
    //    string ans = reversecodes.find(i + 1);
    //    for (int j = 0; j < ans.size(); ++j) {
    //        if (ans[i] == ',') {
    //            break;
    //        }
    //    }
    //    int sz = ans.size();
    //    /*string s = ans.substr(0, sz - 3);
    //    unsigned char c = ans[sz - 1];*/

    //    unsigned char c = ans[0];
    //    string s = ans.substr(2, sz - 1);

    //    // reversecodes.put(s,c);
    //}
    // HashNode<unsigned char, string>* e = codes.table[0];
    // int size = codes.size();
    for (int bucket = 0; bucket < 7; bucket++) {
        auto ptr = codes.table[bucket];
        if (ptr == NULL)
            continue;
        while (ptr != NULL) {
            reversecodes.put(ptr->value,ptr->key);
            ptr = ptr->next;
        }
    }
    int prev_cut = 0;
    for (int i = 0; i < bitstring.size() - paddedBits; i++)
    {
        bit += string(1, bitstring[i]);
        
        for (int bucket = 0; bucket < 7; bucket++) {
            auto ptr = reversecodes.table[bucket];
            if (ptr == NULL)
                continue;
            while (ptr != NULL) {
                if (ptr->key == bit) {
                    buffer.push_back(ptr->value);
                    bit = "";
                    break;
                }
                ptr = ptr->next;
            }
        }
    }

    sz = buffer.size();
    return buffer.data();
}

void writefilefrombuf(char* path, unsigned char* buffer, int sz, int flag)
{
    FILE* fp;
    if (flag == 0)
    {
        fp = fopen(path, "wb");
    }

    else {
        fp = fopen(path, "ab");
    }
    
    fwrite(buffer, 1, sz, fp);

    fclose(fp);
}

void writeHeader(char* path, HashMap<unsigned char, string>& codes, int paddedBits) {

    int size = codes.size();

    // Decoding me use aayega
    // Padded bits, size,   
    // for all map elements
    // A -> 1001
    // A41001 (Esa decode hoga file me)

    writefilefrombuf(path, (unsigned char*)&paddedBits, sizeof(int), 0);
    writefilefrombuf(path, (unsigned char*)&size, sizeof(int), 1);
    char nullBit = '\0';

    HashNode<unsigned char, string>* e = codes.table[0];
    for (int bucket = 0; bucket < 7; bucket++) {
        auto ptr = codes.table[bucket];
        if (ptr == NULL)
            continue;
        while (ptr != NULL) {
            writefilefrombuf(path, (unsigned char*)&ptr->key, 1, 1);
            auto len = ptr->value;
            int l = len.length();
            writefilefrombuf(path, (unsigned char*)&l, sizeof(int), 1);
            auto x = ptr->value;
            int x1 = x.length();
            writefilefrombuf(path, (unsigned char*)ptr->value.c_str(), x1, 1);
            ptr = ptr->next;
        }
    }
}

unsigned char* readHeader(unsigned char* buffer, HashMap<unsigned char, string>& codes, int& paddedBits, int& sz)
{
    paddedBits = *((int*)buffer);
    cout << paddedBits << "PADDED" << endl;
    buffer = buffer + 4;
    sz -= 4;
    int size = *((int*)buffer);
    buffer = buffer + 4;
    sz -= 4;
    for (int i = 0; i < size; i++)
    {
        unsigned char key = buffer[0];
        buffer++;
        sz--;
        int len = *((int*)buffer);
        buffer += 4;
        sz -= 4;
        char* value = (char*)malloc(len + 1);

        for (int j = 0; j < len; j++)
        {
            value[j] = buffer[j];
        }
        buffer += len;
        sz -= len;
        value[len] = '\0';

        codes.put(key, value);
        cout << key << " " << value << endl;
    }

    return buffer;
}


void compressionofFile(char* path, char* output_path, HashMap<unsigned char, string>& codes)
{
    int sz = 0;
    int paddedBits = 0;
    HashMap<unsigned char, int> freqtable;
    unsigned char* buffer = readfilefrombuf(path, sz);

    // cout << "Read File Into Buffer Done\n";
    for (int i = 0; i < sz; i++)
    {
        //freqtable[buffer[i]]++;
        /*int val = 0;
        if (freqtable.search(buffer[i]) == -1) {
            
        }
        else {
            val = freqtable.search(buffer[i]);
        }*/
        // freqtable.insert(buffer[i],i);
        int value = freqtable.search(buffer[i]);
        freqtable.put(buffer[i], value + 1);
    }
    // print_map(freqtable);
    Tree* root = buildHuffmanTree(conversion(freqtable));
    // cout << "Build HUffman Dn!\n";
    cout << root << endl;
    traverseHuffmanTree(root, "", "", codes);
    /*for (int bucket = 0; bucket < 7; bucket++) {
        auto ptr = codes.table[bucket];
        if (ptr == NULL)
            continue;
        while (ptr != NULL) {
            cout << "Key: " << ptr->key << endl;
            cout << "Value: " << ptr->value << endl;
            ptr = ptr->next;
        }
    }*/
    string outputString = getHuffmanBitstring(buffer, codes, sz, paddedBits);
    sz = outputString.size();
    vector<unsigned char> outputBufferV;
    getBufferFromString(outputString, outputBufferV, sz);
    unsigned char* outputBuffer = outputBufferV.data();
    writeHeader(output_path, codes, paddedBits);
    writefilefrombuf(output_path, outputBuffer, sz, 1);
}

void decompressionofFile(char* inputPath, char* outputPath)
{
    int sz = 0;
    HashMap<unsigned char, string> codes;
    int paddedBits = 0;
    unsigned char* fileBuffer = readfilefrombuf(inputPath, sz);
    fileBuffer = readHeader(fileBuffer, codes, paddedBits, sz);
    string fileBitString = getStringFromBuffer(fileBuffer, sz);
    // cout<<fileBitString<<endl;
    vector<unsigned char> outputBufferV;
    unsigned char* outputBuffer;
    getDecodebuf(fileBitString, outputBufferV, codes, sz, paddedBits);
    outputBuffer = outputBufferV.data();
    writefilefrombuf(outputPath, outputBuffer, sz, 0);
}

int main(int argc, char* argv[])
{
    char* datafile = NULL, * encodedfile = NULL, * decodedfile = NULL;

    if (argc == 4)
    {
        datafile = argv[1];
        encodedfile = argv[2];
        decodedfile = argv[3];
    }

    HashMap<unsigned char, string> codes;
    compressionofFile(datafile, encodedfile, codes);
    decompressionofFile(encodedfile, decodedfile);
}
