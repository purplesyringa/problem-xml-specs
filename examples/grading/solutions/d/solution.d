import grader;

void solve(int n) {
	int l = 1;
	int r = n;
	while(true) {
		int m = (l + r) / 2;
		int result = guess(m);
		if(result == -1) {
			r = m - 1;
		} else if(result == 1) {
			l = m + 1;
		} else {
			break;
		}
	}
}
