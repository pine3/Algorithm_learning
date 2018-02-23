#include <iostream>
#include <vector>
#include <queue>

struct Answers {
    Answers(int start_, int size_, std::string pattern_)
    : start(start_), size(size_), pattern(pattern_) {}
    int start;
    int size;
    std::string pattern;
};

struct TrieVertex {
    TrieVertex() : children('z' - 'a', nullptr) {}
    TrieVertex* border;
    TrieVertex* border_pattern = nullptr;
    std::vector<TrieVertex*> children;
    std::string word;
    bool ispattern = false;
};

class Trie {
public: 
    void Initialize(const std::vector<std::string>& patterns);
    void GoToNextBoard(char ch);
    std::vector<Answers> GetPatterns();
    void Print();
private:
    int step = 0;
    TrieVertex* root;
    TrieVertex* common_part;
};

std::vector<Answers> AhoCorasick(const std::string& text, const std::vector<std::string>& patterns) {
    Trie trie;
    trie.Initialize(patterns);
    std::vector<Answers> res;
    for (auto ch : text) {
        trie.GoToNextBoard(ch);
        for (auto i : trie.GetPatterns()) {
            res.push_back(i);
        }
    }
    return res; 
}

void Trie::Initialize(const std::vector<std::string>& patterns) {
    root = new TrieVertex;
    root->word = std::string();
    common_part = root;
    TrieVertex* now;
    std::string word;
    for (auto pattern : patterns) {
        word = std::string();
        now = root;
        for (auto symbol : pattern) {
            word += symbol;
            if (now->children[symbol - 'a'] == nullptr) {
                now->children[symbol - 'a'] = new TrieVertex;
                now->children[symbol - 'a']->word = word;
            }
            now = now->children[symbol - 'a'];
        }
        now->ispattern = true;
    }
    std::queue<TrieVertex*> bfs;
    for (auto vertex : root->children) {
        if (vertex != nullptr) {
            bfs.push(vertex);
            vertex->border = root;
        }
    }
    TrieVertex* patterns_way;
    while (!bfs.empty()) {
        now = bfs.front();
        bfs.pop();
        for (auto vertex : now->children) {
            if (vertex != nullptr) {
                bfs.push(vertex);
                patterns_way = now->border;
                while ((patterns_way->children[*(vertex->word.end() - 1) - 'a'] == nullptr) && (patterns_way != root)) {
                    patterns_way = patterns_way->border;
                }
                if (patterns_way->children[*(vertex->word.end() - 1) - 'a'] != nullptr) {
                    vertex->border = patterns_way->children[*(vertex->word.end() - 1) - 'a'];
                } else {
                    vertex->border = root;
                }
            }
        }
        if (now->border->ispattern) {
            now->border_pattern = now->border;
        } else {
            now->border_pattern = now->border->border_pattern;
        }
    }
    return;
}

void Trie::GoToNextBoard(char ch) {
    while ((common_part->children[ch - 'a'] == nullptr) && (common_part != root)) {
        common_part = common_part->border;
    }
    if (common_part->children[ch - 'a'] != nullptr) {
        common_part = common_part->children[ch - 'a'];
    }
    ++step;
    return;
}

std::vector<Answers> Trie::GetPatterns() {
    std::vector<Answers> res;
    if (common_part->ispattern) {
        res.push_back(Answers(step - common_part->word.size(), common_part->word.size(), common_part->word));
    }
    TrieVertex* now = common_part->border_pattern;
    while (now != nullptr) {
        res.push_back(Answers(step - now->word.size(), now->word.size(), now->word));
        now = now->border_pattern;
    }
    return res;
}

void Trie::Print() {
    std::cout << std::endl;
    std::queue<TrieVertex*> bfs;
    for (auto vertex : root->children) {
        if (vertex != nullptr) {
            bfs.push(vertex);
        }
    }
    TrieVertex* now;
    while (!bfs.empty()) {
        now = bfs.front();
        bfs.pop();
        std::cout << now->word << " -> " << now->border->word << std::endl;
        for (auto vertex : now->children) {
            if (vertex != nullptr) {
                bfs.push(vertex);
            }
        }
    }
}

int main() {
    int number_patterns;
    std::cin >> number_patterns;
    std::vector<std::string> patterns(number_patterns);
    for (int i = 0; i < number_patterns; ++i) {
        std::cin >> patterns[i];
    }
    std::string text;
    std::cin >> text;
    std::cout << AhoCorasick(text, patterns).size() << '\n';
    return 0;
}
