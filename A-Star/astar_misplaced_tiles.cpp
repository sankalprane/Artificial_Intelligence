#include <bits/stdc++.h>
using namespace std;

int expanded_nodes = 0;

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

int heuristic(node *a) {
    vector<vector<int>> state = a->state; 
    int misplaced_tiles = 0;
    int cnt = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == 3 && j == 3) 
                continue;
            if (cnt != state[i][j])
                misplaced_tiles++;
            cnt = (cnt + 1) % 16;
        }
    }
    return misplaced_tiles;
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
            new_node->fn = heuristic(new_node) + new_node->gn;
            list_of_child_nodes.push_back(new_node);
        }
    }
    return list_of_child_nodes;
}

struct compare {
    bool operator()(const node * lhs, const node * rhs) const {
        return lhs->fn > rhs->fn;
    }
};

int astar(node* start) {
    priority_queue<node *, vector<node *>, compare> pq;
    pq.push(start);
    while(!pq.empty()) {
        node* x = pq.top();
        pq.pop();
        setVisited(x->state);
        if (x->state == goal) {
            printf("MOVES: ");
            for (int i = 0; i < x->path.size(); i++)
                printf("%c", x->path[i]);
            printf("\n");
            return 1;
        }
        for (node* child: expand(x)) {
            if (!isVisited(child->state)) {
                pq.push(child);
                setVisited(child->state);
                // printf("pushed to queue\n");
            }
        }
    }
    return 0;
}

int main() {
    // vector<vector<int>> start = {{1,0,2,4},{5,7,3,8},{9,6,11,12},{13,10,14,15}};
    vector<vector<int>> start;
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
