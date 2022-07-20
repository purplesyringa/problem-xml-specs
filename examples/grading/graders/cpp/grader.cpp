#include <iostream>

static int answer;
static int n_guesses;
static bool success;

void solve(int n);

int guess(int num) {
	n_guesses++;
	if(answer < num) {
		return -1;
	} else if(answer > num) {
		return 1;
	} else {
		success = true;
		return 0;
	}
}

int main() {
	int n;
	std::cin >> n >> answer;

	solve(n);

	std::cout << n_guesses << " " << success << std::endl;
	return 0;
}
