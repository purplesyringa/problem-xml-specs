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

The author finds such a complex assumed scheme unsatisfactory and proposes to allow problemsetters to build their own pipelines, which would allow for finer behavior than what the present scheme supports. With this scheme, the pipeline is not inferred by the judge, but loaded from the problem package. Then the actions the judge has to perform are reduced to the following.

When the user sends the submission to the system, it runs the *strategy*. The strategy determines everything that has to be done, including but not limited to compilation, invocation, and evaluation, and does everything necessary to judge the submission. The system then parses the verdicts from the output of the strategy.

That is not to say that all compliant systems are expected to implement support for arbitrary pipelines, or strategies. It also does not mean that problemsetters will have to write these pipelines manually--it is assumed that preparation systems will take care of that in 99% of the cases. Compatibility with existing problems is completely retained too. In fact, the main goal of this section is to formalize the judging process so that how the various configuration options of existing formats affect testing is more obvious.


## 2.2. Scope

The proposed scheme unites all common problem types, hopefully reducing learning complexity and leading to interesting new kinds of competitions.

The list of supported types includes:

- Simple input/output problems,
- Interactive problems,
- Run-twice problems (of various kinds),
- Output-only problems,
- Problems with graders,
- Problems with valuers,
- (For lack of a better word) formulaic problems,
- Compile-on-each-test problems,
- Efficiency-rated problems.

Moreover, the same scheme allows for problems usually considered out of scope of competitive programming, e.g. problems with custom languages or machine learning model grading.


## 2.3. Strategy

A **strategy** is what controls how the judge evaluates the submission. In this document, we will use a domain-specific language for strategies, effectively interpreting them as scripts.

Presently, compliant judges are not required to support arbitrary scripts in this DSL. A complete description of the DSL is, however, present in this document for future reference: new systems SHOULD be designed as to allow this possiblity with minor changes.

Firstly, we show how the strategy for a simple input/output problem looks like:

```python
user = await compile(submission)
for test in tests:
    with test:
        output = File()
        user(stdin=test.input, stdout=output)
        checker(test.input, output, test.answer)
```

Most of this should be self-explanatory. Per line:

1. `submission` is the original file submitted by the user. As this file is passed to `compile`, the judge determines that `submission` must be a script written in a programming language. When the program is compiled, the reference to the executable file is put to the `user` variable.

2. The behavior for each test is defined.

3. Test context is entered, so that all verdicts are applied to this test.

4. An empty file is created (one for each test).

5. The `user` program (that was just compiled) is invoked with standard output redirected into this new file, and standard input redirected from the input file associated with the test. The program is subject to default limits.

6. The `checker` program, which is provided by the problem, is invoked with three arguments: the path to input file, the path to the submission output, and the path to the model answer.

Another example is due, hopefully with no need of explanation. A run-twice problem may have the following strategy:

```python
user = await compile(submission)
for test in tests:
    with test:
        run1_output, run2_input, run2_output = File(), File(), File()
        user(stdin=test.input, stdout=run1_output)
        checker(test.input, run1_output, test.answer, stdout=run2_input)
        user(stdin=run2_input, stdout=run2_output)
        checker(run2_input, run2_output, test.answer)
```


## 2.4. Interpretation

As can perhaps be seen already, the strategy script is written in Python, with some global variables provided in addition to the default ones. We will now dive into formal exporation of these provided objects. You should perhaps skip this section on the first read and return to it after reading the next few sections.

Firstly, `submission` is the file sent to the judge by the user.

Its Python type is `Submission`, which is in turn a subclass of `File`. `File` is a container for any sort of raw data. `File` can store the type of the contained file as specified in [3. Types](03-types.md). This type can be accessed and modified via the `type` property, which stores an instance of `Type`.

In addition to all operations supported by files, `submission` has the following properties:

- `timestamp`: the UNIX timestamp of the time the solution was submitted to the judge,
- `id`: an implementation-defined unique string identifier of the submission,
- `author`: an implementation-defined unique string identifier of the solution author,
- `contest_id`: an implementation-defined unique string identifier of the contest,
- `metadata`: a dictionary of implementation-defined data.

It also has the `submission.rate` and `submission.disable_autorate` methods, which are described below.

