import "dart:io";
import "solution.dart";

var answer = 0;
var n_guesses = 0;
var success = 0;

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

void main() {
	List<String> input = stdin.readLineSync()!.split(" ");
	int n = int.parse(input[0]);
	answer = int.parse(input[1]);

	solve(n);

	print("${n_guesses} ${success}");
}
