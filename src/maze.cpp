#include "../include/maze.h"

Maze::Maze(int rows, int cols)
    : rows_(rows),
      cols_(cols),
      grid_(std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0))),
      vertices_(std::vector<std::vector<Vertex>>(
          rows, std::vector<Vertex>(cols_, {0, 0, 0}))) {
  indexVertices();
};

int Maze::getCols() { return cols_; }

int Maze::getRows() { return rows_; }

std::vector<std::vector<int>> Maze::grid() { return grid_; }

// void Maze::print() {
//   std::cout << " ";
//   for (int j = 0; j < cols_; j++) {
//     std::cout << "__";
//   }
//   std::cout << "\n";

//   for (int i = 0; i < rows_; i++) {
//     std::cout << "|";
//     for (int j = 0; j < cols_; j++) {
//       if (grid_[i][j] == Wall::BOTH) std::cout << "_|";
//       if (grid_[i][j] == Wall::DOWN) std::cout << "__";
//       if (grid_[i][j] == Wall::RIGHT) std::cout << " |";
//       if (grid_[i][j] == Wall::NONE) std::cout << "  ";
//       if (j == cols_ - 1) std::cout << "\n";
//     }
//   }
// }

void Maze::generateGraph() {
  graph_ = std::vector<std::vector<int>>(rows_ * cols_);

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (j != 0) {
        // если у ячейки слева от текущей нет правой стены
        if ((grid_[i][j - 1] & Wall::RIGHT) != Wall::RIGHT)
          // добавляем её как соседа для текущей ячейки
          graph_[vertices_[i][j].id].push_back(vertices_[i][j - 1].id);
      }
      if (j != cols_ - 1) {
        // если у текущей ячейки нет правой стены
        if ((grid_[i][j] & Wall::RIGHT) != Wall::RIGHT)
          // добавляем ячейку справа от неё как соседа
          graph_[vertices_[i][j].id].push_back(vertices_[i][j + 1].id);
      }
      if (i != 0) {
        // если у ячейки сверху от текущей нет нижней стены
        if ((grid_[i - 1][j] & Wall::DOWN) != Wall::DOWN)
          // добавляем её как соседа для текущей ячейки
          graph_[vertices_[i][j].id].push_back(vertices_[i - 1][j].id);
      }
      if (i != rows_ - 1) {
        // если у текущей ячейки нет нижней стены
        if ((grid_[i][j] & Wall::DOWN) != Wall::DOWN)
          // добавляем ячейку снизу от неё как соседа
          graph_[vertices_[i][j].id].push_back(vertices_[i + 1][j].id);
      }
    }
  }
}

void Maze::indexVertices() {
  int counter = 0;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      vertices_[i][j].id = counter++;
      vertices_[i][j].row = i;
      vertices_[i][j].col = j;
    }
  }
}

std::vector<std::pair<int, int>> Maze::getSolution(int row1, int col1, int row2,
                                                   int col2) {
  std::vector<std::pair<int, int>> path;

  if (isEmpty() || !isValidPoints(row1, col1, row2, col2)) return path;
  if (graph_.empty()) generateGraph();

  std::queue<int> queue;
  std::vector<bool> visited(rows_ * cols_, false);
  std::vector<int> parent(rows_ * cols_, -1);

  // вычисляем номера вершин старта и финиша
  int start_id = row1 * cols_ + col1;
  int finish_id = row2 * cols_ + col2;

  // помещаем начальную точку в очередь
  queue.push(start_id);
  visited[start_id] = true;

  while (!queue.empty()) {
    int current = queue.front();
    queue.pop();

    if (current == finish_id) {
      break;
    }

    // проверяем всех соседей
    for (int neighbor : graph_[current]) {
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        parent[neighbor] = current;
        queue.push(neighbor);
      }
    }
  }

  // если конечная точка не была посещена, решения не существует
  int current = finish_id;
  if (!visited[current]) {
    return path;
  }

  // восстанавливаем путь
  while (current != -1) {
    path.push_back(std::make_pair(current / cols_, current % cols_));
    current = parent[current];
  }
  // разворачиваем в обратном порядке
  std::reverse(path.begin(), path.end());
  return path;
}

bool Maze::isEmpty() { return (rows_ == 0 || cols_ == 0); }

bool Maze::isValidPoints(int x1, int y1, int x2, int y2) {
  bool is_valid = true;
  if (x1 < 0 || x1 >= rows_ || x2 < 0 || x2 >= rows_) is_valid = false;
  if (y1 < 0 || y1 >= cols_ || y2 < 0 || y2 >= cols_) is_valid = false;
  return is_valid;
}