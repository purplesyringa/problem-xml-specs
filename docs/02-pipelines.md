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

A **strategy** is what controls how the judge evaluates the submission. In this document, we will use a domain-specific language for strategies, effectively interpreting them as scripts. Presently, compliant judges are not required to support arbitrary scripts in this DSL, but the systems SHOULD be designed as to allow this possiblity with minor changes.

First, we show how the strategy for a simple input/output problem looks like:

```python
user = compile(source)
for test in tests:
    output = file()
    user(stdin=test.input, stdout=output, time=1, memory=256 * 1024 * 1024)
    checker(test.input, output, test.answer)
```

Most of this should be self-explanatory. Per line:

1. `source` is the original file submitted by the user. As this file is passed to `compile`, the judge determines that `source` must be a script written in a programming language. When the program is compiled, the reference to the executable file is put to the `user` variable.

2. The behavior for each test is determined.

3. An empty file is created (for each test).

4. The `user` program (that was just compiled) is invoked with standard output redirected into this new file, and standard input redirected from the input file associated with the test. The program is subject to the provided time limit (in seconds) and memory limit (in bytes).

5. The `checker` program, which is provided by the problem, is invoked with three arguments: the path to input file, the path to the submission output, and the path to the model answer.


## 2.4. Syntax

As can perhaps be seen already, the strategy script is a subset of Python.
