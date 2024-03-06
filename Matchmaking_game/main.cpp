#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Structura pentru nodurile Trie
struct TrieNode {
    unordered_map<char, TrieNode*> children; // Map pentru a stoca copiii nodului
    bool isEndOfWord; // Flag pentru a indica sfârșitul unui cuvânt
    int wordCount; // Contor pentru a ține evidența numărului de cuvinte care folosesc acest nod

    TrieNode() : isEndOfWord(false), wordCount(0) {}
};

// Clasa Trie
class Trie {
private:
    TrieNode* root; // Rădăcina Trie

public:
    Trie() : root(new TrieNode()) {}

    void insert(const string& word); // Funcție pentru a insera un cuvânt în Trie
    bool search(const string& word); // Funcție pentru a căuta un cuvânt în Trie
    void remove(const string& word); // Funcție pentru a elimina un cuvânt din Trie

    TrieNode* getRoot() const { // Funcție pentru a obține rădăcina Trie
        return root;
    }

    // Funcție pentru a găsi cuvinte în grilă începând cu un anumit nod
    void findWordsInGrid(vector<vector<char>>& grid, int i, int j, TrieNode* node, string currentWord, vector<string>& foundWords, vector<vector<bool>>& visited);
};

// Funcție pentru a verifica dacă un index este valid
bool isValidIndex(int i, int j, int rows, int cols) {
    return (i >= 0 && i < rows && j >= 0 && j < cols);
}

// Funcție pentru a găsi cuvinte în grilă începând cu un anumit nod
void Trie::findWordsInGrid(vector<vector<char>>& grid, int i, int j, TrieNode* node, string currentWord, vector<string>& foundWords, vector<vector<bool>>& visited) {
    if (node == nullptr) {
        return;
    }

    if (node->isEndOfWord) {
        // Verificăm dacă cuvântul a fost deja găsit
        if (find(foundWords.begin(), foundWords.end(), currentWord) != foundWords.end()) {
            cout << "Cuvantul '" << currentWord << "' a fost deja gasit.\n";
        } else {
            foundWords.push_back(currentWord);
            node->isEndOfWord = false;
            cout << "Cuvant gasit: " << currentWord << endl;
            this->remove(currentWord);
        }
    }

    

    for (int k = 0; k < 4; ++k) {
        int ni = i + row[k];
        int nj = j + col[k];

        if (isValidIndex(ni, nj, grid.size(), grid[0].size()) && !visited[ni][nj]) {
            char nextChar = grid[ni][nj];

            if (node->children.find(nextChar) != node->children.end()) {
                findWordsInGrid(grid, ni, nj, node->children[nextChar], currentWord + nextChar, foundWords, visited);
            }
        }
    }

    visited[i][j] = false;
}



// Funcție helper pentru a elimina un cuvânt din Trie
bool removeHelper(TrieNode* node, const string& word, int index) {
    if (index == word.size()) {
        if (!node->isEndOfWord) {
            return false;
        }
        node->isEndOfWord = false;
        node->wordCount--;
        return node->wordCount == 0;
    }

    char ch = word[index];
    auto it = node->children.find(ch);
    if (it == node->children.end()) {
        return false;
    }

    bool shouldDeleteCurrentNode = removeHelper(it->second, word, index + 1);

    if (shouldDeleteCurrentNode) {
        delete it->second;
        node->children.erase(it);
        node->wordCount--;
        return node->wordCount == 0;
    }

    return false;
}

// Funcție pentru a elimina un cuvânt din Trie
void Trie::remove(const string& word) {
    removeHelper(root, word, 0);
}

// Funcție pentru a insera un cuvânt în Trie
void Trie::insert(const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = new TrieNode();
        }
        node = node->children[ch];
        node->wordCount++;
    }
    node->isEndOfWord = true;
}

// Funcție pentru a căuta un cuvânt în Trie
bool Trie::search(const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        if (node->children.find(ch) == node->children.end()) {
            return false;
        }
        node = node->children[ch];
    }
    return node != nullptr && node->isEndOfWord;
}

// Funcție pentru a începe jocul de căutare a cuvintelor
void wordSearchGame(vector<vector<char>>& grid, Trie& trie, vector<string>& words, int level);

// Funcție pentru a afișa grila

