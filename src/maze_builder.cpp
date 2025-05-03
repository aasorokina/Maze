#include "../include/maze_builder.h"

MazeBuilder::MazeBuilder()
    : rows_(0), cols_(0), set_counter_(0), maze_(Maze(0, 0)) {
  srand(time(NULL));
}

Maze MazeBuilder::createMaze(int rows, int cols) {
  resetMaze(rows, cols);
  // создаем массив для хранения множеств
  std::vector<std::vector<int>> cell_sets;
  cell_sets.push_back(std::vector<int>(cols_, 0));

  for (int i = 0; i < rows_; i++) {
    assignUniqueSet(cell_sets[i]);
    createRightWalls(cell_sets[i], i);
    createDownWalls(cell_sets[i], i);
    if (i == rows_ - 1) {
      // обрабатываем последнюю строку
      handleLastLine(cell_sets[i], i);
    } else {
      // добавляем строки
      cell_sets.push_back(cell_sets[i]);
      maze_.grid_[i + 1] = maze_.grid_[i];
      deleteWalls(cell_sets[i + 1], i + 1);
    }
  }
  return maze_;
}
void MazeBuilder::assignUniqueSet(std::vector<int>& line) {
  for (int j = 0; j < cols_; j++) {
    // если ячейка равна 0 (не входит ни в одно множество),
    // присваиваем ей уникальное множество
    if (line[j] == 0) {
      line[j] = ++set_counter_;
    }
  }
}
void MazeBuilder::createRightWalls(std::vector<int>& line, int cur_row) {
  for (int i = 0; i < cols_ - 1; i++) {
    // решаем, добавлять ли правую стену
    int wall = rand() % 2;
    if (wall == 0) {
      // если ячейки принадлежат одному множеству,
      if (line[i] == line[i + 1])
        // обязательно добавляем стену
        maze_.grid_[cur_row][i] = maze_.grid_[cur_row][i] | Maze::Wall::RIGHT;
      else
        // объединяем множества
        mergeSets(line, line[i], line[i + 1]);
    } else {
      // wall == 1, добавляем стену
      maze_.grid_[cur_row][i] = maze_.grid_[cur_row][i] | Maze::Wall::RIGHT;
    }
  }
  // к последней ячейке в строке добавляем правую стену
  maze_.grid_[cur_row][cols_ - 1] =
      maze_.grid_[cur_row][cols_ - 1] | Maze::Wall::RIGHT;
}

void MazeBuilder::mergeSets(std::vector<int>& line, int set1, int set2) {
  for (int i = 0; i < cols_; i++) {
    if (line[i] == set2) {
      line[i] = set1;
    }
  }
}

void MazeBuilder::createDownWalls(std::vector<int>& line, int cur_row) {
  for (int i = 0; i < cols_; i++) {
    int wall = rand() % 2;
    if (wall == 1) {
      // проверяем, что множество имеет больше одной ячейки без стены снизу
      if (countAvailableCells(line, line[i], cur_row) > 1)
        // добавляем нижнюю стену
        maze_.grid_[cur_row][i] = maze_.grid_[cur_row][i] | Maze::Wall::DOWN;
    }
  }
}

int MazeBuilder::countAvailableCells(std::vector<int>& line, int set,
                                     int cur_row) {
  int counter = 0;
  for (int i = 0; i < cols_; i++) {
    // если текущая ячейка принадлежит множеству не имеет стены снизу
    if (line[i] == set &&
        (maze_.grid_[cur_row][i] & Maze::Wall::DOWN) != Maze::Wall::DOWN)
      counter++;
  }
  return counter;
}

void MazeBuilder::deleteWalls(std::vector<int>& line, int cur_row) {
  for (int i = 0; i < cols_; i++) {
    // удаляем правую стену
    maze_.grid_[cur_row][i] = maze_.grid_[cur_row][i] & ~Maze::Wall::RIGHT;
    // если у ячейки есть нижняя стена
    if ((maze_.grid_[cur_row][i] & Maze::Wall::DOWN) == Maze::Wall::DOWN) {
      // присваиваем ячейке пустое множество
      line[i] = 0;
      // удаляем нижнюю стену
      maze_.grid_[cur_row][i] = maze_.grid_[cur_row][i] & ~Maze::Wall::DOWN;
    }
  }
}

void MazeBuilder::resetMaze(int rows, int cols) {
  // сбрасываем параметры предыдущего лабиринта и устанавливаем новые
  if (rows >= 1 && cols >= 1) {
    rows_ = rows;
    cols_ = cols;
  } else {
    rows_ = 0;
    cols_ = 0;
  }
  set_counter_ = 0;
  maze_ = Maze(rows_, cols_);
}

void MazeBuilder::handleLastLine(std::vector<int>& line, int cur_row) {
  for (int j = 0; j < cols_ - 1; j++) {
    // добавляем стену снизу
    maze_.grid_[cur_row][j] = maze_.grid_[cur_row][j] | Maze::Wall::DOWN;
    // если множество текущей клетки и следующей не совпадают
    if (line[j] != line[j + 1]) {
      // удаляем правую стену
      maze_.grid_[cur_row][j] = maze_.grid_[cur_row][j] & ~Maze::Wall::RIGHT;
      // объединяем множества
      mergeSets(line, line[j], line[j + 1]);
    }
  }
  // нижней правой ячейке добавляем стены справа и снизу
  maze_.grid_[cur_row][cols_ - 1] =
      maze_.grid_[cur_row][cols_ - 1] | Maze::Wall::BOTH;
}