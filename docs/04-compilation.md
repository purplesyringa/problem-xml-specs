# 4. Compilation

Classically, before a user submission is judged, it has to be compiled. This standard stretches the definition somewhat by allowing a strategy to decide what to compile and when. However, the steps of transforming source code to a runnable program remain, even for some interpreted languages, in which case it might consist only of sandbox creation.

How programs are compiled and executed has historically been black-boxed, and it mostly remains so in the proposed format. However, we have to shed a little light on the internals when graders and libraries are involved.


## 4.1. Case studies

### 4.1.1. C++ graders

In an IOI-style A+B problem, the user is asked to write a function satisfying the following prototype:

```cpp
int sum_ab(int a, int b);
```

...and one of the accepted solutions is:

```cpp
int sum_ab(int a, int b) {
    return a + b;
}
```

This submission is linked with managing code, called *grader*, which might look like this:

```cpp
#include <iostream>

int sum_ab(int a, int b);

int main() {
    int a, b;
    std::cin >> a >> b;
    std::cout << sum_ab(a, b) << std::endl;
}
```

Both files are provided to the compiler in the compilation string.

However, more often graders are bidirectional, which means that the grader exports functions that can be called by the user code. For C++ submissions, these functions are made visible to the submission using a header file. For instance, for the "guess the number" problem:

User submission:

```cpp
#include "grader.h"

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
```

`grader.h`:

```cpp
int guess(int num);
```

`grader.cpp`:

```cpp
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
```

However, extra care has to be taken when compiling this code. The header file shouldn't be passed to the compiler directly, but it has to be made available at a particular path.


### 4.1.2. Python graders

In Python, the expected user's solution to a typical A+B problem would look as follows:

```python
def sum_ab(a, b):
    return a + b
```

The grader might look like this:

```python
import solution
a, b = map(int, input().split())
print(solution.sum_ab(a, b))
```

The grader is invoked by the interpreter, and the user's solution can be accessed using the `solution` module.

For bidirectional graders, the user solution is allowed to use the `import {MODULE}` directive, where `{MODULE}` is the name of the grader file (except the extension).


### 4.1.3. Libraries

Occasionally problemsetters want to permit the user to access library code. This is often used if the ad-hoc idea of the problem is orthogonal to the data structures required to implement it, and the problemsetters think the former is more important than the latter. In other cases, the library code may be necessary for submissions to be tested correctly, as with graders.

Internally, libraries are not any different from graders, except that the user code is considered the "main" part of the executable, not the grader.


## 4.2. Compilation progress

It would be reasonable for the compilation step to consist of compiling all the given source code files into a single executable, and that's it. However, it is seldom this trivial.

Firstly, if the problem reveals a grader or a library, the user and problem code have to somehow interact with each other.

Some programming languages (e.g. Java) use module or class names to access other source files, and this means that problem-provided objects have to have fixed names and be able to learn how to access the submission code.

Other languages use paths, and this means that the problem-provided files have to be stored in a fixed directory hierarchy relative to the submission code, and the submission has to use a fixed file name. Therefore, each problem file that is accessible to the user code has a particular relative path attached to it. By default, this path equals the name of the file. For instance, a classical `grader.h` file will lie next to the user's submission.

Except that some languages require the file name to match the module name, which complicates matters even more.

Besides, many languages have a notion of a "main" file, e.g. Python and Rust. In simple problems that don't provide graders or libraries, there is only one module, that is, the user's submission, but when the problem provides source files the choice is less clear. In this case, it is the responsibility of the judge to either detect the main file automatically or generate a virtual entrypoint, as described below.

This section explains how officially supported languages are to be handled, and how cross-file communication is performed.


### 4.2.1. C

The C programming language supports passing multiple source files to the compiler. Files of two types are allowed: `c` and `c-h`. Only the former should be included in the compilation line. Both types of files MUST be accessible to the compiler at the appropriate locations. The user submission may have an arbitrary file name, except the extension, which MUST be `c`.


### 4.2.2. C++

The C++ programming language supports passing multiple source files to the compiler. Files of two types are allowed: `cpp` and `cpp-h`. Only the former should be included in the compilation line. Both types of files MUST be accessible to the compiler at the appropriate locations. The user submission may have an arbitrary file name, except the extension, which MUST be `cpp`.


### 4.2.3. C#

The C# programming language supports passing multiple source files to the compiler. The user submission may have an arbitrary file name, except the extension, which MUST be `cs`. Among all source files, exactly one file MUST contain exactly one class with the `Main` method (other classes may be defined too). The main class MUST be public. This class is considered the entrypoint.

The paragraph below applies in case the entrypoint is not in the user's submission.

The user code MUST contain at least one public class. If there are several public classes, one of them MUST be named `Solution`, or the submission is considered ill-formed. If there is one exactly public class, and it's not named `Solution`, an implicit using directive MUST be added to make `Solution` an alias of this class.


### 4.2.4. Common Lisp

The Common Lisp programming language does not support passing multiple source files to the compiler. Problem-provided files MUST be accessible using `(load "{PATH}")`; `{PATH}` MUST NOT include the `cl` or `lisp` extension to enable pre-compilation. Similarly, the user file MUST be accessible to the problem-wide programs using `(load *solution*)`. If there is more than one user-kind file, a "virtual" solution is to be provided, containing `(load ...)` directives that load all the user files in some undefined order.

As `clisp` does not load rc files in batch mode, a separate virtual entrypoint that imports all source files in arbitrary order and then exits SHOULD be passed to the interpreter.

CL supports per-file compilation with `(compile-file)` or an equivalent compiler option. For efficiency, files SHOULD be pre-compiled.


### 4.2.5. D

The D programming language supports passing multiple source files to the compiler. The user submission MUST be accessible to the problem-wide files using `import solution`; if the name of the user file is not `solution.d`, a virtual file that re-exports the user code is to be provided.


### 4.2.6. Dart

The Common Lisp programming language does not support passing multiple source files to the compiler. The user submission MUST be accessible to the problem-wide files using `import "solution.dart"`; if the name of the user file is not `solution.dart`, a virtual file that re-exports the user code is to be provided. Problem-provided files MUST be accessible using `import`.
