# 6. Valuation

The judge consumes user submissions, which are just files, and emits some result. This section describes what this result consists of: the metrics, the logs, and the verdicts.


## 6.1. Metrics

When the user submission is invoked and judged, the judge MUST track all information from the following list:

- **CPU time**, also **time**--how much time the processor spent handling the user processes, summed up across CPU cores,
- **memory**--the maximum amount of physical memory (not virtual memory!) allocated to the user processes at once,
- **real time**--how much wall-clock time passed between the moment the user submission was started and finished,
- **exit status**--the exit code the program returned upon termination or the signal it was killed with.

Problemsetters MUST be able to set limits on CPU time and memory and SHOULD be able to set limits on real time.

Typically, not all of this information is public: usually, only CPU time and memory usage are shown to the user.


## 6.2. Logs

The steram stdout and stderr of all invoked programs SHOULD be saved, unless they are redirected to pipes. Whether interaction protocols are recorded is implementation-defined.


## 6.3. Test verdicts

The output, statistics, and exit status of the user program and the problem scripts are used to determine the verdict of the submission on a test. A **verdict** is a kind of status of how the submission handled the test: it may be all right, it may print a wrong answer, it may time out, and so on. Verdicts are mostly for those kinds of errors. Verdicts are often shortened to two or three capital latin letters.

Some verdicts are set by the judge, while others may be set by the checker and other problem scripts too. A judge in itself probably cannot determine that the answer is wrong, but it can stop the program if it uses too much memory.

After a submission is graded, a verdict is attached to each test first, rather than the submission as a whole. All of the following verdicts MUST supported:

- **OK**: the submission printed the correct answer on the test and stayed within all limits,
- **Wrong answer (WA)**: the submission stayed within the limits, but printed a wrong answer,
- **Runtime error (RE)**: the submission returned a non-zero exit code or was killed by a signal; the exit code (or signal number) SHOULD be recorded,
- **Time limit exceeded (TL or TLE)**: the submission used more CPU time than allowed,
- **Memory limit exceeded (ML or MLE)**: the submission used more RAM than allowed,
- **Presentation error (PE)**: the submission stayed within the limits and printed something, but the output could not be parsed due to invalid format,
- **Idleness limit exceeded (IL or ILE)**: the submission used more real (wall-clock) time than allowed,
- **Check failed (CF)**: the checker, interactor, or other problem-wide program returned an invalid exit code, was killed by a singal, or exceeded limits,
- **Compilation error (CE)**: code was compiled during the test rather than before judgement, as can happen in compile-on-each-test problems,
- **Ignored (IG)**: the judge deemed it unnecessary to run the test because it would not affect the score; this is elaborated upon below.

The following verdict MAY be supported:

- **Security violation (SV)**: the program tried to perform an explicitly disallowed operation.

A comment is attached to each verdict that explains in detail what went wrong. Typically, this comment is not shown to the user as it make leak information that can make problem solving easier.


## 6.4. Submission verdicts

The verdicts of the submission on all tests are typically combined to produce a single verdict. This **submission verdict** is similar to per-test verdicts, but there are certain differences, so we list all the possible verdicts anyway:

- **Accepted (AC)**: the submission passed all tests successfully and is considered a full solution,
- **WA, RE, TL, ML, PE, IL, SV**: all tests failed, and the first failed test has this verdict; the test number is attached, and the short form SHOULD be `{VERDICT-ON-FAILED-TEST} {FAILED-TEST-NO}`,
- **Check failed (CF)**: any test failed with CF verdict; the test number is not attached,
- **Compilation error (CE)**: either compilation failed before the submission was judged, or during a per-test compilation in a compile-on-each-test problem.

Judges may define their own submission verdicts. Of common use are:

- **Rejected (RJ)**: an administrator or an automatic check deems the solution unsatisfactory for one reason or another,
- **Disqualified (DQ)**: the submission is considered to be against rules, e.g. due to security bypassing or cheating.


## 6.5. Points

In ICPC-style problems, all submissions are either considered accepted or wrong, and only the former is counted in the scoreboard. In this use case, it suffices to check that the submission verdict is AC.

However, solutions are seldom black or white. More often, they are partially correct, or at least that's the assumption IOI-style competitions run under.

This applies to submission judgement on two levels: firstly, the output of the submission on a test may be partially correct, and secondly, the submission may work correctly for a part of the test suite.

So there is another test verdict that MUST be supported:

- **Partial solution (PT)**: the solution printed a well-founded answer, for some definition of well-founded, and the checker graded it as partially correct.

What this means heavily depends on the problem: perhaps the solution is suboptimal but still "correct" (e.g. if the problem is to find as short Hamiltonian path as possible), or that only a part of the problem was solved (e.g. the length of an optimal path was printed, but not the path itself), or something else entirely.

A real number is attached to this verdict, called **test points**. In short form, this verdict should be presented as `PT {POINTS}`, with a space between `PT` and the points. The judge MUST be able to represent points from 0 to 100'000 or more with 0.0001 precision or better. `PT 0` MUST be differentiated from `WA`.

Another submission verdict which MUST be supported is introduced:

- **Partial solution (PT)**: the submission is partially correct: at least one test passed at least somewhat, but the solution is not considered AC.

