#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
using namespace std;

struct Node {
    int p;
    int id;
    int color;
    int max_depth;
    vector<int> child;
};
int q, type;
Node nodes[100006];
vector<int> roots;
const int COLOR_MAX = 5;

// 깊이체크 함수
bool check_depth(Node p, int depth) {
    if (p.id == 0) return true;
    if (p.max_depth <= depth) return false;
    return check_depth(nodes[p.p], depth + 1);
}

// 삽입 함수
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
        roots.push_back(m);
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

// 색 변경 함수
void change_color() {
    int m, newcolor;
    cin >> m >> newcolor;
    stack<int> s;
    s.push(m);
    while (!s.empty()) {
        Node& cur = nodes[s.top()]; s.pop();
        nodes[cur.id].color = newcolor;
        for (int nxt : cur.child) {
            s.push(nxt);
        }
    }
}

// 색 조회 함수
void search_color() {
    int m;
    cin >> m;
    cout << nodes[m].color << "\n";
}

// 서브트리의 색깔 정보를 기반으로 점수 계산
pair<int, vector<int>> calculate_subtree_beauty(int curId) {
    Node& cur = nodes[curId];
    // 색깔을 카운팅하기 위한 배열, 색깔의 범위는 1부터 COLOR_MAX까지
    vector<int> colorCount(COLOR_MAX + 1, 0);
    colorCount[cur.color] = 1; // 현재 노드의 색깔을 기록

    int subtreeScore = 0;
    
    // 자식 노드들을 순회하면서 서브트리 점수와 색깔 정보를 가져옴
    for (int childId : cur.child) {
        auto [childScore, childColorCount] = calculate_subtree_beauty(childId);
        
        // 자식 노드에서 가져온 색깔 정보를 부모 노드에 합산
        for (int i = 1; i <= COLOR_MAX; i++) {
            colorCount[i] += childColorCount[i];
        }

        // 자식 서브트리에서의 점수를 부모 노드에 합산
        subtreeScore += childScore;
    }

    // 서로 다른 색깔의 개수를 계산하여 점수에 반영
    int distinctColors = 0;
    for (int i = 1; i <= COLOR_MAX; i++) {
        if (colorCount[i] > 0) distinctColors++;
    }
    subtreeScore += distinctColors * distinctColors;

    return make_pair(subtreeScore, colorCount);
}

// 전체 트리의 점수를 계산하는 함수
void value_sum() {
    long long total_value = 0;

    // 각 루트 노드에 대해 서브트리의 beauty 값을 계산
    for (int rootId : roots) {
        total_value += calculate_subtree_beauty(rootId).first;
    }

    cout << total_value << "\n";
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