`Type` can be constructed using `Type("...")` and converted to a string using `str(...)`. `Type` represents a type mask. `Type.intersection(type1, type2, ...)` can be used to find the "best" concrete type that satisfies all of the given masks, or `None` if none exists. `type1.matches(type2)` can be used as a shortcut for `Type.intersection(type1, type2) is not None`. `type.matches("...")` is, again, a shortcut for `type.matches(Type("..."))`.

`compile` is an asynchronous function that takes a source file with a known type as a `File` object (or several source files) and emits an executable file. The type of the result is `Executable`.

`Executable` usually represents a single file that is stored as a `File` object in the `main_file` attribute. However, in some cases an executable object may consist of several files. In this case `main_file` will only store one file, and the list of files along with their names is stored in the `files` attribute of type `dict[str, File]` (including `main_file` itself).

Executables that are attached to the problem, such as checkers and validators, are stored as `Executable` objects named `checker`, `validator`, etc. This can be customized.

A `Limits` object stores the various limitations the program is subject to. The mandatory limits are `time` (in seconds), `memory` (in bytes), and `real_time` (in seconds), e.g. `Limits(time=1)`. Systems MAY implement their own custom limitations.

`tests` is a global iterable of all tests of a program. It will not necessarily be a `list`, but it can be iterated by, yielding instances of `Test`.

A `Test` object has a property called `no`, which stores the ID of test, usually in 1-indexation. It also *usually* has properties `input` and `answer`, each of type `File`, also `points` of type `float` in IOI-style problems and `group` of type `Group` if groups are enabled, but these properties may not be present, or other properties may be, if the problem is of a non-standard type--this is configurable. `test.verdict` contains the current test verdict. `test.metrics` contains the list of metrics, one per each run of the user program on this test.

A `Test` is a Python context manager, which means it can be passed as an argument to the `with` statement. Per-test operations MUST be wrapped in `with test`.

A `File` object can be created using the `File()` constructor, which creates a file of unknown type. The type can be passed at construction time via `File(type="...")`.

An `Executable` object can be invoked by calling the object directly: `executable(*argv, stdin=..., stdout=..., stderr=..., files=..., limits=...)`:

- An item of `argv` can be a string, a `bytes` object, a `File`, and also a `Pipe`, which is described in the next section. If an item is a `File` or a `Pipe`, the file is mounted read-only and the path to the object on the filesystem is passed as an argument. `file.rw()` can be passed to make the mount read-write.
- `File` and `Pipe` objects can be passed as `stdin`, `stdout`, and `stderr`. The default values are `None`, which is equivalent to writing to or reading from `/dev/null`, `NUL`, or equivalent.
- `files` is an optional dictionary of relative path to `File` mapping. If it is specified, the user program will be able to read the provided files at the given paths. `file.rw()` can be passed to make the mount read-write.
- `limits` sets the limits; if it is missing, or if it's present but a property is missing, default values apply. These defaults SHOULD be problem-configurable.

The function returns an asynchronous future. Invocation is performed even if the future is not awaited.

If a `File` passed within `argv` or via `stdin` to an executable is being written to at the moment (by another executable), invocation is implicitly delayed until the latter executable terminates.

Executables have a kind stored in the `kind` property. The property is modifiable. This kind can be either `"user"`, `"system"`, or `"testlib"`. The first option indicates that the program is provided by the user, and if it errors, it is the user's fault. The latter two options indicate that all failures are problemsetter's fault. In the third case, the exit code and the stderr of the program are additionally parsed as if generated by `testlib` and affect the verdict. The result of what test is affected is determined by the covering `with test` statement. If it is missing, the modification is applied problem-wide.

Different default limits apply to executables of different kinds. Limits for `user` kind MUST be configurable; limits for `system` and `testlib` SHOULD be configurable.

If any invocation or compilation fails (which includes exceeding limits, non-zero exit code for `kind == "user"`, and a non-AC verdict for `kind == "system" || kind == "testlib"`), all operations related to the test are terminated immediately, and `AbortedException` is scheduled to be thrown when any of those operations are awaited. This exception is swallowed by `with test` automatically. This behavior was not explicitly visible in the two examples above, because nothing was awaited, but this information will come in handy later.

This should be enough to explain how the examples above work.


## 2.5. Pipes

The strategy for a typical interactive problem looks as follows:

