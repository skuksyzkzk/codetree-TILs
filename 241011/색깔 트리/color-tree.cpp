#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_ID 100005 // ID의 최대값입니다
#define MAX_DEPTH 105 // 트리의 최대 깊이입니다
#define COLOR_MAX 5

using namespace std;

struct Node {
    int id;
    int color;
    int lastUpdate; // 노드가 추가 된 시점 혹은 가장 마지막으로 색깔 변경 명령 된 시점입니다
    int maxDepth; // node가 가질 수 있는 최대 깊이입니다
    int parentId; // 부모노드의 id를 저장합니다
    vector<int> childIds; // 자식노드의 id들을 저장합니다
};

// 점수 조회 명령을 간편히 구현하기 위한 class입니다
class ColorCount {
public:
    int cnt[COLOR_MAX+1];
    // 각 Color의 개수를 합칩니다
    ColorCount operator+(ColorCount const& obj)
    {
        ColorCount res;
        for (int i=1;i<=COLOR_MAX;i++) {
            res.cnt[i] = cnt[i] + obj.cnt[i];
        }
        return res;
    }
    // 서로다른 색의 개수의 제곱을 반환합니다
    int score() {
        int result = 0;
        for (int i=1;i<=COLOR_MAX;i++) {
            result += cnt[i] ? 1 : 0;
        }
        return result * result;
    }
};

Node nodes[MAX_ID];
bool isRoot[MAX_ID];

// 해당 node가 자식노드를 가질 수 있는지 확인합니다
// 해당 과정에서는 root까지 조상들을 각각 탐색하며 maxDepth를 확인합니다
bool canMakeChild(Node curr, int needDepth) {
    if (curr.id == 0) return true;
    if (curr.maxDepth <= needDepth) return false;
    return canMakeChild(nodes[curr.parentId], needDepth+1);
}

// curr 노드의 색깔 정보와 해당 색깔이 설정된 시간을 return 합니다.
// root에 도달할때까지 부모를 거슬러 올라가며 lastUpdate시간을 이용하여 현재 노드가 가져야하는 색깔을 계산합니다
pair<int, int> getColor(Node curr) {
    if (curr.id == 0) return make_pair(0, 0);
    pair<int,int> info = getColor(nodes[curr.parentId]); // root부터 내려온 색변화 정보를 가져옵니다
    if (info.second > curr.lastUpdate) return info;
    else return make_pair(curr.color, curr.lastUpdate); // 나의 색 변화 정보가 가장 최신이라면 이것을 child에게 반환해줍니다.
}

pair<int, ColorCount> getBeauty(Node curr, int color, int lastUpdate) {
    // root에서부터 내려온 색 정보보다 현재 노드의 색정보가 최신이라면 갱신합니다
    if (lastUpdate < curr.lastUpdate) {
        lastUpdate = curr.lastUpdate;
        color = curr.color;
    }
    pair<int, ColorCount> result;
    result.second.cnt[color] = 1;
    for (int childId : curr.childIds) {
        Node child = nodes[childId];
        // 각 자식이 이루는 SubTree에서의 점수와 color count 값을 가져옵니다
        pair<int, ColorCount> subResult = getBeauty(child, color, lastUpdate);
        result.second = result.second + subResult.second;
        result.first += subResult.first;
    }
    result.first += result.second.score();
    return result;
}

int main() {
    int Q;
    cin >> Q;
    // Q개의 query에 대해 명령을 수행합니다
    for(int i=1;i<=Q;i++) {
        int T, mId, pId, color, maxDepth;
        cin >> T;
        switch (T)
        {
        case 100:
            cin >> mId >> pId >> color >> maxDepth;
            // 부모의 Id가 -1인 경우 root노드입니다
            if (pId == -1) isRoot[mId] = true;
            // 현재 노드를 만드려는 위치에 노드를 만들 수 있는지 확인합니다
            if (isRoot[mId] || canMakeChild(nodes[pId], 1)) {
                // node 정보를 기입해줍니다
                nodes[mId].id = mId;
                nodes[mId].color = color;
                nodes[mId].maxDepth = maxDepth;
                nodes[mId].parentId = isRoot[mId] ? 0 : pId;
                nodes[mId].lastUpdate = i;

                if (!isRoot[mId])
                    nodes[pId].childIds.push_back(mId);
            }
            break;
        case 200:
            cin >> mId >> color;
            // 색 변화 명령에 대해 lastUpdate를 갱신하여 lazy update를 가능하게 준비합니다.
            // 시간복잡도를 위하여 색깔 변화 명령에 대해 subtree에 모두 갱신하는 것이 아닌, 추후 색깔 조회, 점수 조회 명령에서 lazy한 계산 가능한 형태로 만듭니다.
            nodes[mId].color = color;
            nodes[mId].lastUpdate = i;
            break;
        case 300:
            cin >> mId;
            // mId번 node가 가지는 색깔을 계산합니다
            cout << getColor(nodes[mId]).first << endl;
            break;
        case 400:
            int beauty = 0;
            for (int i=1; i<MAX_ID; i++) {
                // root 노드들에 대해 점수를 계산합니다
                if (isRoot[i]) {
                    beauty += getBeauty(nodes[i], nodes[i].color, nodes[i].lastUpdate).first;
                }
            }
            cout << beauty << endl;
            break;
        }
    }
    return 0;
}