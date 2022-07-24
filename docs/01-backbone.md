# 1. Backbone

## 1.1. Preface

In this document, the word **judge** refers to a computer system which manages a collection of **problems**, allows users to send **submissions** to the problems, and rates the submissions according to some rules. The term **preparation system** refers to utilities used to create and maintain problems.

Although the focus of this format is on competitive programming competitions, when the problems are programming tasks and submissions are source code files, it naturally applies to a more wide set of goals.

This document assumes the reader is familiar with programming contest competitions. Problem setting experience is not required, but would be of help.


## 1.2. Introductory comments

The two main topics of this document is how submissions are tested and how the problems are represented on disk. Unfortunately, the two are historically highly interconnected, which worsens the learning curve. This standard attempts to separate the two by introducing an intermediate format used for the former and generated from the latter.

This standard starts with two unrelated parts.

The first half of this standard describes an intermediary format--**strategies**--and how submissions would be judged based on this format:

- [2. Pipelines](02-pipelines.md)
- [3. Types](03-types.md)
- [4. Compilation](04-compilation.md)
- [5. Grading](05-grading.md)
- [6. Valuation](06-valuation.md)
- [7. Arbitration](07-arbitration.md)

The second half is the specification of the proposed format itself:

- [8. Package](08-package.md)
- [9. Localization](09-localization.md)
- [10. Files](10-files.md)
- [11. Assets](11-assets.md)
- [12. Judging](12-judging.md)
- [13. Properties](13-properties.md)
- [14. Path patterns](14-path-patterns.md)

Finally, the last section is a link between the package format and strategies and describes how the former is translated to the latter:

- [15. Defaults](15-defaults.md)

The reader is advised to start with reading about the strategies and then proceed to the format description, but this is not strictly necessary.
