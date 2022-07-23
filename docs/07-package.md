# 7. Package

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


## 7.1. problem.xml structure

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

The tags `<names>`, `<statements>`, `<tutorials>`, and `<tags>` are necessary for correct presentation of the problem in the user interface. Their contents are described in [8. Localization](08-localization.md).

The tag `<files>` describes the files stored in the package. This is elaborated on in [9. Files](09-files.md).

The tag `<assets>` describes the various problem-wide programs and scripts stored in the package. This is elaborated on in [10. Assets](10-assets.md).

The tag `<judging>` describes the tests, limitations, and various testing information. This is elaborated on in [11. Judging](11-judging.md).

<!--
    <judging {...}>{...}</judging>
    <properties>{...}</properties>
    <stresses>{...}</stresses>
-->

Implementations MAY add their own tags to the `<problem>` tag if necessary. Key-value information that is not directly related to judging SHOULD be added to `<properties>`.
