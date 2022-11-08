#include <bits/stdc++.h>

using namespace std;
int n, m;
double u[20][20], reward[20][20], v[20][20];
char policy[20][20];
double discount_rate, epsilon;
vector<pair<int, int>> walls, terminal;
vector<vector<pair<int, int>>> dir = {
                                        {{-1, 0}, {0, -1}, {0, 1}, {1, 0}}, 
                                        {{1, 0}, {0, 1}, {0, -1}, {-1, 0}}, 
                                        {{0, -1}, {1, 0}, {-1, 0}, {0, 1}}, 
                                        {{0, 1}, {-1, 0}, {1, 0}, {0, -1}}
                                    };  
char direction[] = {'S','N','W','E'};
vector<double> prob;

bool hitsWall(int i, int j) {
    for (int k = 0; k < walls.size(); k++) {
        if (walls[k].first == i && walls[k].second == j)
            return true;
    }
    return false;
}

double qvalue(int i, int j, int action) {
    double sum = 0;
    for (int k = 0; k < 4; k++) {
        int new_i = i + dir[action][k].first;
        int new_j = j + dir[action][k].second;
        if (new_i < 0 || new_i >= n || new_j < 0 || new_j >= m || hitsWall(new_i, new_j)) {
            new_i = i;
            new_j = j;
        }
        sum += prob[k] * (reward[new_i][new_j] + discount_rate * u[new_i][new_j]);
    }
    return sum;
}

void print_value() {
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < m; j++) {
            if (hitsWall(i, j)) {
                printf("------- ");
            } else {
                printf("%lf ", u[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

bool isTerminalState(int i, int j) {
    for (int k = 0; k < terminal.size(); k++) {
        if (terminal[k].first == i && terminal[k].second == j)
            return true;
    }
    return false;
}

int value_iteration() {
    int iteration = 0;
    printf("The value of %d iteration: \n", iteration);
    print_value();
    double delta = 0;
    while (true) {
        iteration++;
        delta = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                double U = u[i][j];
                double maxi = -1;
                for (int action = 0; action < 4; action++) {
                    if (isTerminalState(i, j)) {
                        maxi = 0;
                    } else {
                        // maxi = max(maxi, qvalue(i, j, action));
                        if (qvalue(i, j, action) > maxi) {
                            maxi = qvalue(i, j, action);
                            policy[i][j] = direction[action];
                        }
                    }
                }
                v[i][j] = maxi;
                if (abs(U - v[i][j]) > delta)
                    delta = abs(U - v[i][j]);
            }
        }
        // printf("delta %lf\n", delta);
        // printf("epsilon %lf\n", epsilon * (1 - discount_rate) / discount_rate);
        for (int i = 0; i < n; i++) 
            for (int j = 0; j < m; j++)
                u[i][j] = v[i][j];
        if (delta <= epsilon * (1 - discount_rate) / discount_rate)
            break;
        printf("iteration: %d\n", iteration);
        print_value();
    };
    printf("Final Value After Convergence \n", iteration);
    print_value();
    return 0;
}

void print_policy() {
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < m; j++) {
            if (isTerminalState(i, j)) {
                printf("T ");
            } else if(hitsWall(i, j)) {
                printf("- ");
            } else {
                printf("%c ", policy[i][j]);
            }
        }
        printf("\n");
    }
}
int main() {
    // n = 4, m = 5;
    string str;
    string input_type;
    double r;
    while (cin >> str) {
        if ( str == "size" || str == "walls" || str == "terminal_states" ||
            str == "reward" || str == "transition_probabilities" || str == "discount_rate" ||
            str == "epsilon" ) {
            input_type = str;
        }
        if (str == ":") {
            if (input_type == "size") {
                cin >> m >> n;
                // printf("m: %d, n: %d\n", m, n);
            }
            if (input_type == "walls") {
                string temp;
                while (cin >> temp) {
                    if (temp[0] == '#')
                        break;
                    if (temp[0] == ',')
                        cin >> temp;
                    int i, j;
                    i = stoi(temp);
                    cin >> j;
                    i--, j--;
                    walls.push_back({j, i});
                    // printf("wall_j: %d, wall_i: %d\n", j, i);
                }
            }
            if (input_type == "terminal_states") {
                string temp;
                while (cin >> temp) {
                    if (temp[0] == '#')
                        break;
                    if (temp[0] == ',')
                        cin >> temp;
                    int i, j;
                    double k;
                    i = stoi(temp);
                    cin >> j >> k;
                    i--, j--;
                    reward[j][i] = k;
                    terminal.push_back({j, i});
                    // printf("reward_j: %d, reward_i: %d, reward_val: %lf\n", j, i, k);
                }
            }
            if (input_type == "reward") {
                cin >> r;
                // printf("reward: %lf\n", r);
            }
            if (input_type == "transition_probabilities") {
                for (int i = 0; i < 4; i++) {
                    double temp;
                    cin >> temp;
                    prob.push_back(temp);
                    // printf("prob: %lf\n", temp);
                }
                
            }
            if (input_type == "discount_rate") {
                cin >> discount_rate;
                // printf("discount_rate: %lf\n", discount_rate);
            }
            if (input_type == "epsilon") {
                cin >> epsilon;
                // printf("epsilon: %lf\n", epsilon);
            }
        }
        // cout << str << " ";
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!isTerminalState(i, j))
                reward[i][j] = r;
            u[i][j] = 0;
            v[i][j] = 0;
        }
    }
    value_iteration();
    printf("Final Policy\n");
    print_policy();
    return 0;
}