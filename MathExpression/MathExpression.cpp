// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <string>
#include <vector>
#include <iostream>

using namespace std;

void parse_st(string &st, vector<string> &sa) {
	// Tokenize string
	int num_start = -1;
	for (int i = 0; i < st.length(); ++i) {
		if (isdigit(st[i])) {
			if (num_start == -1) {
				num_start = i;
			}
			else {
				continue;
			}
		}
		else {
			if (num_start > -1) {
				sa.push_back(st.substr(num_start, i - num_start));
				num_start = -1;
			}
			// Detect negative numbers
			if (st[i] == '-' && i < st.length() - 1 && isdigit(st[i + 1])) {
				if (!i || st[i - 1] == '(' || st[i - 1] == '*' || st[i - 1] == '/') {
					num_start = i;
					continue;
				}
			}
			sa.push_back(st.substr(i, 1));
		}
	}
	if (num_start > -1) {
		sa.push_back(st.substr(num_start, st.length() - num_start));
	}
}

int calc(vector<string> &sa, int pos) {
	if (pos >= sa.size()) {
		return 1;
	}
	// Calculate parentheses
	for (int i = pos; i < sa.size(); ++i) {
		if (sa[i] == "(") {
			int err = calc(sa, i + 1);
			if (err) return err;
			if (i >= sa.size()) return 2;
		}
		if (sa[i] == ")") {
			break;
		}
	}
	// Calculate equation
	for (int layer = 0; layer < 2; ++layer) {
		for (int i = pos; i < sa.size(); ++i) {
			if (sa[i] == "*" || sa[i] == "/") {
				if (!i) return 3;
				if (sa[i] == "*") {
					sa[i - 1] = to_string(stof(sa[i - 1]) * stof(sa[i + 1]));
				}
				if (sa[i] == "/") {
					if (sa[i + 1] == "0") return 4;
					sa[i - 1] = to_string(stof(sa[i - 1]) / stof(sa[i + 1]));
				}
				// Remove operator and right operand
				sa.erase(sa.begin() + i);
				sa.erase(sa.begin() + i);
				i -= 1;
			}
			if (layer == 1 && (sa[i] == "+" || sa[i] == "-")) {
				if (!i) return 5;
				if (sa[i] == "+") {
					sa[i - 1] = to_string(stof(sa[i - 1]) + stof(sa[i + 1]));
				}
				if (sa[i] == "-") {
					sa[i - 1] = to_string(stof(sa[i - 1]) - stof(sa[i + 1]));
				}
				// Remove operator and right operand
				sa.erase(sa.begin() + i);
				sa.erase(sa.begin() + i);
				i -= 1;
			}
			// End of current parentheses
			if (sa[i] == ")") {
				if (layer == 0) break;
				if (i > pos + 1) return 6;
				if (i < 2) return 7;
				sa.erase(sa.begin() + i);
				sa.erase(sa.begin() + i - 2);
				return 0;
			}
			// End of full equation
			if (i == sa.size() - 1) {
				if (layer == 0) break;
				if (i > pos + 1) return 10;
				return 0;
			}
		}
	}
	return 12;
}

int main() {
	// String
	string st;
	//st = "3+2*(8*3/2*4-4*2*(2-1*2/2))";
	//st = "3+2*(-8*3/2*4+4*2*(2-1*2/2))";
	//st = "12+4*(-3/4*2-3+45/2*(1-2*(12-13+(3+4+5*4*2/3)+3)-2)*2)";
	//st = "(12+4*(-3/4*2-3+45/2*(1-2*(12-13+(3+4+5*4*2/3)+3)-2)*2))";
	st = "((12+4*(-3/4*2-3+45/2*(1-2*(12-13+(3+4+5*4*2/3)+3)-2)*2)))";
	vector<string> sa;
	parse_st(st, sa);
	int res = calc(sa, 0);
	if (res) cout << "Error: " << res << "\n";
	cout << "Result: ";
	for (int i = 0; i < sa.size(); ++i) {
		cout << sa[i] << "\n";
	}
	return 0;
}