#include "test.h"

TEST(generate_maze, common) {
  MazeBuilder builder;
  Maze maze = builder.createMaze(5, 5);

  EXPECT_EQ(maze.getCols(), 5);
  EXPECT_EQ(maze.getRows(), 5);
  EXPECT_FALSE(maze.grid().empty());
  EXPECT_FALSE(maze.isEmpty());

  Maze maze2 = builder.createMaze(50, 50);
  EXPECT_EQ(maze2.getCols(), 50);
  EXPECT_EQ(maze2.getRows(), 50);
  EXPECT_FALSE(maze2.grid().empty());
  EXPECT_FALSE(maze2.isEmpty());

  Maze maze3 = builder.createMaze(100, 100);
  EXPECT_EQ(maze3.getCols(), 100);
  EXPECT_EQ(maze3.getRows(), 100);
  EXPECT_FALSE(maze3.grid().empty());
  EXPECT_FALSE(maze3.isEmpty());

  Maze maze4 = builder.createMaze(10, 30);
  EXPECT_EQ(maze4.getCols(), 30);
  EXPECT_EQ(maze4.getRows(), 10);
  EXPECT_FALSE(maze4.grid().empty());
  EXPECT_FALSE(maze4.isEmpty());
}

TEST(generate_maze, edge_cases) {
  MazeBuilder builder;
  Maze maze = builder.createMaze(0, 0);

  EXPECT_EQ(maze.getCols(), 0);
  EXPECT_EQ(maze.getRows(), 0);
  EXPECT_TRUE(maze.grid().empty());
  EXPECT_TRUE(maze.isEmpty());

  Maze maze2 = builder.createMaze(0, 1);
  EXPECT_EQ(maze2.getCols(), 0);
  EXPECT_EQ(maze2.getRows(), 0);
  EXPECT_TRUE(maze2.grid().empty());
  EXPECT_TRUE(maze2.isEmpty());

  Maze maze3 = builder.createMaze(-10, -50);
  EXPECT_EQ(maze3.getCols(), 0);
  EXPECT_EQ(maze3.getRows(), 0);
  EXPECT_TRUE(maze3.grid().empty());
  EXPECT_TRUE(maze3.isEmpty());

  Maze maze4 = builder.createMaze(1, 1);
  EXPECT_EQ(maze4.getCols(), 1);
  EXPECT_EQ(maze4.getRows(), 1);
  EXPECT_FALSE(maze4.grid().empty());
  EXPECT_FALSE(maze4.isEmpty());

  Maze maze5 = builder.createMaze(1, 0);
  EXPECT_EQ(maze5.getCols(), 0);
  EXPECT_EQ(maze5.getRows(), 0);
  EXPECT_TRUE(maze5.grid().empty());
  EXPECT_TRUE(maze5.isEmpty());
}