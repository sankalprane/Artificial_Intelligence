#include <bits/stdc++.h>
using namespace std;

int expanded_nodes = 0;



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
    int depth;
};

// To check if visited
bool isVisited(vector<vector<int>> a, set<vector<vector<int>>> hashset) {
    if (hashset.find(a) == hashset.end())
        return false;
    return true;
}

// To set a state to visited
void setVisited(vector<vector<int>> a, set<vector<vector<int>>> hashset) {
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
            list_of_child_nodes.push_back(new_node);
        }
    }
    return list_of_child_nodes;
}

// just declaration
int depth_limited_search(node* start, int l,set<vector<vector<int>>> hashset );

// Used to increment and use depth limited for increasing depths
int iterative_deepening_search(node *start) {
    for (int i = 0; i < 10; i++) {
        // hashset.clear();
        set<vector<vector<int>>> hashset;
        int result = depth_limited_search(start, i, hashset);
        if (result)
            return result;
    }
    return 0;
}

int depth_limited_search(node* start, int l, set<vector<vector<int>>> hashset) {
    stack<node*> s;
    s.push(start);
    setVisited(start->state, hashset);
    while(!s.empty()) {
        node* x = s.top();
        s.pop();
        // If solution is found the path is printed
        if (x->state == goal) {
            printf("MOVES: ");
            for (int i = 0; i < x->path.size(); i++)
                printf("%c", x->path[i]);
            printf("\n");
            return 1;
        }
        //checks for depth on every iteration
        if (x->depth < l) {
            for (node* child: expand(x)) {
                // Checks for cycles
                if (!isVisited(child->state, hashset)) {
                    s.push(child);
                    setVisited(child->state, hashset);
                    child->depth = x->depth + 1;
                }
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
    s->depth = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (start[i][j] == 0) {
                s->blank_coordinates.x = i;
                s->blank_coordinates.y = j;
            }
        }
    }
    auto start_time = std::chrono::high_resolution_clock::now();
    iterative_deepening_search(s);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    printf("Number of nodes expanded: %d\n", expanded_nodes);
    cout << "Time Taken (in microseconds): " << duration.count() << endl;
    printf("Memory Used: ");
    // cout << (sizeof(hashset) + hashset.size()*sizeof(goal)) / 1024 << " KB";
    return 0;
}