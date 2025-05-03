#include "../include/mainwindow.h"

#include "../resources/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      maze_(Maze(0, 0)),
      bldr_(MazeBuilder()) {
  ui->setupUi(this);

  // пока лабиринт не создан/загружен, кнопки сохранения и решения не доступны
  ui->save_btn->setEnabled(false);
  ui->solve_maze_btn->setEnabled(false);
  ui->rows_create_spb->setValue(DEFAULT_ROWS);
  ui->cols_create_spb->setValue(DEFAULT_COLS);

  // настройка соединений кнопок
  connect(ui->generate_maze_btn, &QPushButton::pressed, this,
          &MainWindow::generateMaze);
  connect(ui->load_btn, &QPushButton::pressed, this, &MainWindow::loadFromFile);
  connect(ui->save_btn, &QPushButton::pressed, this, &MainWindow::saveToFile);
  connect(ui->maze_view, &MazeView::startUpdated, this,
          &MainWindow::updateStartPoint);
  connect(ui->maze_view, &MazeView::finishUpdated, this,
          &MainWindow::updateFinishPoint);
  connect(ui->solve_maze_btn, &QPushButton::pressed, this,
          &MainWindow::showMazeSolution);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::generateMaze() {
  int rows = ui->rows_create_spb->value();
  int cols = ui->cols_create_spb->value();
  maze_ = bldr_.createMaze(rows, cols);
  ui->maze_view->setMaze(maze_.grid());
  ui->save_btn->setEnabled(true);
  ui->solve_maze_btn->setEnabled(true);
  ui->file_name_lbl->clear();
  ui->loaded_maze_size_lbl->clear();
}

void MainWindow::loadFromFile() {
  QString file_path = QFileDialog::getOpenFileName(this, "Open file", "");
  if (!file_path.isEmpty()) {
    maze_ = MazeSerializer::readFromFile(file_path.toStdString());
    QFileInfo file_info(file_path);
    ui->file_name_lbl->setText(file_info.fileName());
    if (!maze_.isEmpty()) {
      ui->maze_view->setMaze(maze_.grid());
      ui->save_btn->setEnabled(true);
      ui->solve_maze_btn->setEnabled(true);
      QString size = QString::number(maze_.getRows()) + "x" +
                     QString::number(maze_.getCols());
      ui->loaded_maze_size_lbl->setText(size);
    } else {
      ui->maze_view->clearScene();
      ui->save_btn->setEnabled(false);
      ui->solve_maze_btn->setEnabled(false);
      ui->loaded_maze_size_lbl->clear();
    }
  }
}

void MainWindow::saveToFile() {
  QString file_name =
      "Maze_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss") +
      ".txt";
  QString file_type("Text (*.txt)");
  QString file_path =
      QFileDialog::getSaveFileName(this, "Save maze", file_name, file_type);
  if (!file_path.isEmpty())
    MazeSerializer::saveToFile(maze_, file_path.toStdString());
}

void MainWindow::updateStartPoint(int x, int y) {
  ui->start_row_spb->setValue(x);
  ui->start_col_spb->setValue(y);
}

void MainWindow::updateFinishPoint(int x, int y) {
  ui->finish_row_spb->setValue(x);
  ui->finish_col_spb->setValue(y);
}

void MainWindow::showMazeSolution() {
  int max_row = maze_.getRows() - 1;
  int max_col = maze_.getCols() - 1;

  // проверка заданных точек старта и финиша решения лабиринта. при выходе за
  // границы будут установлены максимальные корректные для текущего
  // лабиринта значения
  int row_start = qBound(0, ui->start_row_spb->value(), max_row);
  int col_start = qBound(0, ui->start_col_spb->value(), max_col);
  int row_finish = qBound(0, ui->finish_row_spb->value(), max_row);
  int col_finish = qBound(0, ui->finish_col_spb->value(), max_col);

  // получаем путь (решение лабиринта) и передаем в виджет отрисовки
  auto path = maze_.getSolution(row_start, col_start, row_finish, col_finish);
  ui->maze_view->paintMazeSolution(path);

  // обновляем значения в спинбоксах в соответствии с решением
  if (row_start != ui->start_row_spb->value())
    ui->start_row_spb->setValue(row_start);
  if (col_start != ui->start_col_spb->value())
    ui->start_col_spb->setValue(col_start);

  if (row_finish != ui->finish_row_spb->value())
    ui->finish_row_spb->setValue(row_finish);
  if (col_finish != ui->finish_col_spb->value())
    ui->finish_col_spb->setValue(col_finish);
}