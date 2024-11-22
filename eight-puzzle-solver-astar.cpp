#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
using namespace std;

// Structure to represent a state of the puzzle
struct State {
    vector<vector<int>> board;
    int g;
    int h;
    int f;
    pair<int, int> blank;
    State* parent;

    State(vector<vector<int>> b) : board(b), g(0), h(0), f(0), parent(nullptr) {
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if(board[i][j] == 0) {
                    blank = {i, j};
                    break;
                }
    }
};

struct CompareStates {
    bool operator()(const State* s1, const State* s2) {
        return s1->f > s2->f;
    }
};

int calculateHeuristic(const vector<vector<int>>& current) {
    int h = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(current[i][j] != 0) {
                int targetX = (current[i][j] - 1) / 3;
                int targetY = (current[i][j] - 1) % 3;
                h += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return h;
}

string boardToString(const vector<vector<int>>& board) {
    string s;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            s += to_string(board[i][j]);
    return s;
}

bool isGoal(const vector<vector<int>>& board) {
    int k = 1;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(i == 2 && j == 2) break;
            if(board[i][j] != k++) return false;
        }
    }
    return board[2][2] == 0;
}

vector<State*> getSuccessors(State* current) {
    vector<State*> successors;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for(int i = 0; i < 4; i++) {
        int newX = current->blank.first + dx[i];
        int newY = current->blank.second + dy[i];
        
        if(newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
            vector<vector<int>> newBoard = current->board;
            swap(newBoard[current->blank.first][current->blank.second], 
                 newBoard[newX][newY]);
            
            State* successor = new State(newBoard);
            successor->blank = {newX, newY};
            successor->g = current->g + 1;
            successor->h = calculateHeuristic(newBoard);
            successor->f = successor->g + successor->h;
            successor->parent = current;
            
            successors.push_back(successor);
        }
    }
    return successors;
}

void printPath(State* state) {
    if(state->parent != nullptr) {
        printPath(state->parent);
    }
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            cout << state->board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "----------------" << endl;
}

// A* algorithm implementation
State* solvePuzzle(vector<vector<int>> initial) {
    priority_queue<State*, vector<State*>, CompareStates> openList;
    map<string, bool> closedList;
    
    State* initialState = new State(initial);
    initialState->h = calculateHeuristic(initial);
    initialState->f = initialState->g + initialState->h;
    
    openList.push(initialState);
    
    while(!openList.empty()) {
        State* current = openList.top();
        openList.pop();
        
        string boardStr = boardToString(current->board);
        
        if(isGoal(current->board)) {
            return current;
        }
        
        if(closedList[boardStr]) {
            continue;
        }
        
        closedList[boardStr] = true;
        
        vector<State*> successors = getSuccessors(current);
        for(State* successor : successors) {
            string successorStr = boardToString(successor->board);
            if(!closedList[successorStr]) {
                openList.push(successor);
            }
        }
    }
    
    return nullptr;
}

int main() {
    vector<vector<int>> initial = {
        {1, 0, 6},
        {4, 3, 2},
        {7, 5, 8}
    };
    
    cout << "Initial state:" << endl;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            cout << initial[i][j] << " ";
        }
        cout << endl;
    }
    cout << "----------------" << endl;
    
    State* solution = solvePuzzle(initial);
    
    if(solution != nullptr) {
        cout << "Solution found! \nSteps:" << endl;
        printPath(solution);
        cout << "Number of moves: " << solution->g << endl;
    } else {
        cout << "No solution found!" << endl;
    }
    
    return 0;
}