void printGrid(vector<vector<char>>& grid) {
    cout << "Grid:\n";
    for (const vector<char>& row : grid) {
        for (char ch : row) {
            cout << setw(3) << ch;
        }
        cout << "\n";
    }
}

int main() {
    // Nivelul 1
    vector<vector<char>> grid1 = {
        {'c', 'a', 't', 'd', 'e'},
        {'r', 'a', 't', 'f', 'g'},
        {'b', 'a', 't', 'h', 'i'},
        {'j', 'k', 'l', 'm', 'n'},
        {'o', 'p', 'q', 'r', 's'}
    };

    Trie trie1;
    vector<string> words1 = {"cat", "bat", "rat"};

    cout << "Incepe nivelul 1!\n";
    wordSearchGame(grid1, trie1, words1, 1);

    // Nivelul 2
    vector<vector<char>> grid2 = {
        {'c', 'a', 't', 'd', 'e', 'f', 'g', 'h', 'i', 'j'},
        {'r', 'a', 't', 'f', 'g', 'h', 'i', 'j', 'k', 'l'},
        {'b', 'a', 't', 'h', 'i', 'j', 'k', 'l', 'm', 'n'},
        {'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's'},
        {'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x'},
        {'t', 'u', 'v', 'w', 'x', 'y', 'z', 'a', 'b', 'c'},
        {'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'},
        {'d', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'},
        {'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r'},
        {'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w'}
    };

    Trie trie2;
    vector<string> words2 = {"cat", "bat", "rat", "dog", "fox", "lion", "tiger", "bear"};

    cout << "Felicitari! Ai terminat nivelul 1. Acum treci la nivelul 2!\n";
    wordSearchGame(grid2, trie2, words2, 2);

    return 0;
}

void wordSearchGame(vector<vector<char>>& grid, Trie& trie, vector<string>& words, int level) {
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
    int score = 0;
    int points = (level == 1) ? 10 : 30; // Punctele variază în funcție de nivel

    for (const string& word : words) {
        trie.insert(word);
    }

    vector<string> foundWords;

    auto start = high_resolution_clock::now();

    do {
        printGrid(grid);

        auto now = high_resolution_clock::now();
        auto elapsed = duration_cast<seconds>(now - start);
        cout << "Timp ramas: " << 60 - elapsed.count() << " secunde\n";
        
        int i, j;
        while (true) {
            cout << "Introdu coordonatele (i j) ale primei litere a cuvantului (sau -1 pentru a iesi): ";
            cin >> i >> j;

            if (i == -1 || j == -1) {
                cout << "Ai ales sa iesi din joc. La revedere!\n";
                return;
            } else if (i > grid.size() || j > grid[0].size()) {
                cout << "Coordonate invalide. Te rog sa introduci din nou.\n";
            } else {
                break;
            }
        }

        --i;
        --j;

        if (isValidIndex(i, j, grid.size(), grid[0].size())) {
            char startChar = grid[i][j];
            TrieNode* startNode = trie.getRoot()->children[startChar];
            trie.findWordsInGrid(grid, i, j, startNode, string(1, startChar), foundWords, visited);
            
            if (!foundWords.empty() && find(words.begin(), words.end(), foundWords.back()) != words.end()) {
                auto it = find(foundWords.begin(), foundWords.end(), foundWords.back());
                if (it == foundWords.end() - 1) {
                    score += points * foundWords.back().size();
                    cout << "Cuvant gasit! Punctaj: " << score << endl;
                } else {
                    cout << "Cuvantul '" << foundWords.back() << "' a fost deja gasit.\n";
                }
            } else {
                cout << "Nicio potrivire gasita de la acea pozitie.\n";
            }
        } else {
            cout << "Coordonate invalide.\n";
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(stop - start);

        if (duration.count() > 60) {
            cout << "Timpul a expirat!\n";
            break;
        }
        if (foundWords.size() == words.size()) {
            cout << "Toate cuvintele au fost gasite!\n";
            break;
        }

    } while (true);

    cout << "Nivelul " << level << " terminat. Cuvinte gasite:\n";
    for (const string& word : foundWords) {
        cout << word << "\n";
    }

    cout << "Punctaj final nivel " << level << ": " << score << endl;
}


