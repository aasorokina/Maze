#ifndef MAZE_BUILDER_H
#define MAZE_BUILDER_H

#ifdef _WIN32
#include <time.h>
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include "maze.h"

/**
 * @class MazeBuilder
 * @brief Класс для генерации лабиринтов
 */
class MazeBuilder {
 public:
  /**
   * @brief Конструктор по умолчанию
   */
  MazeBuilder();

  /**
   * @brief Деструктор
   */
  ~MazeBuilder() = default;

  /**
   * @brief Генерирует лабиринт заданных размеров по алгоритму Эллера
   * @param[in] rows количество строк
   * @param[in] cols количество столбцов
   * @return экземпляр класса лабиринта
   */
  Maze createMaze(int rows, int cols);

 private:
  /**
   * @brief Если ячейка лабиринта не входит ни в одно множество, то
   * присваивает ей новое уникальное множество
   * @param[in] line ссылка на текущую строку лабиринта
   */
  void assignUniqueSet(std::vector<int> &line);

  /**
   * @brief Генерирует у ячеек текущей строки стены справа
   * @param[in] line ссылка на текущую строку лабиринта
   * @param[in] cur_row номер текущей строки лабиринта
   */
  void createRightWalls(std::vector<int> &line, int cur_row);

  /**
   * @brief Объединяет ячейки двух множеств в одно
   * @param[in] line ссылка на текущую строку лабиринта
   * @param[in] set1 первое множество
   * @param[in] set2 второе множество (присоединяется к первому)
   */
  void mergeSets(std::vector<int> &line, int set1, int set2);

  /**
   * @brief Генерирует у ячеек текущей строки стены снизу
   * @param[in] line ссылка на текущую строку лабиринта
   * @param[in] cur_row номер текущей строки лабиринта
   */
  void createDownWalls(std::vector<int> &line, int cur_row);

  /**
   * @brief Подсчитывает количество ячеек без нижней стены в указанном множестве
   * @param[in] line ссылка на текущую строку лабиринта
   * @param[in] set множество
   * @param[in] cur_row номер текущей строки лабиринта
   * @return количество ячеек без нижней стены
   */
  int countAvailableCells(std::vector<int> &line, int set, int cur_row);

  /**
   * @brief Удаляет стены у ячеек текущей строки и присваивает им
   * пустое множество. Используется при добавлении строк в лабиринт.
   * @param[in] line ссылка на текущую строку лабиринта
   * @param[in] cur_row номер текущей строки лабиринта
   */
  void deleteWalls(std::vector<int> &line, int cur_row);

  /**
   * @brief Очищает текущий лабиринт и устанавливает параметры для нового.
   * @param[in] rows количество строк
   * @param[in] cols количество столбцов
   */
  void resetMaze(int rows, int cols);

  /**
   * @brief Обрабатывает последнюю строку лабиринта - выборочно удаляет стены
   * справа и добавляет стены снизу всей строке.
   * @param[in] line ссылка на текущую строку лабиринта
   * @param[in] cur_row номер текущей строки лабиринта
   */
  void handleLastLine(std::vector<int> &line, int cur_row);

  /// количество строк
  int rows_;
  /// количество столбцов
  int cols_;
  /// счетчик множеств
  int set_counter_;
  /// экземпляр класса лабиринта
  Maze maze_;
};

#endif  // MAZE_BUILDER_H