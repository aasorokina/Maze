#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QFontDatabase>
#include <QMainWindow>
#include <QtWidgets>

#include "maze.h"
#include "maze_builder.h"
#include "maze_serializer.h"
#include "maze_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Основное окно приложения. Содержит элементы управления и область
 * отрисовки лабиринта
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

  /// Количество строк по умолчанию для генерации лабиринта
  constexpr static int DEFAULT_ROWS = 10;
  /// Количество столбцов по умолчанию для генерации лабиринта
  constexpr static int DEFAULT_COLS = 10;

 public:
  /**
   * @brief Конструктор
   * @param[in] parent родитель
   */
  MainWindow(QWidget *parent = nullptr);

  /**
   * @brief Деструктор
   */
  ~MainWindow();

 public slots:

  /**
   * @brief Слот для генерации лабиринта на заданное количество строк и
   * столбцов (указываются в спинбоксах)
   */
  void generateMaze();

  /**
   * @brief Слот для загрузки лабиринта из файла
   */
  void loadFromFile();

  /**
   * @brief Слот для сохранения лабиринта в файл
   */
  void saveToFile();

  /**
   * @brief Слот для обновления спинбоксов начальной точки решения лабиринта.
   * Вызывается, когда точка старта выбрана мышкой.
   * @param[in] x строка точки старта
   * @param[in] y столбец точки старта
   */
  void updateStartPoint(int x, int y);

  /**
   * @brief Слот для обновления спинбоксов конечной точки решения лабиринта.
   * Вызывается, когда точка финиша выбрана мышкой.
   * @param[in] x строка точки финиша
   * @param[in] y столбец точки финиша
   */
  void updateFinishPoint(int x, int y);

  /**
   * @brief Слот для генерации и отображения решения текущего лабиринта
   */
  void showMazeSolution();

 private:
  /// UI-форма
  Ui::MainWindow *ui;
  /// Экземпляр лабиринта
  Maze maze_;
  /// Экземпляр строителя лабиринта
  MazeBuilder bldr_;
};
#endif  // MAINWINDOW_H
