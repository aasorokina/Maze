#include <maze_view.h>

MazeView::MazeView(QWidget *parent) : QGraphicsView(parent) {
  scene_ = new QGraphicsScene(this);
  setScene(scene_);
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::NoDrag);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameShape(QFrame::NoFrame);
}

void MazeView::mousePressEvent(QMouseEvent *event) {
  QPointF scene_pos = mapToScene(event->pos());
  // поиск ячейки на сцене по координатам клика
  QGraphicsItem *item = scene()->itemAt(scene_pos, QTransform());

  if (item) {
    QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem *>(item);
    if (rect) {
      if (item->data(Cell::PRESSED).toBool() == false &&
          selected_points_.size() < 2) {
        if (!selected_points_.contains("start")) {
          setPoint("start", rect);
        } else if (!selected_points_.contains("finish")) {
          setPoint("finish", rect);
        }
      } else {
        if (*(selected_points_.find("start")) == rect) {
          removePoint("start", rect);
        } else if (*(selected_points_.find("finish")) == rect) {
          removePoint("finish", rect);
        }
      }
      sendSelectedPoints();
    }
  }
  update();
  QGraphicsView::mousePressEvent(event);
}

void MazeView::setMaze(std::vector<std::vector<int>> grid) {
  clearScene();
  maze_grid_ = grid;

  int rows = maze_grid_.size();
  int cols = maze_grid_[0].size();

  // установка размеров сцены равными размеру виджета
  scene_->setSceneRect(0, 0, rect().width(), rect().height());

  double cell_width = rect().width() / static_cast<double>(cols);
  double cell_height = rect().height() / static_cast<double>(rows);
  // расчет размера ячейки с учетом сохранения квадратной формы
  cell_size_ = std::min(cell_width, cell_height);
  // вычисление отступов для центированной отрисовки
  double offset_x = (rect().width() - (cell_size_ * cols)) / 2.0;
  double offset_y = (rect().height() - (cell_size_ * rows)) / 2.0;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      double x = offset_x + j * cell_size_;
      double y = offset_y + i * cell_size_;

      // создание ячейки и сохранение её данных
      QGraphicsRectItem *cell =
          scene_->addRect(x, y, cell_size_, cell_size_, QPen(Qt::NoPen));
      cell->setData(Cell::ROW, i);
      cell->setData(Cell::COL, j);
      cell->setData(Cell::PRESSED, false);
    }
  }
}

void MazeView::clearScene() {
  selected_points_.clear();
  clearMazeSolution();
  scene_->clear();
  maze_grid_.clear();
}

void MazeView::paintEvent(QPaintEvent *event) {
  QGraphicsView::paintEvent(event);

  if (maze_grid_.empty()) return;

  QPainter painter(viewport());

  int rows = maze_grid_.size();
  int cols = maze_grid_[0].size();

  double offset_x = (rect().width() - (cell_size_ * cols)) / 2.0;
  double offset_y = (rect().height() - (cell_size_ * rows)) / 2.0;

  QPen wall_pen(color_map[Color::WALL_COLOR], WALL_THICKNESS, Qt::SolidLine,
                Qt::FlatCap);
  painter.setPen(wall_pen);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      double x = offset_x + j * cell_size_;
      double y = offset_y + i * cell_size_;
      // отрисовка стены сверху ячейки
      if (i == 0) painter.drawLine(QPointF(x, y), QPointF(x + cell_size_, y));
      // отрисовка стены слева ячейки
      if (j == 0) painter.drawLine(QPointF(x, y), QPointF(x, y + cell_size_));
      // отрисовка стены справа ячейки
      if ((maze_grid_[i][j] & Wall::RIGHT) == Wall::RIGHT) {
        painter.drawLine(QPointF(x + cell_size_, y),
                         QPointF(x + cell_size_, y + cell_size_));
      }
      // отрисовка стены снизу ячейки
      if ((maze_grid_[i][j] & Wall::DOWN) == Wall::DOWN) {
        painter.drawLine(QPointF(x, y + cell_size_),
                         QPointF(x + cell_size_, y + cell_size_));
      }
    }
  }
  // отрисовка иконок на точках старта и финиша
  for (auto iter = selected_points_.begin(); iter != selected_points_.end();
       iter++) {
    QGraphicsRectItem *cell = iter.value();
    QPixmap icon(":/icons/" + iter.key() + "_icon.png");
    QRectF cell_rect = cell->sceneBoundingRect();
    QPointF top_left = mapFromScene(cell_rect.topLeft());
    QSize size = cell_rect.size().toSize();
    painter.drawPixmap(top_left, icon.scaled(size, Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
  }
}

void MazeView::sendSelectedPoints() {
  if (selected_points_.contains("start")) {
    QGraphicsItem *item = selected_points_["start"];
    emit startUpdated(item->data(Cell::ROW).toInt(),
                      item->data(Cell::COL).toInt());
  } else
    emit startUpdated(0, 0);

  if (selected_points_.contains("finish")) {
    QGraphicsItem *item = selected_points_["finish"];
    emit finishUpdated(item->data(Cell::ROW).toInt(),
                       item->data(Cell::COL).toInt());
  } else
    emit finishUpdated(0, 0);
}

void MazeView::paintMazeSolution(std::vector<std::pair<int, int>> path) {
  if (path.empty()) return;

  // удаляется предыдущее решение лабиринта
  clearMazeSolution();

  // удаляются текущие точки старта и финиша, они будут добавлены заново
  // исходя из пути
  if (selected_points_.contains("start"))
    removePoint("start", selected_points_["start"]);
  if (selected_points_.contains("finish"))
    removePoint("finish", selected_points_["finish"]);

  QPen pen(color_map[Color::PATH_COLOR], PATH_THICKNESS, Qt::DashLine,
           Qt::FlatCap);

  for (size_t i = 0; i < (path.size() - 1); i++) {
    // поиск текущей и следующей ячеек на сцене по координатам элементов пути
    QGraphicsRectItem *cell = findCell(path[i].first, path[i].second);
    QGraphicsRectItem *next_cell =
        findCell(path[i + 1].first, path[i + 1].second);

    if (cell == nullptr || next_cell == nullptr) return;

    QPointF cell_center = cell->sceneBoundingRect().center();
    QPointF next_cell_center = next_cell->sceneBoundingRect().center();

    // отрисовка линии между центрами двух ячеек
    scene_->addLine(QLineF(cell_center, next_cell_center), pen);

    // точки старта и финиша обновляются
    if (i == 0) {
      setPoint("start", cell);
    }
    if (i == path.size() - 2) {
      setPoint("finish", next_cell);
    }
  }
  update();
}

QGraphicsRectItem *MazeView::findCell(int row, int col) {
  for (QGraphicsItem *item : scene_->items()) {
    QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem *>(item);
    if (rect && rect->data(Cell::ROW).toInt() == row &&
        rect->data(Cell::COL).toInt() == col) {
      return rect;
    }
  }
  return nullptr;
}

void MazeView::clearMazeSolution() {
  for (QGraphicsItem *item : scene_->items()) {
    QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(item);
    if (line) {
      scene_->removeItem(line);
      delete line;
    }
  }
}

void MazeView::setPoint(QString type, QGraphicsRectItem *rect) {
  selected_points_.insert(type, rect);
  rect->setData(Cell::PRESSED, true);
}

void MazeView::removePoint(QString type, QGraphicsRectItem *rect) {
  selected_points_.remove(type);
  rect->setData(Cell::PRESSED, false);
}