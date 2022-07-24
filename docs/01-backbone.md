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

The second half is the specification of the proposed format itself:

- [7. Package](07-package.md)
- [8. Localization](08-localization.md)
- [9. Files](09-files.md)
- [10. Assets](10-assets.md)
- [11. Judging](11-judging.md)
- [12. Properties](12-properties.md)
- [13. Path patterns](13-path-patterns.md)

Finally, the last section is a link between the package format and strategies and describes how the former is translated to the latter:

- [14. Defaults](14-defaults.md)

The reader is advised to start with reading about the strategies and then proceed to the format description, but this is not strictly necessary.
