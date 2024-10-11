#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct Node {
    int p;
    int id;
    int color;
    int max_depth;
    vector<int> child;
};
struct ColorCheck {
    long long result;
    int color[6];
};
int q, type;
Node nodes[100006];
vector<Node> roots;

// 깊이체크
bool check_depth(Node p, int depth) {
    if (p.id == 0) return true;
    if (p.max_depth <= depth) return false;
    else (check_depth(nodes[p.p], depth + 1));
}
// 삽입
void insert() {
    int m, p, c, d;
    cin >> m >> p >> c >> d;
    // 노드 정보 
    Node nd;
    nd.id = m;
    nd.p = p;
    nd.color = c;
    nd.max_depth = d;
    // -1이면 루트노드 
    if (p == -1) {
        roots.push_back(nd);
        nd.p = 0;
    }
    // root거나 아니거나 확인 해서 가능하면 
    if (p == -1 || check_depth(nodes[p], 1)) {
        nodes[m] = nd;
    }
    // root 아니면 해당 부모의 자식으로 삽입 
    if (p != -1) {
        nodes[p].child.push_back(m);
    }
    
}
// 해당 하는 m_id의 서브트리를 전부 바꿔야함 
void change_color() {
    int m, newcolor;
    cin >> m >> newcolor;
    stack<int> s;
    s.push(m);
    while (!s.empty()) {
        Node cur = nodes[s.top()]; s.pop();
        nodes[cur.id].color = newcolor;
        for (int nxt : cur.child) {
            s.push(nxt);
        }
    }
    
}
int search_color() {
    int m;
    cin >> m;
    cout << nodes[m].color << "\n";
}
long long value_sum() {
    long long totalBeauty = 0;
    for (Node& root : roots) {
        stack<int> s;
        s.push(root.id);
        ColorCheck colorCheck = {0, {0}};

        while (!s.empty()) {
            int curId = s.top(); s.pop();
            Node& cur = nodes[curId];
            if (colorCheck.color[cur.color] == 0) {
                colorCheck.result++;
            }
            colorCheck.color[cur.color]++;

            for (int childId : cur.child) {
                s.push(childId);
            }
        }

        totalBeauty += colorCheck.result * colorCheck.result;
    }
    cout << totalBeauty << "\n";
}

int main() {
    // 여기에 코드를 작성해주세요.
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