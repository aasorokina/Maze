#ifndef MAZE_H
#define MAZE_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

/**
 * @class Maze
 * @brief Класс лабиринта. Хранит матрицу стен лабиринта, матрицу вершин
 * лабиринта и список смежности графа
 */
class Maze {
 public:
  friend class MazeBuilder;
  friend class MazeSerializer;

  /**
   * @enum Wall
   * @brief Определяет тип стены в ячейке лабиринта.
   */
  enum Wall {
    NONE = 0b00,   ///< Стен нет
    RIGHT = 0b01,  ///< Стена справа
    DOWN = 0b10,   ///< Стена снизу
    BOTH = 0b11    ///< Стены справа и снизу
  };

  /**
   * @struct Vertex
   * @brief Структура вершины для создания списка смежности графа лабиринта.
   * Одна вершина соответствует одной ячейке лабиринта.
   */
  struct Vertex {
    int id;   ///< Уникальный номер вершины
    int row;  ///< Номер строки
    int col;  ///< Номер столбца
  };

  /**
   * @brief Конструктор
   * @param[in] rows количество строк
   * @param[in] cols количество столбцов
   */
  Maze(int rows, int cols);

  /**
   * @brief Деструктор
   */
  ~Maze() = default;

  /**
   * @brief Возвращает количество столбцов в лабиринте
   * @return Количество столбцов
   */
  int getCols();

  /**
   * @brief Возвращает количество строк в лабиринте
   * @return Количество строк
   */
  int getRows();

  /**
   * @brief Проверяет, является ли лабиринт пустым
   * @return true, если лабиринт пустой, иначе false
   */
  bool isEmpty();

  // void print();

  /**
   * @brief Возвращает матрицу стен лабиринта
   * @return Матрица стен лабиринта
   */
  std::vector<std::vector<int>> grid();

  /**
   * @brief Вычисляет решение лабиринта по заданным точкам с помощью алгоритма
   * обхода в ширину (BFS) и возвращает вектор пар (строка и столбец) координат
   * пути
   * @param[in] row1 строка точки старта
   * @param[in] col1 столбец точки старта
   * @param[in] row2 строка точки финиша
   * @param[in] col2 столбец точки финиша
   * @return Координаты пути
   */
  std::vector<std::pair<int, int>> getSolution(int row1, int col1, int row2,
                                               int col2);

#ifdef TEST
 public:
#else
 private:
#endif

  /**
   * @brief Индексирует вершины лабиринта
   */
  void indexVertices();

  /**
   * @brief Генерирует список смежности по вершинам лабиринта, учитывая наличие
   * стен
   */
  void generateGraph();

  /**
   * @brief Проверяет, являются ли указанные координаты допустимыми точками в
   * лабиринте
   * @param[in] x1 строка точки старта
   * @param[in] y1 столбец точки старта
   * @param[in] x2 строка точки финиша
   * @param[in] y2 столбец точки финиша
   * @return true, если точки корректные, иначе false
   */
  bool isValidPoints(int x1, int y1, int x2, int y2);

  /// количество строк в лабиринте
  int rows_;
  /// количество столбцов в лабиринте
  int cols_;
  /// матрица стен лабиринта
  std::vector<std::vector<int>> grid_;
  /// матрица вершин лабиринта
  std::vector<std::vector<Vertex>> vertices_;
  /// представление лабиринта в виде списка смежности графа
  std::vector<std::vector<int>> graph_;
};

#endif  // MAZE_H