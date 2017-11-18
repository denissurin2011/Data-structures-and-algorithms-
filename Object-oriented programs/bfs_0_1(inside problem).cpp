#include <iostream>
#include <vector>
#include <cmath>
#include <deque>
#include <algorithm>

int graph[(1000 + 2) * (1000 + 2) * 8][8];
int dist[(1000 + 2) * (1000 + 2) * 8];

struct Cell {
    int number, left, right, up, down, up_left, up_right, down_left, down_right;
    Cell() : number(0), left(0), right(0), up(0), down(0), up_left(0),
             up_right(0), down_left(0), down_right(0) {}
    Cell(int width, int i, int j, const int v_in_cell) {
        number = (width + 2) * i * v_in_cell + j * v_in_cell;
        left = (width + 2) * i * v_in_cell + (j - 1) * v_in_cell + 4;
        right = (width + 2) * i * v_in_cell + (j + 1) * v_in_cell;
        up = (width + 2) * (i - 1) * v_in_cell + j * v_in_cell + 6;
        down = (width + 2) * (i + 1) * v_in_cell + j * v_in_cell + 2;
        up_left = (width + 2) * (i - 1) * v_in_cell + (j - 1) * v_in_cell + 5;
        up_right = (width + 2) * (i - 1) * v_in_cell + (j + 1) * v_in_cell + 7;
        down_left = (width + 2) * (i + 1) * v_in_cell + (j - 1) * v_in_cell + 3;
        down_right = (width + 2) * (i + 1) * v_in_cell + (j + 1) * v_in_cell + 1;
    }
};

void build_start_cell_inside(std::vector<std::vector<bool>>& field,
                             Cell cell, const int v_in_cell) {
    for (int u = cell.number; u < cell.number + v_in_cell; u++) {
        for (int num_v_in_cell = cell.number, num_of_neighbour = 0;
             num_v_in_cell < cell.number + v_in_cell; num_v_in_cell++) {
            if (u == num_v_in_cell) {
                continue;
            }
            graph[u][num_of_neighbour] = num_v_in_cell;
            num_of_neighbour++;
        }
    }
}

void build_start_cell_outside(std::vector<std::vector<bool>>& field,
                              Cell cell, const int v_in_cell, int i, int j) {
    if (!field[i][j - 1]) graph[cell.number][v_in_cell - 1] = (-cell.left);
    if (!field[i - 1][j - 1]) graph[cell.number + 1][v_in_cell - 1] = (-cell.up_left);
    if (!field[i - 1][j]) graph[cell.number + 2][v_in_cell - 1] = (-cell.up);
    if (!field[i - 1][j + 1]) graph[cell.number + 3][v_in_cell - 1] = (-cell.up_right);
    if (!field[i][j + 1]) graph[cell.number + 4][v_in_cell - 1] = (-cell.right);
    if (!field[i + 1][j + 1]) graph[cell.number + 5][v_in_cell - 1] = (-cell.down_right);
    if (!field[i + 1][j]) graph[cell.number + 6][v_in_cell - 1] = (-cell.down);
    if (!field[i + 1][j - 1]) graph[cell.number + 7][v_in_cell - 1] = (-cell.down_left);
}

void build_ordinary_cell_inside(std::vector<std::vector<bool>>& field,
                                Cell cell, const int v_in_cell) {
    for (int u = cell.number; u < cell.number + v_in_cell; u++) {
        for (int num_v_in_cell = cell.number, num_of_neighbour = 0;
             num_v_in_cell < cell.number + v_in_cell; num_v_in_cell++) {
            if (num_v_in_cell == u) {
                continue;
            }
            if (abs(u - num_v_in_cell) == 4) {
                graph[u][num_of_neighbour] = (num_v_in_cell);
                num_of_neighbour++;
            } else {
                graph[u][num_of_neighbour] = (-num_v_in_cell);
                num_of_neighbour++;
            }
        }
    }
}

void build_ordinary_cell_outside(std::vector<std::vector<bool>>& field,
                                 Cell cell, const int v_in_cell, int i, int j) {
    if (!field[i][j - 1]) graph[cell.number][v_in_cell - 1] = (cell.left);
    if (!field[i - 1][j - 1]) graph[cell.number + 1][v_in_cell - 1] = (cell.up_left);
    if (!field[i - 1][j]) graph[cell.number + 2][v_in_cell - 1] = (cell.up);
    if (!field[i - 1][j + 1]) graph[cell.number + 3][v_in_cell - 1] = (cell.up_right);
    if (!field[i][j + 1]) graph[cell.number + 4][v_in_cell - 1] = (cell.right);
    if (!field[i + 1][j + 1]) graph[cell.number + 5][v_in_cell - 1] = (cell.down_right);
    if (!field[i + 1][j]) graph[cell.number + 6][v_in_cell - 1] = (cell.down);
    if (!field[i + 1][j - 1]) graph[cell.number + 7][v_in_cell - 1] = (cell.down_left);
}

