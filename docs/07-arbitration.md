# 7. Arbitration

In certain competitions, submissions are not isolated, so to speak: the verdict of a submission may depend on other submissions too. Some use cases are:

- Problems without a known optimal solution, when the points depend on the best result among all solutions, e.g. `100 * points / best points`.

- Place-based points: the best submission gets 100 points, the second best submission gets 90 points, etc.

- CTF-style grading: the maximum points for a problem degrade when more people solve the problem.

An **arbiter** is effectively an interactive program that assigns and updates verdicts of submissions. Unlike other scripts, it is not started from scratch for each submission; rather, it is launched once and remains online during the whole contest. The arbiter MUST be deterministic: if it crashes or if the machine that runs the arbiter is shut down, all data passed to the arbiter so far can simply be sent again.

If the problem has offline tests (i.e. there are more tests than pretests), the arbiter is restarted after the contest ends and all submissions verdicts are reassigned.

Arbitration is the only process not directly controlled by the strategy.


## 7.1. Protocol

The arbiter receives new submissions from standard input and prints verdicts and verdict modifications to standard output.

At the start, the arbiter receives a JSON object on standard input on a single line, with the following fields:

- `type`: always `"init"`,
- `testset`: what testset the submissions are judged on, either `pretests` or `tests`.

Each time the strategy calls `submission.rate`, a JSON object is passed to the arbiter via standard input, terminated by a newline character. This object has the following fields:

- `type`: always `"update"`,
- `timestamp`, `id`, `author`, `contest_id`, and `metadata` as described in [2. Pipelines](02-pipelines.md),
- `verdict`: the short string form of the preliminary verdict, e.g. `AC`, `PT 50.0`, or `CE`,
- `comment`: the rate comment as passed to `submission.rate`,
- `final`: a boolean, signifying whether this is the last call to `submission.rate`.

Note that the timestamps are not necessarily increasing: an earlier submission may be judged more slowly than a later submission.

At any moment, the arbiter may print a JSON object to standard input, terminated by a newline character. This object MUST have the following fields:

- `id`: the submission identifier, matching the one sent to the arbiter by the judge,
- `verdict`: the short string form of the verdict as seen by the user.

Of course, the arbiter may output several updates in response to a single submission.