This verdict MUST be used even if some tests (but not all) failed completely. A number called **submission points** is attached to this verdict. It has the same precision and limits as test points.

Typically, submission points are computed by adding up all test points, but there is a hiccup to be resolved--a test may have OK verdict that does not store any points, so there is nothing to add up.

For this reason, points are directly attached to tests in IOI-style problems. They are accessible to the strategy using the `test.points` field. These values should be considered the maximum points per test, and checkers and interactors that return the PT verdict SHOULD NOT return numbers larger than `test.points`, if that field is present. An AC test is considered to be worth `test.points` points. The judge MUST automatically convert `PT {MAX-POINTS}` to `OK` test verdict.

It should be explicitly noted that `test.points` is orthogonal to the PT test verdict: it can be omitted if OK verdict is never emitted, and it can be used even if the PT test verdict is never emitted, e.g. in some IOI-style problems.


## 6.6. Groups

Sometimes tests are independent and you can just add up their points and use that metric to find which solution is better. However, this often results in data crunching, attempts to guess tests, or other undesirable behavior from the contestants. This is what **groups** are for: the points for the tests of a group are only awarded if all tests pass.

A group contains of one or more tests and has two properties, called **policies**.

**Points policy** specifies how points are awarded. It can be of two values:

- `complete-group` means that if at least one test fails, no points are awarded for the test group whatsoever,
- `each-test` means that all tests are judged independently, as if the tests were not attached to any group.

The `complete-group` policy means that if any test of the group failed, all other tests can be safely ignored. The judge SHOULD use this for optimization and emit IG verdicts for skipped tests.

`complete-group` MUST NOT be used if PT test verdicts are emitted, because it is not obvious what the correct behavior should be in this case. Possible interpretations include taking the minimum of the points and multiplying it by the test count, or requiring that all tests are one hundred percent successful, or requiring that the sum of points or the number of passed tests is above the given limit. If either of this is intended, a custom valuer or strategy should be used.

The other property is **feedback policy**, which shows how much information about the tests the user has access to. It can be of four values:

- `none` means that no information is revealed about the group and its tests whatsoever,
- `points` means that only the total points awarded for the group are shown,
- `icpc` means that the verdict and public metrics for the first failed test of the group are shown, as well as the number of that test, and if all tests pass, the maximum across public metrics is shown (e.g. if one test uses 1s and 200MB, and another test uses 2s and 100MB, 2s and 200MB are shown),
- `complete` means that the verdict and public metrics for all tests of the group are shown.

If points are disabled, the tests that are not attached to any group are assumed to be in a separate group with `complete-group` points policy and `icpc` feedback policy. If points are enabled, they are assumed to be in a separate group with `each-test` points policy and `points` feedback policy.

If the feedback policy is `none` and the points policy is `complete-group`, the judge MAY judge the tests of the group out of order, preferring tests that are more likely to fail first.

Additionally, a group may have **dependencies**, which are other groups that must all be passed for the present group to be judged. Dependencies form a directed acyclic graph. A test from a group a dependency of which did not pass MUST have IG verdict. A group is considered passed if all its tests have verdict OK.


## 6.7. Realtime verdicts

For better responsibility, some judges show testing progress while the submission is tested, including the number of points secured by the solution so far. This number is not simply the sum of points on the tests judged so far--that wouldn't work correctly--but the current lower bound on the points that the submission is guaranteed to get.

If the judge supports such realtime verdict assignment, and the strategy did not call `submission.disable_autorate`, the behavior is obvious. If `submission.disable_autorate` is called, `submission.rate` can be called multiple times: the last call sets the final verdict, and all other calls are realtime updates, which MUST all be either `AC` or `PT`.


## 6.8. Testlib output

Checkers and interactors usually use the [testlib](https://codeforces.com/testlib) library.

Testlib checkers are invoked as follows:

```shell
checker {INPUT-FILE} {PARTICIPANT-OUTPUT-FILE} {JURY-ANSWER-FILE}
```

Testlib interactors are invoked as follows:

```shell
interactor {INPUT-FILE} {LOG-FILE} {JURY-ANSWER-FILE} <{PARTICIPANT-STDOUT} >{PARTICIPANT-STDIN}
```

...and their log files are then passed to the checker as follows:

```shell
checker {INPUT-FILE} {LOG-FILE} {JURY-ANSWER-FILE}
```

But of more interest is how testlib programs yield verdicts. The exit code sets the verdict:

- `0` is for OK,
- `1` is for WA,
- `2` and `8` are for PE,
- `3` and `4` are for "graceful" CF (which usually means that the jury solution was deemed incorrect, e.g. if the jury answer was worse than the participant's answer),
- `7` is for PT.

Standard error stream contains a string representation of the verdict as well as an optional comment. The verdicts map to output as follows:

- OK: `ok {COMMENT}`,
- WA: `wrong answer {COMMENT}`,
- PE: `wrong output format {COMMENT}` or `unexpected eof {COMMENT}`,
- CF: `FAIL {COMMENT}`,
- PT: `points {POINTS} {COMMENT}`, where `{POINTS}` is a decimal number with `.` separator.

The judge MAY support other exit codes and verdicts. All unsupported exit codes MUST be interpreted as "hard" CF.
