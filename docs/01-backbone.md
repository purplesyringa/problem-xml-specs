# 1. Backbone

## 1.1. Preface

In this document, the word **judge** refers to a computer system which manages a collection of **problems**, allows users to send **submissions** to the problems, and rates the submissions according to some rules. The term **preparation system** refers to utilities used to create and maintain problems.

Although the focus of this format is on competitive programming competitions, when the problems are programming tasks and submissions are source code files, it naturally applies to a more wide set of goals.

This document assumes the reader is familiar with programming contest competitions. Problem setting experience is not required, but would be of help.


## 1.2. Introductory comments

The two main topics of this document is how submissions are tested and how the problems are represented on disk. Unfortunately, the two are historically highly interconnected, which worsens the learning curve. This standard attempts to separate the two slightly, introducing a simple intermediate format used for the former and generated from the latter.

The reader is advised to read [2. Pipelines](02-pipelines.md) first and then proceed towards [3. Package](03-package.md) and forwards.


## 1.2. Problem package

A **package** is a directory (often compressed into a zip file) containing all information necessary to run a contest with the problem, including but not limited to:

- Problem metadata,
- Statements,
- Optional tutorials and editorials,
- Invoker process configuration,
- Tests and commands to generate them,
- Checkers, validators, and other auxiliary scripts,
- Model solutions,
- A configuration file defining how to use and where to find the above.

The heart of the package is the `problem.xml` file, which is stored in the root of the package. All other files, barring some exceptions, are stored in the appropriate subdirectories.


## 1.3. problem.xml structure

`problem.xml` MUST be an UTF-8-encoded XML file of a specific structure described below. Hereinafter `{VARIABLE-NAME}` means variable content, `[...]` means optional content, and `{...}` stands for snipped complex structured data which is elaborated on later.

```xml
<?xml version="1.0" encoding="utf-8" standalone="no"?>
<problem short-name="{SHORT-NAME}" [revision="{REVISION}"] [url="{DOWNLOAD-URL}"]>
    <names>{...}</names>
    <statements>{...}</statements>
    <tutorials>{...}</tutorials>
    <judging {...}>{...}</judging>
    <files>{...}</files>
    <assets>{...}</assets>
    <properties>{...}</properties>
    <stresses>{...}</stresses>
    <tags>{...}</tags>
    {...}
</problem>
```

`{SHORT-NAME}` is an internal identifier of the problem. It is usually not seen by the end users, but SHOULD be human-readable and MUST consist of one or more characters among: Latin alphabet, digits, and dashes. Preparation systems, such as Polygon, SHOULD ensure no two managed problems have the same short name, but as problems can be prepared manually, judges SHOULD NOT rely on short names for uniqueness.

`{REVISION}` is an optional version of the problem, which, if present, MUST be a positive integer. Although this field can be used as a quicker alternative to comparing a hashsum, its primary goal is to ensure the package is not accidentally downgraded. Implementations are advised to warn upon a downgrading attempt.

`{DOWNLOAD-URL}`, if present, MUST be an URL to the package directory or zip file, requiring authorization if needed.

The tags `<names>`, `<statements>`, `<tutorials>`, and `<tags>` are necessary for correct presentation of the problem in the user interface. Their contents are described in [2. Localization](02-localization.md).

The tag `<files>` describes the files stored in the package. This is elaborated on in [3. Files](03-files.md).
