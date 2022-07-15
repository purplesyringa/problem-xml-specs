# 2. Pipelines

## 2.1. Rationale

Consider the simplest competitive programming problem: A+B. In this problem, you are given two numbers A and B, and you should output the sum of the two numbers. You are to write a program that solves this problem by scanning the two numbers from the standard input and printing the result to the standard output, and submit it to the judge.

In C++, this problem may be solved with the following code:

```cpp
#include <iostream>

int main() {
    int a, b;
    std::cin >> a >> b;
    std::cout << a + b << std::endl;
    return 0;
}
```

Or in Python:

```python
a, b = map(int, input().split())
print(a + b)
```

The goal of this small digression is not to waste the problemsetters' valuable time, but to demonstrate the huge number of assumptions and the contrast between the simplicity of the submission and the background work to judge it.

When the user sends the submission to the system, it *compiles* the submission unless it's written in an interpretable language. It then *runs* the submission on each of the tests, ensuring that it does not use more time, memory, or other resources than allowed, that it does not crash or attempt anything malicious. The output on each of the tests is recorded, and the *checker* is invoked to compare the output of the submission with the output of the *model solution*, which is the author's supposedly correct solution to the problem. The checker responds with a *verdict*. The verdicts of the submission on each test are then combined to rate the submission using a *valuer*.

The author finds such a complex assumed scheme unsatisfactory and proposes to allow problemsetters to build their own pipelines, which would allow for finer behavior than what the present scheme supports.

That is not to say that all compliant systems are expected to implement the support for such arbitrary pipelines. The main goal of this section is to formalize the judging process so that how the various configuration options affect testing is more obvious.


## 2.2. Scope

The proposed scheme unites all common problem types, hopefully reducing learning complexity and leading to interesting new kinds of competitions.

The list of supported types includes:

- Simple input/output problems,
- Interactive problems,
- Run-twice problems (of various kinds),
- Output-only problems,
- Dynamic scoring problems,
- Problems with graders,
- (For lack of a better word) formulaic problems.

Moreover, the same scheme allows for problems usually considered out of scope of competitive programming, e.g. problems with custom languages or machine learning model grading.


## 2.3. Strategy

A **strategy** is what controls how the judge evaluates the submission. In this document, we will use a domain-specific language for strategies, effectively interpreting them as scripts.

Presently, compliant judges are not required to support arbitrary scripts in this DSL. A complete description of the DSL is, however, present in this document for future reference: new systems SHOULD be designed as to allow this possiblity with minor changes.

Firstly, we show how the strategy for a simple input/output problem looks like:

```python
user = await compile(submission)
for test in tests:
    output = File()
    user(stdin=test.input, stdout=output, limits=Limits(time=1, memory=256 * 1024 * 1024))
    checker(test.input, output, test.answer)
```

Most of this should be self-explanatory. Per line:

1. `submission` is the original file submitted by the user. As this file is passed to `compile`, the judge determines that `submission` must be a script written in a programming language. When the program is compiled, the reference to the executable file is put to the `user` variable.

2. The behavior for each test is defined.

3. An empty file is created (one for each test).

4. The `user` program (that was just compiled) is invoked with standard output redirected into this new file, and standard input redirected from the input file associated with the test. The program is subject to the provided CPU time limit (in seconds) and memory limit (in bytes).

5. The `checker` program, which is provided by the problem, is invoked with three arguments: the path to input file, the path to the submission output, and the path to the model answer.


### 2.4. More examples

The strategy for a typical interactive problem looks as follows:

```python
limits = Limits(time=1, memory=256 * 1024 * 1024)
user = await compile(submission)
for test in tests:
    interactor_output = File()
    interactor_to_user, user_to_interactor = Pipe(), Pipe()
    user(stdin=interactor_to_user, stdout=user_to_interactor, limits=limits)
    interactor(test.input, interactor_output, test.answer, stdin=user_to_interactor, stdout=interactor_to_user)
    checker(test.input, interactor_output, answer)
```

A run-twice problem can have the following strategy:

```python
limits = Limits(time=1, memory=256 * 1024 * 1024)
user = await compile(submission)
for test in tests:
    run1_output, run2_input, run2_output = File(), File()
    user(stdin=test.input, stdout=run1_output, limits=limits)
    checker(test.input, run1_output, test.answer, stdout=run2_input)
    user(stdin=run2_input, stdout=run2_output, limits=limits)
    checker(run2_input, run2_output, test.answer)
```


## 2.5. Interpretation

As can perhaps be seen already, the strategy script is written in Python, with the following global variables provided in addition to the default ones:

- `File`, a class designating a file object and an empty file constructor,
- `Pipe`, which is basically `File` but for interactive problems,
- `source`, which is the file submitted by the user as a `File` object,
- `compile`, an asynchronous function that takes a source file with a known type and emits a binary file,
- `tests`, which is an iterable designating the multiple test cases,
- `Limits`, which is a constructor for invocation limits,
- `checker` and other programs attached to the problem.

The syntax for program invocation is `file(*argv, stdin=..., stdout=..., stderr=..., limits=...)`, which returns a future. The default values of `stdin`, `stdout`, and `stderr` are `None`, which is equivalent to `/dev/null` or `NUL`. The default value of `limits` is `Limits()`. The return value (if awaited) is an `InvocationResult` object, which stores the platform-dependent exit code in the `exit_code` attribute, and various metrics, accessible via `metrics.time`, `metrics.time` and other attributes.

The class `Limits` stores the various limitations the program is subject to. The limits are `time` (in seconds) and `memory` (in bytes). Systems MAY implement their own custom limitations. If a limit is not set, its value is implementation-defined.

The class `File` may have a type, whether it is plain data, source code, or an executable file. The various types are described in [4. Types](04-types.md). It tracks role, so to speak, that is, whether it originated from the user input, perhaps via a call to `compile`, or from the problem package. The file may be read from and written to as a normal Python file instance.

By default, tests are rated in the following way. The programs are executed in the order in which they are specified, and the first failure determines the verdict of the submission on the current test. You may want to read [5. Verdicts](05-verdicts.md).

- If the program originated from user input, then TLE, MLE, RE, and other errors are directly translated into the verdict.
- If the program originated from the problem package, then TLE, MLE, and other errors are translated to CF, and otherwise the exit code and stderr are used to determine the appropriate verdict. Exactly how this is done depends on the type of the program.

The script can rate the tests manually by calling `test.rate(verdict, comment=...)`, with the `comment` field optional. The verdicts are global variables named `OK`, `RE`, etc., with the exception of `PT`, which should be used as `PT(points)` instead of plain `PT`.

The invoked programs do not have to be executed sequentially. In fact, judges MUST support program-level parallelism (for interactive problems), and they MAY support test-level parallelism. If a program is invoked while any a program's arguments or its `stdin` are still being written to, the invocation is delayed until the other program that writes to that file terminates.
