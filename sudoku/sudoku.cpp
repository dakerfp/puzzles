
#include <set>
#include <tuple>
#include <string>


struct sudoku
{
	int b[9][9];
	sudoku();
	bool valid_row(int row, int v=0) const;
	bool valid_col(int col, int v=0) const;
	bool valid_blk(int col, int row, int v=0) const;
	bool can_insert(int col, int row, int v=0) const;
	std::tuple<int,int> next_cell_to_fill() const;
	std::string str() const;
	std::string repr() const;
};

sudoku::sudoku() {
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			b[i][j] = 0;
}

bool sudoku::valid_row(int row, int v) const {
	std::set<int> numbers;
	if (v != 0) numbers.insert(v);
	for (int j = 0; j < 9; j++) {
		int n = b[row][j];
		if (n == 0) continue;
		if (numbers.count(n) != 0) return false;
		numbers.insert(n);
	}
	return true;
}

bool sudoku::valid_col(int col, int v) const {
	std::set<int> numbers;
	if (v != 0) numbers.insert(v);
	for (int i = 0; i < 9; i++) {
		int n = b[i][col];
		if (n == 0) continue;
		if (numbers.count(n) != 0) return false;
		numbers.insert(n);
	}
	return true;
}

bool sudoku::valid_blk(int blkrow, int blkcol, int v) const {
	std::set<int> numbers;
	if (v != 0) numbers.insert(v);
	for (int i = blkrow * 3; i < blkrow * 3 + 3; i++) {
		for (int j = blkcol * 3; j < blkcol * 3 + 3; j++) {
			int n = b[i][j];
			if (n == 0) continue;
			if (numbers.count(n) != 0) return false;
			numbers.insert(n);
		}
	}
	return true;
}

bool sudoku::can_insert(int row, int col, int v) const {
	if (b[row][col] != 0) return false;
	if (!valid_row(row, v)) return false;
	if (!valid_col(col, v)) return false;
	if (!valid_blk(row/3, col/3, v)) return false;
	return true;
}

std::tuple<int,int> sudoku::next_cell_to_fill() const {
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (b[i][j] == 0) return {i,j};
	return {-1,-1};
}

std::string sudoku::repr() const {
	std::string s = "------------\n";
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; ++j) {
			if (b[i][j] == 0) s += " ";
			else s += '0' + b[i][j];
			if (j % 3 == 2) s += "|";
		}
		if (i % 3 == 2) s += "\n------------\n";
		else s += "\n";
	}
	return s;
}

std::string sudoku::str() const {
	std::string s = "";
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; ++j) {
			s += '0' + b[i][j];
		}
	}
	return s;
}

bool solve(sudoku &s, int &backtrack) {
	auto [i,j] = s.next_cell_to_fill();
	if (i < 0 || j < 0) return true;
	for (int v = 1; v < 10; v++) {
		if (s.can_insert(i, j, v)) {
			s.b[i][j] = v;
			if (solve(s, backtrack)) return true;
			backtrack++;
			s.b[i][j] = 0;
		}
	}
	return false;
}

sudoku parse_sudoku(std::string s) {
	sudoku sud;
	int idx = 0;
	for (int i = 0; i < 9; i++)	{
		for (int j = 0; j < 9; j++)	{
			sud.b[i][j] = s[idx++] - '0';
		}
	}
	return sud;
}

#include <iostream>
#include <cassert>

void test_solver() {
	struct tt {std::string quizzle, solution; };
	tt tts[10] = { // https://www.kaggle.com/bryanpark/sudoku
		{"004300209005009001070060043006002087190007400050083000600000105003508690042910300", "864371259325849761971265843436192587198657432257483916689734125713528694542916378"},
		{"040100050107003960520008000000000017000906800803050620090060543600080700250097100", "346179258187523964529648371965832417472916835813754629798261543631485792254397186"},
		{"600120384008459072000006005000264030070080006940003000310000050089700000502000190", "695127384138459672724836915851264739273981546946573821317692458489715263562348197"},
		{"497200000100400005000016098620300040300900000001072600002005870000600004530097061", "497258316186439725253716498629381547375964182841572639962145873718623954534897261"},
		{"005910308009403060027500100030000201000820007006007004000080000640150700890000420", "465912378189473562327568149738645291954821637216397854573284916642159783891736425"},
		{"005910308009403060027500100030000201000820007006007004000080000640150700890000420", "465912378189473562327568149738645291954821637216397854573284916642159783891736425"},
		{"009065430007000800600108020003090002501403960804000100030509007056080000070240090", "289765431317924856645138729763891542521473968894652173432519687956387214178246395"},
		{"000000657702400100350006000500020009210300500047109008008760090900502030030018206", "894231657762495183351876942583624719219387564647159328128763495976542831435918276"},
		{"503070190000006750047190600400038000950200300000010072000804001300001860086720005", "563472198219386754847195623472638519951247386638519472795864231324951867186723945"},
		{"060720908084003001700100065900008000071060000002010034000200706030049800215000090", "163725948584693271729184365946358127371462589852917634498231756637549812215876493"},
	};

	for (int i = 0; i < 10; i++) {
		std::cout << i << std::endl;
		auto t = tts[i];
		auto s = parse_sudoku(t.quizzle);
		int b = 0;
		assert(solve(s, b));
		assert(s.str() == t.solution);
		std::cout << s.repr();
	}
}


int main() {
	sudoku s = parse_sudoku("004300209005009001070060043006002087190007400050083000600000105003508690042910300");
	std::cout << s.str() << std::endl;
	int b = 0;
	solve(s,b);
	std::cout << s.str() << std::endl;
	test_solver();
	return 0;
}