#ifndef MAZE_SERIALIZER_H
#define MAZE_SERIALIZER_H

#include <fstream>

#include "maze.h"

/**
 * @class MazeSerializer
 * @brief Класс для чтения и записи лабиринтов из файла/в файл
 */
class MazeSerializer {
 public:
  /**
   * @brief Конструктор по умолчанию
   */
  MazeSerializer() = default;

  /**
   * @brief Деструктор
   */
  ~MazeSerializer() = default;

  /**
   * @brief Считывает лабиринт из файла по указанному пути
   * @param[in] file_path путь к файлу
   * @return экземпляр класса лабиринта
   */
  static Maze readFromFile(const std::string file_path);

  /**
   * @brief Сохраняет лабиринт в файл по указанному пути
   * @param[in] file_path путь к файлу
   */
  static void saveToFile(Maze maze, const std::string file_path);
};

#endif  // MAZE_SERIALIZER_H