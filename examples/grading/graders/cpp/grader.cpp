#include <iostream>
#include <atomic>

static int answer;
static std::atomic_int n_guesses;
static std::atomic_int success;

void solve(int n);

int guess(int num) {
	n_guesses++;
	if(answer < num) {
		return -1;
	} else if(answer > num) {
		return 1;
	} else {
		success = 1;
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
