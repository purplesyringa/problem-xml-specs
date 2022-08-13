# 11. Assets

Assets are the various problem-wide programs and scripts, for example, the checker. We will provide a stripped-down version of the format of the `<assets>` tag first, just how it appears in classical problems, and then generalize that.

The basic structure of the tag `<assets>` of `problem.xml` is:

```xml
<assets>
    {CHECKER}
    [{SCORER}]
    [{ARBITER}]
    [{INTERACTOR}]
    [<validators>{VALIDATOR}</validators>]
    <solutions>
        {SOLUTION1}
        {SOLUTION2}
        {...}
    </solutions>
    [{PROGRAMS}]
    [{STRATEGY}]
</assets>
```

The checker is always present, and the interactor may be omitted. The tags `<validators>` and `<solutions>` may be omitted if no validators or model solutions are present respectively.


## 11.1. Checker

The checker is listed as follows:

```xml
<checker [name="{NAME}"] type="testlib">
    {SOURCES}
    {BINARIES}
    [<copy path="{COPY-PATH}" />]
    <testset>
        <test-count>{CHECKER-TEST-COUNT}</test-count>
        <input-path-pattern>{CHECKER-TESTS-INPUT-PATTERN}</input-path-pattern>
        <output-path-pattern>{CHECKER-TESTS-OUTPUT-PATTERN}</output-path-pattern>
        <answer-path-pattern>{CHECKER-TESTS-ANSWER-PATTERN}</answer-path-pattern>
        <tests>
            {CHECKER-TEST1}
            {CHECKER-TEST2}
            {...}
        </tests>
    </testset>
</checker>
```

`{NAME}` is an optional checker identifier reserved to built-in checkers, e.g. `std::ncmp.cpp`. This field SHOULD NOT be used by the judge. Examples of built-in checkers are provided in [examples/std-checkers](examples/std-checkers), barring the `std::` prefix due to path name limitations.

The checker MUST be listed in `<executables>` according to [10. Files](10-files.md). `{SOURCES}` and `{BINARIES}` MUST match the values specified there.

If there is exactly one source file, the `<copy>` tag can be used to specify that it is also copied to `{COPY-PATH}`. This is to support legacy systems that require the checker to be present in the root of the package with name `check.cpp`. Judges SHOULD NOT rely on existence of this field.

`<testset>` contains information on checker unit tests. Patterns are filesystem path patterns as described in [14. Path patterns](14-path-patterns.md). `{CHECKER-TEST-COUNT}` MUST be equal to the number of nodes in `<tests>`. A single test has the following format:

```xml
<test verdict="{VERDICT}" />
```

The `{i}`-th node specifies the expected output of the checker on the `{i}`-th test. `{VERDICT}` is one of the following:

- `ok` for OK,
- `wrong-answer` for WA,
- `presentation-error` for PE,
- `crashed` for CF.


## 11.2. Scorer

The scorer, also known as valuer, is used to compile test verdicts to a single submission verdict. If present, it is listed as follows:

```xml
<scorer [type="{SCORER-TYPE}"]>
    {SOURCES}
    {BINARIES}
</scorer>
```

The scorer MUST be listed in `<executables>` according to [10. Files](10-files.md). `{SOURCES}` and `{BINARIES}` MUST match the values specified there.

`{SCORER-TYPE}` is one of the following values:

- `codeforces` (default),
- `ejudge`,
- `problem-xml`.

We will get to what it means later, in [15. Defaults](15-defaults.md).


## 11.3. Arbiter

The arbiter handles cross-submission verdict assignment as described in [7. Arbitration](07-arbitration.md). If present, it is listed as follows:

```xml
<arbiter [type="{ARBITER-TYPE}"]>
    {SOURCES}
    {BINARIES}
</arbiter>
```

The arbiter MUST be listed in `<executables>` according to [10. Files](10-files.md). `{SOURCES}` and `{BINARIES}` MUST match the values specified there.

`{ARBITER-TYPE}` MUST be `problem-xml`.


## 11.4. Interactor

The interactor, if present, is listed as follows:

```xml
<interactor>
    {SOURCES}
    {BINARIES}
    [{RUNS}]
</interactor>
```

The interactor MUST be listed in `<executables>` according to [10. Files](10-files.md). `{SOURCES}` and `{BINARIES}` MUST match the values specified there.

The `{RUNS}` field is used only for run-twice problems. If it is present, it MUST look exactly like:

