# 5. Grading

There are two classes of graders.

Firstly, some graders don't store or process any information, but pass it to the checker or the interactor. Such graders are usually introduced purely for convenience. This is how graders are commonly used in IOI-like competitions.

On the other hand, some graders store and process secret information that the user must not be able to access, and provide the user with limited and metered access to it. Such graders are usually used for efficiency, because the overhead from input/output and context switches is too high.

Historically, there has been no separation between the two, but it is obviously unsafe to proceed in this way. While graders of the first class can be handled without any safety measures just like libraries, the second class has to be handled much more carefully.

Let us call the graders of the second class **secure graders**, and the graders of the first class **insecure graders**. Graders of both kinds are listed in the problem specification as described in [9. Files](09-files.md). The `compile` strategy function determines the kind of grader from that specification automatically.

While secure graders MAY be linked with the user's code directly if the submission is trusted, the judge SHOULD support isolation techniques as described below.


## 5.1. Protocol

As the grader is to be isolated from the user's submission, arbitrary procedure calls between the two cannot be allowed. We therefore formalize the communication protocols to enable secure data transfer between these two contexts.

The grader can **export** functions and types to make them available to the user code, and **import** functions from the user code. How this is configured is language-specific and is subject to further standardization, but we will provide the draft of the standard for some languages below.


## 5.2. Importing and exporting in C++

The imports are parsed from forward declarations listed in a file called `imports.hpp`. For the number guessing problem, this file might look as follows:

```cpp
void solve(int n);
```

Similarly, exports are provided in the file `exports.hpp`, an example of which is listed below:

```cpp
int guess(int num);
```

As it is common to allow the user code to include the grader's header file like `#include "grader.h"`, the `#include` directive is allowed inside the `exports.hpp` file. For instance, the above is equivalent to the following two files:

`grader.h`:

```cpp
int guess(int num);
```

`exports.hpp`:

```cpp
#include "grader.h"
```

The forward declarations MUST be of the following syntax:

```cpp
RETURN_TYPE FUNCTION_NAME(ARG1_TYPE ARG1_NAME, ARG2_TYPE ARG2_NAME, ...);
```

Variadic and default parameters are not allowed. Overloading isn't either.

In addition to forward declarations, `exports.hpp` may contain structure definitions:

```cpp
struct STRUCTURE_NAME {
	FIELD1_TYPE FIELD1_NAME;
	FIELD2_TYPE FIELD2_NAME;
	...
};
```


## 5.3. Security measures

There are several ways to implement secure graders. This document does not require the judge developers to choose any particular way, leaving it to their own best judgement.

One possibility is to use process isolation: one process would run the user code and the other process would run the grader code. Fake functions would be made available that would send the commands between the two. The grader process might run under a different user or with PR_SET_DUMPABLE reset to stop the user from accessing grader data.

Another possibility is architecture-dependent and involves the user of protecting rings. In x86 and x86-64, the kernel typically runs at ring 0 and userland uses ring 3. If the memory pages used by the grader were ring 2, the user program would not be allowed to access grader data directly, only by explicitly configured call gates.
