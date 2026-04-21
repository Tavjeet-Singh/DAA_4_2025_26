#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

struct HuffNode {
    unsigned char ch;
    long long freq;
    int id;
    HuffNode* left;
    HuffNode* right;
    HuffNode(unsigned char c, long long f, int i)
        : ch(c), freq(f), id(i), left(nullptr), right(nullptr) {}
    HuffNode(long long f, int i, HuffNode* l, HuffNode* r)
        : ch(0), freq(f), id(i), left(l), right(r) {}
};

struct Compare {
    bool operator()(HuffNode* a, HuffNode* b) {
        if (a->freq != b->freq) return a->freq > b->freq;
        return a->id > b->id;
    }
};

HuffNode* buildTree(const std::map<unsigned char, long long>& freq) {
    std::priority_queue<HuffNode*, std::vector<HuffNode*>, Compare> pq;
    int id = 0;
    for (auto& [ch, f] : freq)
        pq.push(new HuffNode(ch, f, id++));

    if (pq.size() == 1) {
        HuffNode* leaf = pq.top(); pq.pop();
        return new HuffNode(leaf->freq, id++, leaf, nullptr);
    }
    while (pq.size() > 1) {
        HuffNode* left  = pq.top(); pq.pop();
        HuffNode* right = pq.top(); pq.pop();
        pq.push(new HuffNode(left->freq + right->freq, id++, left, right));
    }
    return pq.top();
}

void generateCodes(HuffNode* node, const std::string& prefix,
                   std::map<unsigned char, std::string>& codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->ch] = prefix.empty() ? "0" : prefix;
        return;
    }
    generateCodes(node->left,  prefix + "0", codes);
    generateCodes(node->right, prefix + "1", codes);
}

void freeTree(HuffNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void writeBits(std::ofstream& out, const std::string& bits, uint8_t& buffer, int& bitCount) {
    for (char b : bits) {
        buffer = (uint8_t)((buffer << 1) | (b - '0'));
        bitCount++;
        if (bitCount == 8) { out.put((char)buffer); buffer = 0; bitCount = 0; }
    }
}

bool compress(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream in(inputPath, std::ios::binary);
    if (!in) { std::cerr << "Cannot open: " << inputPath << "\n"; return false; }
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(in)), {});
    in.close();
    if (data.empty()) { std::cerr << "Input file is empty.\n"; return false; }

    std::map<unsigned char, long long> freq;
    for (unsigned char c : data) freq[c]++;

    HuffNode* root = buildTree(freq);
    std::map<unsigned char, std::string> codes;
    generateCodes(root, "", codes);
    freeTree(root);

    std::ofstream out(outputPath, std::ios::binary);
    if (!out) { std::cerr << "Cannot open: " << outputPath << "\n"; return false; }

    uint32_t uniqueChars = (uint32_t)freq.size();
    out.write(reinterpret_cast<char*>(&uniqueChars), sizeof(uniqueChars));
    for (auto& [ch, f] : freq) {
        out.put((char)ch);
        uint64_t fv = (uint64_t)f;
        out.write(reinterpret_cast<char*>(&fv), sizeof(fv));
    }
    uint64_t totalBytes = (uint64_t)data.size();
    out.write(reinterpret_cast<char*>(&totalBytes), sizeof(totalBytes));

    uint8_t buffer   = 0;
    int     bitCount = 0;
    for (unsigned char c : data) writeBits(out, codes[c], buffer, bitCount);
    if (bitCount > 0) { buffer = (uint8_t)(buffer << (8 - bitCount)); out.put((char)buffer); }
    out.close();

    long long inSize = (long long)data.size();
    std::ifstream check(outputPath, std::ios::binary | std::ios::ate);
    long long outSize = check.tellg();
    double change = 100.0 * ((double)outSize / inSize - 1.0);
    std::cout << "Compressed: " << inSize << " -> " << outSize << " bytes";
    if (change <= 0) std::cout << "  [" << -change << "% smaller]\n";
    else             std::cout << "  [+" << change << "% larger — file may already be dense]\n";
    return true;
}

bool decompress(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream in(inputPath, std::ios::binary);
    if (!in) { std::cerr << "Cannot open: " << inputPath << "\n"; return false; }

    uint32_t uniqueChars;
    in.read(reinterpret_cast<char*>(&uniqueChars), sizeof(uniqueChars));
    if (!in || uniqueChars == 0 || uniqueChars > 256) {
        std::cerr << "Invalid header.\n"; return false;
    }

    std::map<unsigned char, long long> freq;
    for (uint32_t i = 0; i < uniqueChars; i++) {
        unsigned char ch = (unsigned char)in.get();
        uint64_t f;
        in.read(reinterpret_cast<char*>(&f), sizeof(f));
        freq[ch] = (long long)f;
    }

    uint64_t totalBytes;
    in.read(reinterpret_cast<char*>(&totalBytes), sizeof(totalBytes));

    std::vector<unsigned char> compressed((std::istreambuf_iterator<char>(in)), {});
    in.close();

    HuffNode* root = buildTree(freq);
    std::ofstream out(outputPath, std::ios::binary);
    if (!out) { std::cerr << "Cannot open: " << outputPath << "\n"; return false; }

    HuffNode* cur    = root;
    uint64_t  decoded = 0;

    for (unsigned char byte : compressed) {
        for (int bit = 7; bit >= 0; bit--) {
            if (decoded >= totalBytes) goto done;
            int b = (byte >> bit) & 1;
            cur = b ? cur->right : cur->left;
            if (!cur) { cur = root; continue; }
            if (!cur->left && !cur->right) {
                out.put((char)cur->ch);
                decoded++;
                cur = root;
            }
        }
    }
done:
    out.close();
    freeTree(root);

    if (decoded != totalBytes) {
        std::cerr << "Warning: expected " << totalBytes << " bytes, decoded " << decoded << "\n";
        return false;
    }
    std::cout << "Decompressed: " << decoded << " bytes -> " << outputPath << "\n";
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage:\n";
        std::cout << "  huffman compress   <input.txt>  <output.huf>\n";
        std::cout << "  huffman decompress <input.huf>  <output.txt>\n";
        return 1;
    }
    std::string mode = argv[1], input = argv[2], output = argv[3];
    if      (mode == "compress")   return compress(input, output)   ? 0 : 1;
    else if (mode == "decompress") return decompress(input, output) ? 0 : 1;
    else { std::cerr << "Unknown mode: " << mode << "\n"; return 1; }
}
