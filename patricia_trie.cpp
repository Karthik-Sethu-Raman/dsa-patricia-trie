#include <iostream>
#include <map>
using namespace std;

class PatriciaTrie {

private:

    struct Node {
        map<string, Node*> children;
        bool isEnd;

        Node() {
            isEnd = false;
        }
    };

    Node* root;

    // ===== Longest Common Prefix =====
    string getLCP(string a, string b) {
        int i = 0;
        while (i < a.length() && i < b.length() && a[i] == b[i])
            i++;
        return a.substr(0, i);
    }

    // ===== Recursive Search =====
    bool searchRecursive(Node* current, string word) {

        if (word.empty())
            return current->isEnd;

        for (auto it : current->children) {
            string edge = it.first;

            if (word.find(edge) == 0) {
                string remaining = word.substr(edge.length());
                return searchRecursive(it.second, remaining);
            }
        }

        return false;
    }

    // ===== Display Trie =====
    void display(Node* current, string indent) {

        for (auto it : current->children) {

            cout << indent << "|-- " << it.first;

            if (it.second->isEnd)
                cout << " (END)";

            cout << endl;

            display(it.second, indent + "    ");
        }
    }

    // ===== Delete Helper =====
    bool deleteHelper(Node* current, string word) {

        if (word.empty()) {
            if (!current->isEnd)
                return false;

            current->isEnd = false;

            return current->children.empty();
        }

        for (auto it = current->children.begin();
             it != current->children.end(); it++) {

            string edge = it->first;

            if (word.find(edge) == 0) {

                string remaining = word.substr(edge.length());

                bool shouldDelete =
                    deleteHelper(it->second, remaining);

                if (shouldDelete) {
                    delete it->second;
                    current->children.erase(it);
                }

                return current->children.empty() && !current->isEnd;
            }
        }

        return false;
    }

public:

    PatriciaTrie() {
        root = new Node();
    }

    // ===== Insert =====
    void insert(string word) {

        Node* current = root;

        while (!word.empty()) {

            bool matched = false;

            for (auto it = current->children.begin();
                 it != current->children.end(); it++) {

                string edge = it->first;
                string lcp = getLCP(word, edge);

                if (lcp.empty())
                    continue;

                matched = true;

                // Full match
                if (lcp == edge) {
                    word = word.substr(lcp.length());
                    current = it->second;
                    break;
                }

                // Partial match → split
                string oldRemain = edge.substr(lcp.length());
                string newRemain = word.substr(lcp.length());

                Node* oldChild = it->second;
                Node* splitNode = new Node();

                current->children.erase(edge);
                current->children[lcp] = splitNode;

                splitNode->children[oldRemain] = oldChild;

                if (!newRemain.empty()) {
                    Node* newNode = new Node();
                    newNode->isEnd = true;
                    splitNode->children[newRemain] = newNode;
                } else {
                    splitNode->isEnd = true;
                }

                return;
            }

            // No match → direct insert
            if (!matched) {
                Node* newNode = new Node();
                newNode->isEnd = true;
                current->children[word] = newNode;
                return;
            }
        }

        current->isEnd = true;
    }

    // ===== Iterative Search =====
    bool search(string word) {

        Node* current = root;

        while (!word.empty()) {

            bool found = false;

            for (auto it : current->children) {

                string edge = it.first;

                if (word.find(edge) == 0) {
                    word = word.substr(edge.length());
                    current = it.second;
                    found = true;
                    break;
                }
            }

            if (!found)
                return false;
        }

        return current->isEnd;
    }

    // ===== Recursive Search Wrapper =====
    bool searchRec(string word) {
        return searchRecursive(root, word);
    }

    // ===== Delete =====
    void remove(string word) {
        deleteHelper(root, word);
    }

    // ===== Show =====
    void show() {
        cout << "\nPatricia Trie Structure:\n";
        display(root, "");
    }
};



// ================= MAIN FUNCTION =================
int main() {

    PatriciaTrie trie;

    // Insert words
    trie.insert("car");
    trie.insert("card");
    trie.insert("care");
    trie.insert("dog");

    cout << "Inserted words: car, card, care, dog\n";

    // Display structure
    trie.show();

    // Search demo
    cout << "\nSearch Results:\n";
    cout << "car  : " << (trie.search("car") ? "Found" : "Not Found") << endl;
    cout << "care : " << (trie.search("care") ? "Found" : "Not Found") << endl;
    cout << "cat  : " << (trie.search("cat") ? "Found" : "Not Found") << endl;

    // Delete demo
    cout << "\nDeleting 'card'...\n";
    trie.remove("card");

    trie.show();

    return 0;
}