SHELL = /bin/sh

CXX := g++
CXXSTANDARD := 20
CXXFLAGS := -Wall -Werror -Wextra -g -std=c++$(CXXSTANDARD)
LFLAGS := -lpthread -lgtest -pthread -lstdc++ -lm
GFLAGS := -fprofile-arcs -ftest-coverage

BUILD_DIR := build
EXECUTABLE := Maze

TESTS_BUILD_DIR := tests/build
TESTS_EXECUTABLE := maze_tests

GCOV_NAME := gcov_tests.info
REPORT_DIR := report

DOC_DIR := docs
DIST_DIR := Maze-1.0
DIST_NAME := Maze
DIST_FILE := Maze-1.0.tar.gz

.PHONY: all install uninstall run clean tests valgrind_test gcov_report style_check style_fix dvi dist
all: install

install:
	@echo "==> Установка $(EXECUTABLE) ..."
	@mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && \
	cmake .. -DCMAKE_CXX_COMPILER=$(CXX) \
	-DCMAKE_CXX_STANDARD=$(CXXSTANDARD) \
	-DCMAKE_CXX_FLAGS="$(CXXFLAGS)" && \
	cmake --build .
	@echo "==> Готово!"

uninstall:
	@echo "==> Удаление $(EXECUTABLE) ..."
	@rm -rf $(BUILD_DIR)

run:
	@echo "==> Запуск $(EXECUTABLE) ..."
	@./$(BUILD_DIR)/$(EXECUTABLE)

clean:
	@echo "==> Очистка..."
	@rm -rf $(REPORT_DIR)
	@rm -rf $(TESTS_BUILD_DIR) 
	@rm -rf $(GCOV_NAME)
	@rm -rf $(DIST_FILE)
	@rm -rf $(DOC_DIR)
	@rm -rf $(DIST_FILE)
	@rm -rf .clang-format

dvi:
	@echo "==> Генерация документации ..."
	@mkdir -p $(DOC_DIR)
	@doxygen Doxyfile > /dev/null 2>&1
	@cd $(DOC_DIR)/latex && xelatex -interaction=batchmode refman.tex > /dev/null 2>&1
	@echo "==> Готово!"

dist:
	@mkdir -p $(DIST_DIR)/
	@cp $(BUILD_DIR)/$(EXECUTABLE) ./$(DIST_DIR)/$(EXECUTABLE)
	tar cvzf $(DIST_FILE) $(DIST_DIR)/
	@rm -rf $(DIST_DIR)/

tests:
	@echo "==> Запуск тестов ..."
	@mkdir -p $(TESTS_BUILD_DIR) && cd $(TESTS_BUILD_DIR) && \
	cmake .. -DCMAKE_CXX_COMPILER=$(CXX) \
	-DCMAKE_CXX_STANDARD=$(CXXSTANDARD) \
	-DCMAKE_CXX_FLAGS="-DTEST $(CXXFLAGS) $(LFLAGS)" && \
	cmake --build .
	@./$(TESTS_BUILD_DIR)/$(TESTS_EXECUTABLE)

gcov_report:
	@echo "==> Генерация отчета о покрытии ..."
	@mkdir -p $(TESTS_BUILD_DIR) && cd $(TESTS_BUILD_DIR) && \
	cmake .. -DCMAKE_CXX_COMPILER=$(CXX) \
	-DENABLE_COVERAGE=ON \
	-DCMAKE_CXX_STANDARD=$(CXXSTANDARD) \
	-DCMAKE_CXX_FLAGS="-DTEST $(CXXFLAGS)" \
	-DCMAKE_EXE_LINKER_FLAGS="$(LFLAGS) $(GFLAGS)" && \
	cmake --build .
	@./$(TESTS_BUILD_DIR)/$(TESTS_EXECUTABLE)
	@lcov -t "gcov_tests" -o $(GCOV_NAME) -c -d $(TESTS_BUILD_DIR) \
		--exclude '/usr/include/c++/11/*' \
		--exclude '/usr/include/c++/11/bits/*' \
		--exclude '/usr/include/c++/11/ext/*' \
		--exclude '/usr/include/gtest/*' \
		--exclude '/usr/include/gtest/internal/*' \
		--exclude './tests/*'
	@genhtml -o $(REPORT_DIR) $(GCOV_NAME)
	@rm -f *.gcno *.gcda $(GCOV_NAME)
	
valgrind_test: tests
	valgrind --tool=memcheck --leak-check=yes --show-leak-kinds=all --track-origins=yes -s ./$(TESTS_BUILD_DIR)/$(TESTS_EXECUTABLE)

style_check:
	@cp materials/linters/.clang-format .clang-format
	@find . -name '*.cpp' -o -name '*.h' | xargs clang-format -Werror -n
	@rm -rf .clang-format

style_fix:
	@cp materials/linters/.clang-format .clang-format
	@find . -name '*.cpp' -o -name '*.h' | xargs clang-format -Werror -i
	@rm -rf .clang-format