```python
user = await compile(submission)
for test in tests:
    with test:
        interactor_output = File()
        i2u, u2i = Pipe(), Pipe()
        user(stdin=i2u, stdout=u2i)
        interactor(test.input, interactor_output.rw(), test.answer, stdin=u2i, stdout=i2u)
        checker(test.input, interactor_output, test.answer)
```

In this example, there is a new kind of object: `Pipe`. A `Pipe` refers to a named PIPE or an equivalent object, depending on the platform. A `Pipe` is not subject to the reader-after-writer feature of `File`. `Pipe`s are unidirectional: if a pipe is passed to an executable via `stdout` or `stderr`, only the write end is passed, and if it is passed via `argv` and `stdin`, only the read end is passed. If the write end of `pipe` is to be passed via `argv`, `pipe.as_writable()` should be passed instead.


## 2.6. Custom file names

In some old programs, the user is expected to read from `input.txt` and output to `output.txt` instead of using standard streams. This can be implemented as follows:

```python
user = await compile(submission)

for test in tests:
    with test:
        output = File()
        user(stdin=test.input, stdout=output, files={"input.txt": test.input, "output.txt": output.rw()})
        checker(test.input, output, test.answer)
```

Note that in this example, we allow the user to read from both stdin and `input.txt`, and write to boht stdout and `output.txt` (though not at once unless buffering is disabled). The author believes this is more correct than only supporting file I/O.


## 2.7. Archives

The strategy for a typical output-only problem looks as follows:

```python
user = submission.into_archive()
for test in tests:
    with test:
        try:
            checker(test.input, user[f"{test.no:02}.out"], answer)
        except KeyError:
            test.rate(PE, "File is missing")
```

The `into_archive()` method of a `File` interprets it as a ZIP archive, or another archive type if supported by the judge. The contents of an `Archive` object can be accessed using the indexing operator, which takes the name of the file as the index and returns a `File` object. If there is no such file, `KeyError` is thrown, just like with a normal dictionary.

The verdicts on the tests are usually detected automatically according to the following rules. The list of verdicts is listed in [6. Valuation](06-valuation.md). The first executable to fail, if any, determines the verdict:

- If the failed program has `user` kind, then TLE, MLE, RE, and other errors are directly translated into the verdict.

- If the failed program has `system` kind, then TLE, MLE, and other errors are translated to CF,

- If the failed program has `testlib` kind, then TLE, MLE, and other errors are translated to CF, and otherwise the exit code and stderr are used to determine the appropriate verdict.

If no program fails, the verdict is considered OK, unless any program of kind `testlib` sets the verdict to PT, in which case that verdict is assumed. If several programs of kind `testlib` are called and none fails, either all of them MUST return OK, or one of them MUST return PE and all the others OK.

The strategy can overwrite the verdict via a call to `test.rate`. This function takes a verdict, and then a comment as an optional argument. The verdicts are global variables named `OK`, `RE`, etc., as described in [6. Valuation](06-valuation.md), with the exception of `PT`, which is to be initialized as `PT(points)`. The verdict can later be accessed via the `test.verdict`, and the comment via `test.comment`.


## 2.8. Arbitrary compilation

If different code has to be linked into the program supplied by the user, the strategy may look as follows:

```python
async def run_test(test):
    with test:
        output = File()
        user = await compile(submission, test.code, kind="testlib")
        user(stdin=test.input, stdout=output)
        checker(test.input, output, test.answer)

for test in tests:
    asyncio.create_task(run_test(test))
```

The coroutine `run_test` is introduced and is started as an asyncio task. This is to ensure that several tests can be judged in parallel, if the judge desires so. This was not necessary before because `await` was not used inside the loop.

Several source code files are passed to `compile`. As they are of different kinds, the kind of the resulting executable has to be supplied manually. In this example, it is assumed that `test.code` contains the entrypoint of the program and returns the appropriate verdict in testlib style. **Note that this is insecure and only used here for demonstration of capabilities of strategies.**

If compilation fails during per-test judgement, `compile` raises `AbortedException`, which is swallowed by `with test`, and this failure is translated to CE verdict on the test.

Note that this assumes that the programs at `submission` and `test.code` are written in the same language. If that isn't so, CE verdict MAY be raised, unless the judge supports cross-language compilation, which is described in [4. Compilation](04-compilation.md).


