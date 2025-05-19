#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <string>

//testing for git commit.

// Type defs alias
using vvChar = std::vector<std::vector<char>>;

auto PrintMatrix(const vvChar &MAZE) -> void {
	for (const auto &x : MAZE) {
		for (const auto& y : x) {
			std::cout << y << " ";
		}
		std::cout << "\n";
	}
}

auto PrintMatrixVisited(const vvChar &visited) -> void {
	for (const auto& x : visited) {
		for (const auto& y : x) {
			std::cout << std::boolalpha << y << " ";
		}
		std::cout << "\n";
	}
}

auto PrintShortestPathMaze(const vvChar& MAZE, const std::string& moves, const int& row, const int& col) -> void {
	int x = row, y = col;
	std::string path = moves.substr(0, moves.length() - 1);
	std::set<std::pair<int, int>> pos;
	for (char move : path) {
		if (move == 'N')
			x -= 1;
		else if (move == 'S')
			x += 1;
		else if (move == 'E')
			y += 1;
		else if (move == 'W')
			y -= 1;
		pos.insert({ x, y });
	}

	for (size_t i=0; i < MAZE.size(); i++) {
		for (size_t j=0; j < MAZE[0].size(); j++) {
			bool skip = false;
			for (auto it = pos.begin(); it != pos.end(); it++) {
				if (it->first == i && it->second == j) {
					std::cout << "+ ";
					skip = true;
				}
			}
			if(!skip)
				std::cout << MAZE[i][j] << " ";
		}
		std::cout << "\n";
	}
}

auto findEnd(const vvChar &MAZE, const std::string &moves, const int &row, const int &col) -> bool{
	if (moves.length() > 0) {
		int x = row, y = col;

		for (char move : moves) {
			if (move == 'N')
				x -= 1;
			else if (move == 'S')
				x += 1;
			else if (move == 'E')
				y += 1;
			else if (move == 'W')
				y -= 1;
		}

		if (MAZE[x][y] == 'E') {
			std::cout << "Found :" << moves << "\n";
			PrintShortestPathMaze(MAZE, moves, row, col);
			return true;
		}

		return false;
	}
	else
		return false;
}

auto valid(const vvChar &MAZE, const std::string &moves, const int &row, const int &col) -> bool {
	if (moves.length() > 0) {
		int x = row, y = col;

		for (char move : moves) {
			if (move == 'N')
				x -= 1;
			else if (move == 'S')
				x += 1;
			else if (move == 'E')
				y += 1;
			else if (move == 'W')
				y -= 1;

			if (x < 0 || y < 0)
				return false;
			else if (x >= MAZE.size() || y >= MAZE[0].size())
				return false;
			else if (MAZE[x][y] == '#')
				return false;
		}

		return true;
	}
	else
		return false;
}

auto main() -> int {
	//read file input
	std::ifstream icin("input.txt", std::ios::in);
	if (icin.good() == false) {
		std::cout << "Cannot open file.\n";
		exit(EXIT_FAILURE);
	}

	//R # of rows and C # of columns of the grid
	int R, C;
	icin >> R >> C;
	icin.ignore();

	//store the input matrix
	std::vector<char> col(C);
	vvChar MAZE(R, col);

	//initialize 
	int sr, sc; //start position in the matrix
	int er, ec;
	int reached_end = false;
	int move_count = 0;
	int nodes_left_in_layer = 1;
	int nodes_in_next_layer = 0;
	std::queue<int> rq, cq;

	//initialize boolean matrix to keep track of the visited nodes / cells
	std::vector<bool> bool_col(C, false);
	std::vector<std::vector<bool>> visited(R, bool_col);

	//direction vectors     N,  S,  E, W
	std::vector<int> dr = { -1, +1, 0, 0 }; // x -> row 
	std::vector<int> dc = {  0,  0,+1,-1 }; // y -> col

	std::string line;
	int i = 0;
	while (std::getline(icin, line)) {
		for (size_t j = 0; j < line.size(); j++) {
			MAZE[i][j] = line[j];
			if (line[j] == 'S') { //find the starting position while scanning input
				sr = i;
				sc = j;
			}
		}
		i++;
	}

	PrintMatrix(MAZE);

	//initilalize queue with S position vectors
	rq.push(sr);
	cq.push(sc);

	//mark the boolean array as true at the starting position
	visited[sr][sc] = true;

	while (rq.size() > 0) {
		int r = rq.front(); rq.pop();
		int c = cq.front(); cq.pop();

		//check to see if we reach the end
		if (MAZE[r][c] == 'E') {
			reached_end = true;
			er = r; ec = c;
			break;
		}

		//direction locations
		int rr, cc; 
		//explore neighbours from the current node
		for (int i = 0; i < 4; i++) {
			rr = r + dr[i];
			cc = c + dc[i];

			//skip out of bound location
			if (rr < 0 || cc < 0) continue;
			if (rr >= R || cc >= C) continue;

			//skip the visited location or blocked cells
			if (visited[rr][cc]) continue; 
			if (MAZE[rr][cc] == '#') continue;
			
			rq.push(rr);
			cq.push(cc);
			visited[rr][cc] = true;
			nodes_in_next_layer++;
		}

		nodes_left_in_layer--;

		if (nodes_left_in_layer == 0) {
			nodes_left_in_layer = nodes_in_next_layer;
			nodes_in_next_layer = 0;
			move_count++;
		}
	}

	if (reached_end)
		std::cout << "Total count:" << move_count << "\n";
	else
		std::cout << "Couldn't find the path.\n";

	//create the Main Queue 
	std::queue<std::string> MainQ;

	//initilaize the queue with the starting co-ordinate of the maze
	MainQ.push("");
	std::string add;

	//Main Loop 
	while (!findEnd(MAZE, add, sr, sc)) {
		add = MainQ.front(); MainQ.pop();

		for (char k : { 'N', 'S', 'E', 'W' }) { //scan all the 4 directions in N S E W
			std::string dxy = add + k;

			if (valid(MAZE, dxy, sr, sc)) {
				MainQ.push(dxy);
			}
		}
	}

	std::cin.get();
	return 0;
}