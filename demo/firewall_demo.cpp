#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

class PatriciaNode {
public:
    string key;                 // Compressed bits
    bool is_terminal;           // Is this a rule endpoint?
    string data;                // Action (ALLOW / BLOCK)
    unordered_map<char, PatriciaNode*> children;

    PatriciaNode(string k = "", bool terminal = false, string d = "")
        : key(k), is_terminal(terminal), data(d) {}
};

class FirewallRadixTree {
private:
    PatriciaNode* root;

    string getCommonPrefix(const string& s1, const string& s2) {
        size_t minLen = min(s1.length(), s2.length());
        size_t i = 0;
        while (i < minLen && s1[i] == s2[i]) {
            i++;
        }
        return s1.substr(0, i);
    }

public:
    FirewallRadixTree() {
        root = new PatriciaNode();
    }

    void insert(string ip_binary, string action) {
        PatriciaNode* node = root;
        string path = ip_binary;

        while (true) {
            bool matched = false;

            for (auto it = node->children.begin(); it != node->children.end(); ++it) {
                PatriciaNode* child = it->second;
                string childKey = child->key;

                // Exact prefix match
                if (path.rfind(childKey, 0) == 0) {
                    node = child;
                    path = path.substr(childKey.length());

                    if (path.empty()) {
                        node->is_terminal = true;
                        node->data = action;
                        return;
                    }
                    matched = true;
                    break;
                }

                // Partial match → need split
                string common = getCommonPrefix(path, childKey);
                if (!common.empty()) {

                    // Remove old child
                    node->children.erase(it);

                    // Create split node
                    PatriciaNode* splitNode = new PatriciaNode(common);
                    node->children[common[0]] = splitNode;

                    // Remaining old key
                    string remainingOld = childKey.substr(common.length());
                    child->key = remainingOld;
                    splitNode->children[remainingOld[0]] = child;

                    // Continue insertion from split node
                    node = splitNode;
                    path = path.substr(common.length());
                    matched = false;
                    break;
                }
            }

            if (!matched) {
                if (!path.empty()) {
                    PatriciaNode* newNode =
                        new PatriciaNode(path, true, action);
                    node->children[path[0]] = newNode;
                } else {
                    node->is_terminal = true;
                    node->data = action;
                }
                return;
            }
        }
    }

    string search(string ip_binary) {
        PatriciaNode* node = root;
        string path = ip_binary;
        string last_match = "DEFAULT_DENY";

        while (true) {
            if (node->is_terminal) {
                last_match = node->data;
            }

            if (path.empty()) break;

            bool found = false;

            for (auto& pair : node->children) {
                PatriciaNode* child = pair.second;
                string childKey = child->key;

                if (path.rfind(childKey, 0) == 0) {
                    path = path.substr(childKey.length());
                    node = child;
                    found = true;
                    break;
                }
            }

            if (!found) break;
        }

        return last_match;
    }

    void printTree(PatriciaNode* node = nullptr, int level = 0) {
        if (!node) node = root;

        string indent(level * 3, ' ');
        cout << indent << "|-- Edge '" << node->key << "'";
        if (node->is_terminal)
            cout << " -> RULE: " << node->data;
        cout << endl;

        for (auto& pair : node->children) {
            printTree(pair.second, level + 1);
        }
    }
};

string ipToBinary(const string& ip) {
    string result;
    size_t start = 0, end;

    while ((end = ip.find('.', start)) != string::npos) {
        int num = stoi(ip.substr(start, end - start));
        for (int i = 7; i >= 0; --i)
            result += ((num >> i) & 1) ? '1' : '0';
        start = end + 1;
    }

    int num = stoi(ip.substr(start));
    for (int i = 7; i >= 0; --i)
        result += ((num >> i) & 1) ? '1' : '0';

    return result;
}

int main() {

    FirewallRadixTree fw;

    cout << "--- 1. CONFIGURING FIREWALL RULES ---\n";

    string office_net = "11000000"; // 192.x.x.x
    fw.insert(office_net, "ALLOW (Office Network)");
    cout << "Added Rule: 192.0.0.0/8 -> ALLOW\n";

    string malicious_ip =
        "11000000101010000000000100110111"; // 192.168.1.55
    fw.insert(malicious_ip, "BLOCK (Malicious Host)");
    cout << "Added Rule: 192.168.1.55 -> BLOCK\n";

    cout << "\n--- 2. VISUALIZING THE PATRICIA TRIE ---\n";
    fw.printTree();

    cout << "\n--- 3. TESTING PACKETS (Longest Prefix Match) ---\n";

    string test1 = "11000000111111110000000000000001"; // 192.255.0.1
    cout << "Packet from 192.255.0.1\n";
    cout << "Action: " << fw.search(test1) << "\n\n";

    cout << "Packet from 192.168.1.55\n";
    cout << "Action: " << fw.search(malicious_ip) << "\n\n";

    string test3 = ipToBinary("8.8.8.8");
    cout << "Packet from 8.8.8.8\n";
    cout << "Action: " << fw.search(test3) << "\n";

    return 0;
}