## 2.9. Efficiency-rated problems

If problemsetters wanted to rate solutions based on the time it takes for them to execute, their strategies could look like this:

```python
user = await compile(submission)

async def run_test(test):
    with test:
        result = await user(stdin=test.input, stdout=output)
        await checker(test.input, output, test.answer)
        test.rate(PT(test.points * min(1, 0.1 / result.metrics.time)))

for test in tests:
    asyncio.create_task(run_test(test))
```

This demonstrates that the return value of an invocation is an `InvocationResult` object, which stores the following information:

- a platform-dependent integral exit status at `exit_code`,
- metrics, accessible via `metrics.time`, `metrics.memory`, `metrics.real_time`, etc.,
- the verdict at `verdict` and the comment at `comment` for `testlib` kind.

In this example, the points of a submission on a test are equial to the maximum points of test times `min(1, 0.1 / CPU time used by the submission)`.


## 2.10. Raw submissions

Sometimes the user is asked to submit a raw data file for evaluation, not a whole program, but the algorithm that determines the verdict of the submission is so complicated that it is meaningful to use a competitive programming judge rather than your typical quiz service.

As should be obvious already, this is not a tiniest problem for strategies. In fact, all the necessary features have already been described above, and the code is as simple as:

```python
for test in tests:
    with test:
        checker(test.input, submission, test.answer)
```


## 2.11. Valuation

It is often reasonable to let the judge convert the list of verdicts on tests into a single verdict of the submission: there are test groups and dependencies, which the judge takes care about as described in [6. Valuation](06-valuation.md). However, sometimes the relations and formulae are so bizarre that this must be implemented manually.

Here's how one might do that:

```python
submission.disable_autorate()

user = await compile(submission)

async def run_test(test):
    with test:
        output = File()
        await user(stdin=test.input, stdout=output)
        await checker(test.input, output, test.answer)

    if test.verdict is OK:
        return test.points
    elif isinstance(test.verdict, PT):
        return test.verdict.points
    else:
        return 0

test_points = await asyncio.gather(*map(run_test, tests))

#
# Manually:
#
if min(test_points) == 0:
    points = 0
else:
    # Harmonic mean
    points = len(test_points) / (sum(1 / x for x in test_points))

submission.rate(PT(points))

#
# ...or with a call to an external program:
#
f = File()
f.write(f"{len(test_points)}\n".encode())
for x in test_points:
    f.write(f"{x}\n".encode())

await valuer(f)
```

There is only one new detail here: the `submission.disable_autorate` and `submission.rate` methods. `submission.rate` takes the same arguments as `test.rate`, including an optional comment, and set the main submission verdict. For it to work, `submission.disable_autorate()` MUST be called at the very beginning of the script; otherwise, the submission is rated automatically based on test verdicts. In this example, `submission.rate` is called only once, but it can be called several times, say, after each test is judged, to provide realtime verdict updates, which are described in [6. Valuation](06-valuation.md).


## 2.12. Grading

This is similar to the example provided for arbitrary compilation:

```python
if submission.type.matches("cpp"):
    files = [submission, grader_cpp, grader_h]
elif submission.type.matches("python^3"):
    files = [submission, grader_python]
else:
    submission.rate(CE, comment="Only C++ and Python programs are supported")
    return

user = await compile(*files, kind="testlib")

for test in tests:
    with test:
        output = File()
        user(stdin=test.input, stdout=output)
        checker(test.input, output, test.answer)
```

In this example, we support graders for multiple languages. This is the first time we used explicit type identifiers. Although the behavior of this particular strategy is obvious, you should probably take a look at [3. Types](03-types.md).

`compile` is called with several operands. Exactly how multi-file compilation works depends on the language and is described thoroughly in [4. Compilation](04-compilation.md).


## 2.13. Groups

As described in [6. Valuation](06-valuation.md), tests can be divided into groups.

The group name of a test is accessible via `test.group`. The global object `groups` is a dictionary with string keys and `Group` values. A `Group` object contains the `tests` property, which is a subset of `tests` contained in the group, and the `name` property, as well as string `points_policy` and `feedback_policy` attributes and the `dependencies` attribute of type `List[Test]`.


---

The author believes that all the illustrations were more than enough to help problemsetters write strategies and help judge developers comprehend them.
