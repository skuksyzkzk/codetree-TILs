#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
using namespace std;

struct Node {
    int p;
    int id;
    int depth;
    int color;
    vector<int> child;
};

int q,type;
Node nodes[100005];
int COLOR_MAX = 5;
vector<int> roots;

bool isValid(int parent, int depth) {
    if (nodes[parent].id == 0) return true;
    if (nodes[parent].depth <= depth) return false;
    else return isValid(nodes[parent].p, depth + 1);
}
void insert() {
    int m, p, color, depth;
    cin >> m >> p >> color >> depth;
    Node dummy;
    dummy.id = m;
    dummy.p = p;
    dummy.depth = depth;
    dummy.color = color;

    if (p == -1) {
        roots.push_back(m);
        dummy.p = 0;
    }
    if (p == -1 || isValid(p, 1)) {
        nodes[m] = dummy;
    }
    if (p != -1) {
        nodes[p].child.push_back(m);
    }
    
}
void change_color() {
    int m, newcolor;
    cin >> m >> newcolor;
    stack<int> s;
    s.push(m);
    while (!s.empty()) {
        Node& cur = nodes[s.top()]; s.pop();
        nodes[cur.id].color = newcolor;
        for (int nxt : nodes[cur.id].child) {
            s.push(nxt);
        }
    }
}
void search_color() {
    int m;
    cin >> m;
    cout << nodes[m].color << "\n";
}
pair<int, vector<int>> calculate_sum(int curid) {
    Node& cur = nodes[curid];
    vector<int> colorCount(COLOR_MAX + 1, 0);
    colorCount[cur.color] = 1;

    int subtree_sum = 0;

    for (int nxt : cur.child) {
        pair<int, vector<int>> subret = calculate_sum(nxt);

        for (int i = 1; i <= COLOR_MAX; i++) {
            colorCount[i] += subret.second[i];
        }

        subtree_sum += subret.first;
    }
    int different = 0;
    for (int i = 1; i <= COLOR_MAX; i++) {
        if (colorCount[i] > 0) different++;
    }
    subtree_sum += different * different;

    return make_pair(subtree_sum,colorCount);
}
void value_sum() {
    int total = 0;
    for (auto z : roots) {
        total += calculate_sum(z).first;
    }
    cout << total << "\n";
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> q;
    nodes[0].id = 0; // 루트의 루트는 0
    for (int i = 0; i < q; i++) {
        cin >> type;
        if (type == 100) insert();
        else if (type == 200) change_color();
        else if (type == 300) search_color();
        else if (type == 400) value_sum();
    }
    return 0;
}