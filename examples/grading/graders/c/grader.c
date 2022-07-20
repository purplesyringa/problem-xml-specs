#include <stdio.h>

static int answer;
static int n_guesses;
static int success;

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
	scanf("%d%d", &n, &answer);

	solve(n);

	printf("%d %d\n", n_guesses, success);
	return 0;
}