```xml
<runs>
    <run>1</run>
    <run>2</run>
</runs>
```

We will get to what it means later, in [15. Defaults](15-defaults.md).


## 11.5. Validator

The validator, if present, is listed as:

```xml
<validator>
    {SOURCES}
    {BINARIES}
    <testset>
        <test-count>{VALIDATOR-TEST-COUNT}</test-count>
        <input-path-pattern>{VALIDATOR-TESTS-INPUT-PATTERN}</input-path-pattern>
        <tests>
            {VALIDATOR-TEST1}
            {VALIDATOR-TEST2}
            {...}
        </tests>
    </testset>
</validator>
```

The validator MUST be listed in `<executables>` according to [10. Files](10-files.md). `{SOURCES}` and `{BINARIES}` MUST match the values specified there.

`<testset>` contains information on validator unit tests. Patterns are filesystem path patterns as described in [14. Path patterns](14-path-patterns.md). `{VALIDATOR-TEST-COUNT}` MUST be equal to the number of nodes in `<tests>`. A single test has the following format:

```xml
<test verdict="{VERDICT}" />
```

The `{i}`-th node specifies the expected output of the checker on the `{i}`-th test. `{VERDICT}` may be either `valid` or `invalid`, with self-explanatory meanings.

Unlike checkers and interactors, validators are not used at runtime, but they are still testlib programs. Validators are invoked using

```shell
validator <{INPUT-FILE}
```

...and return verdicts via a testlib-style exit code, either OK or PE.


## 11.6. Model solutions

The `<solutions>` tag lists authors' solutions in the following format:

```xml
<solution [note="{NOTE}"] tag="{TAG}">
    {SOURCES}
    {BINARIES}
</solution>
```

The checker MUST NOT be listed in `<executables>`, because it is not necessary for judge operation, but `{SOURCES}` and `{BINARIES}` have the same format.

`{NOTE}` is an optional human-readable comment.

`{TAG}` specifies whether it is expected to pass or fail, and if the latter, how. The supported tags are:

- `main`: this solution passes and is used to generate jury answers; exactly one solution MUST have this tag;
- `accepted`: this solution passes;
- `rejected`: this solution fails, for any reason;
- `time-limit-exceeded`: this solution fails with TL verdict;
- `time-limit-exceeded-or-accepted`: this solution may either pass or fail with TL verdict;
- `time-limit-exceeded-or-memory-limit-exceeded`: this solution fails, either with TL or ML verdict;
- `wrong-answer`: this solution fails with WA verdict;
- `presentation-error`: this solution fails with PE verdict;
- `memory-limit-exceeded`: this solution fails with ML verdict;
- `failed`: the solution fails with CF verdict.


## 11.7. Other programs

Other programs are defined in the `<programs>` tag as follows:

```xml
<programs>
    {PROGRAM1}
    {PROGRAM2}
    {...}
</programs>
```

A single program is defined as:

```xml
<program name="{NAME}">
    {SOURCES}
    {BINARIES}
</program>
```

The program MUST be listed in `<executables>` according to [10. Files](10-files.md). `{SOURCES}` and `{BINARIES}` MUST match the values specified there.

The program will be accessible to the strategy by name `{NAME}`.

Technically, programs defined just inside `<assets>` could be moved into `<programs>`, if not for tests and other legacy options (except solutions, which are not really programs). The names `checker`, `interactor`, and `validator` can be used for this. For example, a checker can be defined as:

```xml
<program name="checker">
    {SOURCES}
    {BINARIES}
</program>
```

...and strategies will work just fine, but tests and the `<copy>` tag, if present, will be lost.

Similarly, an interactor can be defined as:

```xml
<program name="interactor">
    {SOURCES}
    {BINARIES}
</program>
```

...and, if the problem is not run-twice, this will be identical to a dedicated `<interactor>` tag, except for legacy judges that can't parse `<programs>`. For this reason, preparation systems SHOULD abstain from using `<programs>` for checkers, interactors, and validators.

In addition, the name `solution` is reserved and MUST NOT be used.


## 11.8. Strategy

Finally, problems that want to use a custom strategy as described in [2. Pipelines](02-pipelines.md), MUST use the following tag:

```xml
<strategy>
    <source path="{STRATEGY-PATH}" type="python^3" />
</strategy>
```

The file at `{STRATEGY-PATH}` MUST be a strategy script as described in [2. Pipelines](02-pipelines.md).
