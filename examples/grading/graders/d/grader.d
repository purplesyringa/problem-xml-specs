import core.atomic;
import std.array;
import std.conv;
import std.stdio;

import solution : solve;

private int answer;
private int n_guesses;
private int success;

int guess(int num) {
	atomicFetchAdd(n_guesses, 1);
	if(answer < num) {
		return -1;
	} else if(answer > num) {
		return 1;
	} else {
		success = 1;
		return 0;
	}
}

void main() {
	string input = readln();
	int n = to!int(input.split[0]);
	answer = to!int(input.split[1]);
	solve(n);
	writefln("%d %d", n_guesses, success);
}
