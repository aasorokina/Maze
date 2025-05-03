#include "test.h"

TEST(solve_maze, common) {
  MazeBuilder builder;
  Maze maze = builder.createMaze(5, 5);
  std::vector<std::pair<int, int>> solution = maze.getSolution(0, 0, 4, 4);

  EXPECT_EQ(maze.getCols(), 5);
  EXPECT_EQ(maze.getRows(), 5);
  EXPECT_FALSE(maze.graph_.empty());
  EXPECT_FALSE(maze.vertices_.empty());

  Maze::Vertex begin = maze.vertices_.front().front();
  Maze::Vertex test(0, 0, 0);
  EXPECT_EQ(begin.id, test.id);
  EXPECT_EQ(begin.row, test.row);
  EXPECT_EQ(begin.col, test.col);

  Maze::Vertex end = maze.vertices_.back().back();
  Maze::Vertex test2(24, 4, 4);
  EXPECT_EQ(end.id, test2.id);
  EXPECT_EQ(end.row, test2.row);
  EXPECT_EQ(end.col, test2.col);

  EXPECT_EQ(solution.front(), std::make_pair(0, 0));
  EXPECT_EQ(solution.back(), std::make_pair(4, 4));

  Maze maze2 = builder.createMaze(50, 50);
  std::vector<std::pair<int, int>> solution2 = maze2.getSolution(0, 49, 49, 0);

  EXPECT_EQ(maze2.getCols(), 50);
  EXPECT_EQ(maze2.getRows(), 50);
  EXPECT_FALSE(maze2.graph_.empty());
  EXPECT_FALSE(maze2.vertices_.empty());

  Maze::Vertex begin2 = maze2.vertices_.front().front();
  Maze::Vertex test3(0, 0, 0);
  EXPECT_EQ(begin2.id, test3.id);
  EXPECT_EQ(begin2.row, test3.row);
  EXPECT_EQ(begin2.col, test3.col);

  Maze::Vertex end2 = maze2.vertices_.back().back();
  Maze::Vertex test4(2499, 49, 49);
  EXPECT_EQ(end2.id, test4.id);
  EXPECT_EQ(end2.row, test4.row);
  EXPECT_EQ(end2.col, test4.col);

  EXPECT_EQ(solution2.front(), std::make_pair(0, 49));
  EXPECT_EQ(solution2.back(), std::make_pair(49, 0));
}

TEST(solve_maze, edge_cases) {
  MazeBuilder builder;
  Maze maze = builder.createMaze(0, 0);

  std::vector<std::pair<int, int>> solution = maze.getSolution(0, 0, 4, 4);

  EXPECT_TRUE(maze.grid_.empty());
  EXPECT_TRUE(maze.graph_.empty());
  EXPECT_TRUE(maze.vertices_.empty());
  EXPECT_TRUE(solution.empty());

  Maze maze2 = builder.createMaze(1, 1);
  std::vector<std::pair<int, int>> solution2 = maze2.getSolution(0, 0, 0, 0);

  EXPECT_TRUE(!maze2.grid_.empty());
  EXPECT_TRUE(!maze2.graph_.empty());
  EXPECT_TRUE(!maze2.vertices_.empty());
  EXPECT_FALSE(solution2.empty());
  EXPECT_EQ(solution2.front(), std::make_pair(0, 0));

  Maze maze3 = builder.createMaze(1, 2);
  std::vector<std::pair<int, int>> solution3 = maze3.getSolution(0, 0, 0, 1);

  EXPECT_FALSE(maze3.grid_.empty());
  EXPECT_FALSE(maze3.graph_.empty());
  EXPECT_FALSE(maze3.vertices_.empty());
  EXPECT_FALSE(solution3.empty());
  EXPECT_EQ(solution3.size(), 2);
  EXPECT_EQ(solution3.front(), std::make_pair(0, 0));
  EXPECT_EQ(solution3.back(), std::make_pair(0, 1));

  Maze maze4 = builder.createMaze(5, 5);
  std::vector<std::pair<int, int>> solution4 = maze4.getSolution(15, 0, 0, 10);
  EXPECT_TRUE(solution4.empty());

  Maze maze5 = builder.createMaze(5, 5);
  std::vector<std::pair<int, int>> solution5 =
      maze5.getSolution(-1, 3, 15, -10);
  EXPECT_TRUE(solution5.empty());
}