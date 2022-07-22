public class Grader {
	private static int answer;
	private static int n_guesses;
	private static int success;

	public static int Guess(int num) {
		System.Threading.Interlocked.Increment(ref n_guesses);
		if(answer < num) {
			return -1;
		} else if(answer > num) {
			return 1;
		} else {
			System.Threading.Interlocked.Exchange(ref success, 1);
			return 0;
		}
	}

	static void Main(string[] args) {
		var items = System.Console.ReadLine().Split(null);
		int n = int.Parse(items[0]);
		answer = int.Parse(items[1]);
		Solution.Solve(n);
		System.Console.WriteLine(
			System.Threading.Interlocked.CompareExchange(ref n_guesses, 0, 0)
			+ " "
			+ System.Threading.Interlocked.CompareExchange(ref success, 0, 0)
		);
	}
}