void build_graph(std::vector<std::vector<bool>>& field,
                 int height, int width,  const int v_in_cell, int start) {
    for (int i = 1; i < height + 1; i++) {
        for (int j = 1; j < width + 1; j++) {
            if (field[i][j]) {
                continue;
            }
            Cell cell(width, i, j, v_in_cell);
            if (cell.number == start) {
                build_start_cell_inside(field, cell, v_in_cell);
                build_start_cell_outside(field, cell, v_in_cell, i, j);
            } else {
                build_ordinary_cell_inside(field, cell, v_in_cell);
                build_ordinary_cell_outside(field, cell, v_in_cell, i, j);
            }
        }
    }
}

void bfs_0_1(int start, int dist[], const int v_in_cell) {
    std::deque<int> q;
    q.push_back(start);
    while (!q.empty()) {
        int v = q.front();
        q.pop_front();
        for (int i = 0; i < v_in_cell && graph[v][i] != 0; i++) {
            int to = abs(graph[v][i]);
            int weight;
            if (graph[v][i] < 0) {
                weight = 1;
            } else {
                weight = 0;
            }
            if (dist[to] > dist[v] + weight) {
                dist[to] = dist[v] + weight;
                if (graph[v][i] < 0) {
                    q.push_back(to);
                } else {
                    q.push_front(to);
                }
            }
        }
    }
}

int get_ans(int dist[], int finish, const int v_in_cell, const int INF) {
    int ans = INF;
    for (int i = finish; i < finish + v_in_cell; i++) {
        if (dist[i] == INF) {
            return -1;
        }
        ans = std::min(ans, dist[i]);
    }
    return ans;
}

void initialise_field_size(std::vector<std::vector<bool>>& field, const int max_width) {
    field.resize(max_width);
    for (int i = 0; i < max_width; i++) {
        field[i].resize(max_width);
    }
}

void initialise_field_borders(std::vector<std::vector<bool>>& field,
                              const int max_width, int height, int width) {
    for (int i = 0; i < width + 2; i++) {
        field[0][i] = true;
        field[height + 1][i] = true;
    }
    for (int i = 0; i < height + 2; i++) {
        field[i][0] = true;
        field[i][width + 1] = true;
    }
}

void input_field(std::vector<std::vector<bool>>& field, int height, int width) {
    for (int i = 1; i < height + 1; i++) {
        for (int j = 1; j < width + 1; j++) {
            char symb;
            std::cin >> symb;
            if (symb == '.') {
                field[i][j] = false;
            } else {
                field[i][j] = true;
            }
        }
    }
}

void initialise_field(std::vector<std::vector<bool>>& field,
                      const int max_width, int height, int width) {
    initialise_field_size(field, max_width);
    initialise_field_borders(field, max_width, height, width);
    input_field(field, height, width);
}

int change_coordinate(int height, int coordinate) {
    return height - coordinate + 1;
}

void initialise_dist(int dist[], const int INF, int start, const int max_number) {
    for (int i = 0; i < max_number; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;
}

int get_cell_number(int y, int width, const int v_in_cell, int x) {
    return y * (width + 2) * v_in_cell + x * v_in_cell;
}

int main() {
    const int INF = 2e9;
    const int max_width = 1e3 + 2;
    const int v_in_cell = 8;
    const int max_number = max_width * max_width * v_in_cell;
    std::vector<std::vector<bool>> field;
    int height, width, start_x, start_y, target_x, target_y;
    std::cin >> height >> width;
    initialise_field(field, max_width, height, width);
    std::cin >> start_x >> start_y >> target_x >> target_y;
    start_y = change_coordinate(height, start_y);
    target_y = change_coordinate(height, target_y);
    int start = get_cell_number(start_y, width, v_in_cell, start_x);
    int finish = get_cell_number(target_y, width, v_in_cell, target_x);
    build_graph(field, height, width, v_in_cell, start);
    initialise_dist(dist, INF, start, max_number);
    bfs_0_1(start, dist, v_in_cell);
    std::cout << get_ans(dist, finish, v_in_cell, INF);
    return 0;
}
