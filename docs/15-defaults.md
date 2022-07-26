# 15. Defaults

If a problem does not provide a strategy as specified in [11. Assets](11-assets.md), one is generated automatically. This section describes how this is done.


## 15.1. Options

At the very start of the strategy, options are configured.

If a scorer is listed in [11. Assets](11-assets.md), the strategy starts with:

```python
submission.disable_autorate()
```


## 15.2. Problem types

The problem type is determined. This type can be either of the following:

- `io`,
- `interactive`,
- `run-twice`,
- `run-twice-interactive`
- `run-twice-double-interactive`.

Firstly, the `<judging>` tag, which is described in detail in [12. Judging](12-judging.md), is consulted.

If it contains the field `run-count="2"`, the algorithm is as follows: if an interactor is not listed in `<assets>` (see [11. Assets](11-assets.md)), the type is `run-twice`. If it is listed, but the description does not contain a `<runs>` tag, it is considered `run-twice-interactive`. If the `<runs>` tag is present, it is considered `run-twice-double-interactive`.

If `<judging>` does not have the `run-count` field, the algorithm is as follows: if the tag `run-twice` is not present in `<tags>` (see [9. Localization](09-localization.md)), the type is `io`. Otherwise, the type is based on whether the program has an interactor: if it does, the type is `run-twice-double-interactive`, otherwise it is `run-twice`.


## 15.3. Compilation

At the start of the strategy, the user submission is compiled. Resources listed in [10. Files](10-files.md) that have matching `for-type` fields and have `compile` stage and `solution` asset enabled are compiled with the submission:

```python
# Pseudocode
libraries = []
for resource in resources:
    if resource.for_type.matches(submission.type) and "compile" in resource.stages and "solution" in resource.assets:
        libraries.append(resource.source)
# End pseudocode

user = await compile(submission, *libraries)
```

`compile` automatically determines which source files are parts of secure graders and links them safely, as described in [5. Grading](05-grading.md).


## 15.4. Limits and streams

Let us denote the fields `{TIME-LIMIT}`, `{MEMORY-LIMIT}`, `{INPUT-FILE}`, and `{OUTPUT-FILE}` from [12. Judging](12-judging.md) by `time_limit`, `memory_limit`, `input_file`, and `ouptut_file`, respectively.

For simplicity, we describe the function `invoke`, the call to which is almost equivalent to calling `user` directly.

Also, occasionally, the user is allowed to use files like `input.txt` and `output.txt` instead of standard streams. This is the place where such behavior is enabled. Let us denote the fields `{INPUT-FILE}` and `{OUTPUT-FILE}` from [12. Judging](12-judging.md) by `input_file` and `output_file` in the code below.

```python
def invoke(stdin, stdout):
    files = {}
    if input_file != "":
        files[input_file] = stdin
    if output_file != "":
        files[output_file] = stdout
    return user(
        stdin=stdin,
        stdout=stdout,
        files=files,
        limits=Limits(
            time=time_limit / 1000,
            memory=memory_limit,
            real_time=time_limit / 1000 * 2 + 0.1  # This line is implementation-defined
        )
    )
```


## 15.5. Test handling

After that, the following part is injected:

```python
for test in tests:
    with test:
```

This part is followed by code that depends on the problem type. For technical reasons, the code blocks in this and the following chapter (15.5.1 and forwards, up to and including 15.6) are not indented to two levels as they should be.


## 15.5.1. I/O

If the problem type is `io`, the following code is injected:

```python
output = File()
invoke(stdin=test.input, stdout=output)
checker(test.input, output, test.answer)
```


### 15.5.2. Interactive

If the problem type is `interactive`, the following code is injected:

```python
interactor_output = File()
i2u, u2i = Pipe(), Pipe()
invoke(stdin=i2u, stdout=u2i)
interactor(test.input, interactor_output.rw(), test.answer, stdin=u2i, stdout=i2u)
checker(test.input, interactor_output, test.answer)
```


### 15.5.3. Run-twice

If the problem type is `run-twice`, the following code is injected:

```python
run1_output, run2_input, run2_output = File(), File(), File()
invoke(stdin=test.input, stdout=run1_output)
checker(test.input, run1_output, test.answer, stdout=run2_input)
invoke(stdin=run2_input, stdout=run2_output)
checker(run2_input, run2_output, test.answer)
```


### 15.5.4. Run-twice interactive

If the problem type is `run-twice-interactive`, the following code is injected:

```python
run2_input, run2_output = File(), File()
i2u, u2i = Pipe(), Pipe()
invoke(stdin=i2u, stdout=u2i)
interactor(test.input, run2_input.rw(), test.answer, stdin=u2i, stdout=i2u)
invoke(stdin=run2_input, stdout=run2_output)
checker(run2_input, run2_output, test.answer)
```


### 15.5.5. Run-twice double interactive

If the problem type is `run-twice-double-interactive`, the following code is injected:

```python
run2_input, interactor_output = File(), File()
run1_i2u, run1_u2i, run2_i2u, run2_u2i = Pipe(), Pipe(), Pipe(), Pipe()
invoke(stdin=run1_i2u, stdout=run1_u2i)
interactor(test.input, run2_input.rw(), test.answer, stdin=run1_u2i, stdout=run1_i2u)
invoke(stdin=run2_i2u, stdout=run2_u2i)
interactor(run2_input, interactor_output.rw(), test.answer, stdin=run2_u2i, stdout=run2_i2u)
checker(run2_input, interactor_output, test.answer)
```


## 15.6. Points

If the field `{TREAT-POINTS-FROM-CHECKER-AS-PERCENT}` from [12. Judging](12-judging.md) is set, the following chunk is appended to each test:

```python
if isinstance(test.verdict, PE):
    test.verdict = PT(test.points * (test.verdict.points / 100))
```


## 15.7. Scoring

Finally, if a scorer is listed in [11. Assets](11-assets.md), another chunk is appended, based on the scorer type.


### 15.7.1. `codeforces`

```python
tests_passed = sum(test.verdict == OK for test in tests, 0)

scorer_in, scorer_out = File(), File()
scorer_in.write(f"{tests_passed}\n".encode())
await scorer(stdin=scorer_in, stdout=scorer_out)
points = int(scorer_out.read().decode())

submission.rate(PT(points))
```


### 15.7.2. `ejudge`

```python
scorer_in, scorer_out = File(), File()

scorer_in.write(f"{len(tests)}\n".encode())

for test in tests:
    r = {
        OK: 0,
        RE: 2,
        TL: 3,
        PE: 4,
        WA: 5,
        CF: 6,
        ML: 12,
        SV: 13,
        IL: 15,
        IG: 18
    }[test.verdict]

    if test.verdict == OK:
        s = test.points
    elif isinstance(test.verdict, PT):
        s = test.verdict.points
    else:
        s = 0

    t = int(max(metric.time for metric in test.metrics) * 1000)

    scorer_out.write(f"{r} {s} {t}\n".encode())

await scorer(stdin=scorer_in, stdout=scorer_out)
points = int(scorer_out.read().decode())

submission.rate(PT(points))
```
