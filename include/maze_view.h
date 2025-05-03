#ifndef MAZE_WIDGET_H
#define MAZE_WIDGET_H

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <vector>

/**
 * @class MazeView
 * @brief Класс для отрисовки лабиринта, его решения и интерактивного
 * взаимодействия с ним.
 *
 * Позволяет отдельно отрисовать сетку лабиринта, решения, с помощью мыши
 * указать точки старта и финиша для решения лабиринта.
 *
 * @note Унаследован от QGraphicsView
 */
class MazeView : public QGraphicsView {
  Q_OBJECT
  using QGraphicsView::QGraphicsView;

  /**
   * @enum Wall
   * @brief Определяет тип стены в ячейке лабиринта.
   */
  enum Wall {
    RIGHT = 0b01,  ///< Стена справа
    DOWN = 0b10    ///< Стена снизу
  };

  /**
   * @enum Cell
   * @brief Определяет данные, которые хранит элемент для отрисовки ячейки
   * лабиринта.
   */
  enum Cell {
    ROW = 0,     ///< Номер строки
    COL = 1,     ///< Номер столбца
    PRESSED = 2  ///< Флаг нажатия
  };

  /**
   * @enum Color
   * @brief Определяет цвета для отрисовки стен и пути в лабиринте.
   */
  enum Color {
    WALL_COLOR = 0,  ///< Цвет отрисовки стен
    PATH_COLOR = 1   ///< Цвет отрисовки пути
  };

  /// Толщина отрисовки стены лабиринта в пикселях
  constexpr static double WALL_THICKNESS = 2.0;
  /// Толщина отрисовки пути лабиринта в пикселях
  constexpr static double PATH_THICKNESS = 2.0;
  /// Словарь для хранения цветов отрисовки
  const QMap<Color, QColor> color_map = {{Color::WALL_COLOR, QColor(Qt::white)},
                                         {Color::PATH_COLOR, QColor(Qt::blue)}};

 public:
  /**
   * @brief Конструктор
   * @param[in] parent родитель
   */
  explicit MazeView(QWidget *parent = nullptr);

  /**
   * @brief Деструктор
   */
  ~MazeView() = default;

  /**
   * @brief Создает матрицу элементов (QGraphicsRectItem) для последующей
   * отрисовки лабиринта, взаимодействия мышкой. Матрица подгоняется под размер
   * виджета в зависимости от количества ячеек в лабирите. Каждый
   * QGraphicsRectItem соответствует одной клетке лабиринта, хранит номер строки
   * и столбца, информацию о клике по нему.
   * @param[in] grid матрица со стенами лабиринта
   */
  void setMaze(std::vector<std::vector<int>> grid);

  /**
   * @brief Очищает сцену, матрицу QGraphicsRectItem, путь решения лабиринта и
   * матрицу стен лабиринта
   */
  void clearScene();

  /**
   * @brief Отправляет сигналы в MainWindow, если ячейки
   * старта и фишина для решения лабиринта были выбраны мышкой
   */
  void sendSelectedPoints();

  /**
   * @brief Отрисовывает путь решения лабиринта с помощью отдельных
   * элементов-линий (QGraphicsLineItem)
   * @param[in] path путь решения лабиринта
   */
  void paintMazeSolution(std::vector<std::pair<int, int>> path);

 signals:
  /**
   * @brief Сигнал для обновления координат старта пути
   * в спинбоксах на MainWindow
   * @param[in] x строка ячейки старта
   * @param[in] y столбец ячейки старта
   */
  void startUpdated(int x, int y);

  /**
   * @brief Сигнал для обновления координат финиша пути
   * в спинбоксах на MainWindow
   * @param[in] x строка ячейки финиша
   * @param[in] y столбец ячейки финиша
   */
  void finishUpdated(int x, int y);

 protected:
  /**
   * @brief Обрабатывает событие нажатия мыши на виджете лабиринта, определяет
   * выбранную ячейку лабиринта при клике и устанавливает точки старта или
   * финиша в зависимости от текущего состояния
   * @param[in] event указатель на событие мыши
   */
  void mousePressEvent(QMouseEvent *event) override;

  /**
   * @brief Переопределяет событие перерисовки виджета лабиринта
   * @param[in] event указатель на событие перерисовки
   */
  void paintEvent(QPaintEvent *event) override;

  /**
   * @brief Находит элемент (QGraphicsRectItem) сцены виджета отрисовки по
   * указанным координатам строки и столбца
   * @param[in] row строка ячейки в лабиринте
   * @param[in] col столбец ячейки в лабиринте
   * @return Указатель на QGraphicsRectItem или nullptr,
   * если ячейка не найдена
   */
  QGraphicsRectItem *findCell(int row, int col);

  /**
   * @brief Очищает решение лабиринта - удаляет все линии для его изображения
   */
  void clearMazeSolution();

  /**
   * @brief Добавляет ячейку с указанным ключом в словарь для хранения
   * точек старта и финиша решения лабиринта
   * @param[in] type тип ячейки и её ключ в словаре
   * @param[in] rect ячейка (QGraphicsRectItem)
   */
  void setPoint(QString type, QGraphicsRectItem *rect);

  /**
   * @brief Удаляет ячейку с указанным ключом из словаря для хранения
   * точек старта и финиша решения лабиринта
   * @param[in] type тип ячейки и её ключ в словаре
   * @param[in] rect ячейка (QGraphicsRectItem)
   */
  void removePoint(QString type, QGraphicsRectItem *rect);

 private:
  /// Указатель на графическую сцену для отрисовки элементов лабиринта
  QGraphicsScene *scene_;
  /// Словарь для хранения точек старта и финиша решения лабиринта
  QMap<QString, QGraphicsRectItem *> selected_points_;
  /// Размер ячейки лабиринта
  double cell_size_;
  /// Матрица со стенами лабиринта
  std::vector<std::vector<int>> maze_grid_;
};

#endif  // MAZE_WIDGET_H