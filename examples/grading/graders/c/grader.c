#include <stdio.h>
#include <stdatomic.h>

static int answer;
static atomic_int n_guesses;
static int success;

void solve(int n);

int guess(int num) {
	atomic_fetch_add(&n_guesses, 1);
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
