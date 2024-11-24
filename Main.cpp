#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <algorithm>

using namespace std;


const vector<int> GOAL_STATE = {1, 2, 3, 4, 5, 6, 7, 8, 0};

void printState(const vector<int>& state) {
    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0) cout << endl;
        cout << state[i] << " ";
    }
    cout << endl;
}
int findBlank(const vector<int>& state) {
    return find(state.begin(), state.end(), 0) - state.begin();
}
vector<int> getPossibleMoves(int blankPos) {
    vector<int> moves;
    int row = blankPos / 3, col = blankPos % 3;
    // Up
    if (row > 0) moves.push_back(blankPos - 3);
    // Down
    if (row < 2) moves.push_back(blankPos + 3);
    // Left
    if (col > 0) moves.push_back(blankPos - 1);
    // Right
    if (col < 2) moves.push_back(blankPos + 1);

    return moves;
}

void solve8Puzzle(const vector<int>& startState) {
    queue<vector<int>> q;
    set<vector<int>> visited; 
    map<vector<int>, vector<int>> parent; 

    q.push(startState);
    visited.insert(startState);
    parent[startState] = {}; 
    int explored = 0;

    while (!q.empty()) {
        vector<int> currentState = q.front();
        q.pop();
        explored++; 
        if (currentState == GOAL_STATE) {
            cout << "______________" << endl;
            vector<vector<int>> path;
            for (vector<int> state = currentState; !state.empty(); state = parent[state])
                path.push_back(state);

            reverse(path.begin(), path.end());
            
            for (const auto& state : path)
                printState(state);
            cout << endl;
            cout << "Total nodes explored: " << explored << endl;    
            cout << "Number of moves: " << path.size()-1 << endl;
            return;
        }

        int blankPos = findBlank(currentState);
        vector<int> moves = getPossibleMoves(blankPos);

        for (int newPos : moves) {
            vector<int> newState = currentState;
            swap(newState[blankPos], newState[newPos]);

            if (!visited.count(newState)) {
                visited.insert(newState);
                q.push(newState);
                parent[newState] = currentState;
            }
        }
    }
    cout << explored << endl;    
    cout << "No solution found." << endl;
}

int main() {
    // vector<int> startState = {
    //   1, 2, 3 ,
    //   4, 6, 8, 
    //   7, 0, 5 
    // };
    vector<int> startState = {
        1, 2, 3 ,
        4, 6, 8, 
        7, 0, 5
    };
    cout << "Start state:";
    printState(startState);
    solve8Puzzle(startState);

    return 0;
}




