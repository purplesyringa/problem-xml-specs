# 5. Grading

There are two classes of graders.

Firstly, some graders don't store or process any information, but pass it to the checker or the interactor. Such graders are usually introduced purely for convenience. This is how graders are commonly used in IOI-like competitions.

On the other hand, some graders store and process secret information that the user must not be able to access, and provide the user with limited and metered access to it. Such graders are usually used for efficiency, because the overhead from input/output and context switches is too high.

Historically, there has been no separation between the two, but it is obviously unsafe to proceed in this way. While graders of the first class can be handled without any safety measures just like libraries, the second class has to be handled much more carefully.

Let us call the graders of the second class **secure graders**. These graders, unlike graders of the first class, which we shall call **insecure graders**, MUST be listed in the problem specification as described in further sections of this document.

While secure graders MAY be linked with the user's code if the submission is trusted, the judge SHOULD support isolation techniques as described below.


## 5.1. Protocol

As the grader is to be isolated from the user's submission, arbitrary procedure calls between the two cannot be allowed. We therefore formalize the communication protocols to enable secure data transfer between these two contexts.

The grader can **export** functions and types to make them available to the user code, and **import** functions from the user code. How this is configured is language-specific and is subject to further standardization, but we will provide the draft of the standard for some languages below.

One great unexpected side-effect is that standardizing this allows us to enable seamless foreign function interface. For instance, Python code could be allowed to use a C++ grader, provided that imports and exports are configured correctly.


## 5.2. Types

Internally, the following scalar types are recognized:

- `i8`, `i16`, `i32`, and `i64` for 8-, 16-, 32-, and 64-bit signed integers,
- `u8`, `u16`, `u32`, and `u64` for 8-, 16-, 32-, and 64-bit unsigned integers,
- `isize` and `usize` of architecture-specific size for integral indexation types (signed and unsigned, respectively),
- `float` and `double` for 32- and 64-bit IEEE754 floating-point numbers, respectively,
- `bool` for booleans,
- `str` for strings,
- `void` for nothing.

Additionally, higher-order types can be constructed as follows:

- `(T1, T2, ...)` for tuples (type products) containing at least two items,
- `[T]` for homogenous arrays of dynamic size,
- `[N x T]` for homogenous array of fixed size.

Finally, structures and enums can be defined. The former is basically tuples with named fields, and the latter is an `i32` with named options (not a full-blown sum type).

The syntax mostly matches LLVM types when analogues are available, and C or Python types otherwise.

Note that in some languages, mutable and immutable data is stored in different types. For instance, in Rust a mutable string is usually stored in `String`, while an immutable string is usually represented by `&str`.


## 5.3. Type translation

### 5.3.1. C++

The above translates to C++ types as follows:

- `iN` translates to `intN_t` and `uN` translates to `uintN_t`,
- `isize` and `usize` translate to `ssize_t` and `size_t`, respectively,
- `float`, `double`, `bool`, and `void` are translated as-is,
- `str` translates to `std::string`,
- `(T1, T2)` translates to `std::pair<T1, T2>`,
- `(T1, T2, T3, ...)` translates to `std::tuple<T1, T2, T3, ...>`,
- `[T]` translates to `std::vector<T>`,
- `[N x T]` translates to `std::array<T, N>`,
- Structures translate to `struct`,
- Enums translate to `enum class`.


### 5.3.2. Python

The above translates to Python types as follows:

- `iN`, `uN`, `isize`, and `usize` all translate to `int`,
- `float` and `double` both translate to `float`,
- `bool` and `str` translate as-is,
- `void` translates to NoneType,
- `(T1, T2, ...)` translates to `Tuple[T1, T2, ...]`,
- `[T]` and `[N x T]` translate to `List[T]`,
- Structures translate to `dataclasses.dataclass`,
- Enums translate to `enum.IntEnum`.


### 5.3.3. Rust

The above translates to Rust types as follows:

- `iN`, `uN`, `isize`, `usize`, and `bool` are translated as-is,
- `float` and `double` translate to `f32` and `f64`, respectively,
- `str` translates to `&str` in function parameters and `String` otherwise,
- `void` translates to `()`,
- `(T1, T2, ...)` is translated as-is,
- `[T]` translates to `&[T]` in function parameters and `Vec<T>` otherwise,
- `[N x T]` translates to `&[T; N]` in function parameters and `Box<[T; N]>` otherwise,
- Structures translate to `struct`,
- Enums translate to `enum`.


## 5.4. Importing and exporting in C++

The imports are parsed from forward declarations listed in the file, which MUST be called `imports.hpp`. For the number guessing problem, this file might look as follows:

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

Note that such formalized definition enables automatic signature generation for all languages.


## 5.5. Security measures

There are several ways to implement secure graders. This document does not require the judge developers to choose any particular way, leaving it to their own best judgement.

One possibility is to use process isolation: one process would run the user code and the other process would run the grader code. Fake functions would be made available that would send the commands between the two. The grader process might run under a different user or with PR_SET_DUMPABLE reset to stop the user from accessing grader data.

Another possibility is architecture-dependent and involves the user of protecting rings. In x86 and x86-64, the kernel typically runs at ring 0 and userland uses ring 3. If the memory pages used by the grader were ring 2, the user program would not be allowed to access grader data directly, only by explicitly configured call gates.
