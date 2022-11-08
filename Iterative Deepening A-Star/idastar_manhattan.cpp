#include <bits/stdc++.h>
using namespace std;

int expanded_nodes = 0;

bool found_ans = 0;

map<int, pair<int, int>> m1;

set<vector<vector<int>>> hashset;

vector<vector<int>> goal = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};

// Used to move in 4 Directions
int dir_row[] = { -1, 1, 0, 0};            
int dir_col[] = { 0, 0, -1, 1};   
char direction[] = {'U','D','L','R'};         

// To save co-ordinates of blank tile
struct coordinates {
    int x;
    int y;
};

//Node has state in 2D matrix form , coodinates of zero, path and depth
struct node {
    vector<vector<int>> state;
    coordinates blank_coordinates;
    vector<char> path;
    int fn;
    int gn;
};

void populate_map() {
    int cnt = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m1[cnt] = {i, j};
            cnt++;
            cnt %= 16;
        }
    }
}

int manhattan_distance(node *a) {
    vector<vector<int>> state = a->state; 
    int answer = 0;
    for (int x1 = 0; x1 < 4; x1++) {
        for (int y1 = 0; y1 < 4; y1++) {
            if (state[x1][y1] == 0) 
                continue;
            int x2 = m1[state[x1][y1]].first;
            int y2 = m1[state[x1][y1]].second;
            answer += (abs(x1 - x2) + abs(y1 - y2));
        }
    }
    return answer;
}

// To check if visited
bool isVisited(vector<vector<int>> a) {
    if (hashset.find(a) == hashset.end())
        return false;
    return true;
}

// To set a state to visited
void setVisited(vector<vector<int>> a) {
    hashset.insert(a);
}

// This is used to copy the 2D matrix
vector<vector<int>> copy_matrix(vector<vector<int>> old_matrix) {
    vector<vector<int>> new_matrix;
    for (int i = 0; i < old_matrix.size(); i++) {
        vector<int> temp;
        for (int j = 0; j < old_matrix[i].size(); j++) {
            temp.push_back(old_matrix[i][j]);
        }
        new_matrix.push_back(temp);
    }
    return new_matrix;
}

// This can be used to print the states and check the solution 
void print_state(node* current) {
    vector<vector<int>> a = current->state;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}

// This is used to expand the nodes. Visit 4 directions if and only if possible
vector<node*> expand(node* current) {
    expanded_nodes++;
    vector<node*> list_of_child_nodes;
    int x = current->blank_coordinates.x;
    int y = current->blank_coordinates.y;
    for (int i = 0; i < 4; i++) {
        struct node* new_node = (struct node *) malloc(sizeof(struct node));
        new_node->state = copy_matrix(current->state);
        new_node->path = current->path;
        (new_node->path).push_back(direction[i]);
        int new_x = x + dir_row[i];
        int new_y = y + dir_col[i];
        // Condition to check if it is possible to move in a particular direction
        if (new_x >= 0 && new_x < 4 && new_y >= 0 && new_y < 4) {
            swap(new_node->state[new_x][new_y], new_node->state[x][y]);
            new_node->blank_coordinates.x = new_x;
            new_node->blank_coordinates.y = new_y;
            new_node->gn = current->gn + 1;
            // f(n) = g(n) + h(n)
            new_node->fn = manhattan_distance(new_node) + new_node->gn;
            list_of_child_nodes.push_back(new_node);
        }
    }
    return list_of_child_nodes;
}

int search(stack<node*> s, int bound);

int astar(node* start) {
    int bound = manhattan_distance(start);
    // printf("%d", bound);
    stack<node*> s;
    s.push(start);
    setVisited(start->state);
    while (true) {
        int t = search(s, bound);
        if (found_ans)
            return 0;
        if (t == INT_MAX) {
            printf("NOT FOUND\n");
            return 0;
        }
        bound = t;
    }
    return 0;
}

int search(stack<node*> s, int bound) {
    node* curr_node = s.top();
    s.pop();
    // print_state(curr_node);
    // printf("%d %d\n", bound, curr_node->fn);
    if (curr_node->state == goal) {
        found_ans = true;
        printf("MOVES: ");
        for (int i = 0; i < curr_node->path.size(); i++)
            printf("%c", curr_node->path[i]);
        printf("\n");
        return 0;
    }
    if (curr_node->fn > bound)
        return curr_node->fn;
    int mini = INT_MAX;
    for (auto child: expand(curr_node)) {
        if (!isVisited(child->state)) {
            // print_state(child);
            setVisited(child->state);
            s.push(child);
            int t = search(s, bound);
            if (found_ans)
                return 0;
            if (t < mini)
                mini = t;
            // s.pop();
        }
    }
    return mini;
}

int main() {
    // vector<vector<int>> start = {{1,0,2,4},{5,7,3,8},{9,6,11,12},{13,10,14,15}};
    vector<vector<int>> start;
    populate_map();
    for (int i = 0; i < 4; i++) {
        vector<int> temp;
        for (int j = 0; j < 4; j++) {
            int x;
            scanf("%d", &x);
            temp.push_back(x);
        }
        start.push_back(temp);
    }
    node *s = (struct node *) malloc(sizeof(struct node));
    s->state = copy_matrix(start);
    s->gn = 0;
    s->fn = s->gn + manhattan_distance(s);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (start[i][j] == 0) {
                s->blank_coordinates.x = i;
                s->blank_coordinates.y = j;
            }
        }
    }
    auto start_time = std::chrono::high_resolution_clock::now();
    astar(s);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    printf("Number of nodes expanded: %d\n", expanded_nodes);
    cout << "Time Taken (in microseconds): " << duration.count() << endl;
    printf("Memory Used: ");
    cout << (sizeof(hashset) + hashset.size()*sizeof(goal)) / 1024 << " KB";
    return 0;
}
