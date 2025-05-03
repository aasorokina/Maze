#include "../include/maze_serializer.h"

Maze MazeSerializer::readFromFile(const std::string file_path) {
  std::ifstream file;
  file.open(file_path);
  if (!file) {
    std::cerr << "Cannot open file: " + file_path << '\n';
    return Maze(0, 0);
  }
  int rows = 0, cols = 0;
  if (!(file >> rows) || !(file >> cols) || rows <= 0 || cols <= 0) {
    file.close();
    return Maze(0, 0);
  }

  Maze maze(rows, cols);

  // считывание первой матрицы с правыми стенами
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int temp = 0;
      if (!(file >> temp)) {
        file.close();
        return Maze(0, 0);
      }
      maze.grid_[i][j] = temp;
    }
  }

  // считывание второй матрицы со стенами снизу
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int temp = 0;
      if (!(file >> temp)) {
        file.close();
        return Maze(0, 0);
      }
      // стена снизу записывается во второй бит ячейки,
      // содержащей правую стену
      maze.grid_[i][j] = maze.grid_[i][j] | (temp << 1);
    }
  }

  file.close();
  return maze;
}

void MazeSerializer::saveToFile(Maze maze, const std::string file_path) {
  std::ofstream file(file_path);
  if (!file) {
    std::cerr << "Cannot write to file:" + file_path << '\n';
    return;
  }

  int rows = maze.getRows();
  int cols = maze.getCols();

  if (rows < 1 || cols < 1) {
    file.close();
    return;
  }

  file << rows << ' ' << cols << '\n';

  // запись в файл первой матрицы с правыми стенами
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      file << (maze.grid_[i][j] & Maze::Wall::RIGHT);
      file << ((j == cols - 1) ? '\n' : ' ');
    }
  }

  file << '\n';

  // запись в файл второй матрицы со стенами снизу
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      file << ((maze.grid_[i][j] & Maze::Wall::DOWN) >> 1);
      file << ((j == cols - 1) ? '\n' : ' ');
    }
  }

  file.close();
}
