# 9. Files

The tag `<files>` of `problem.xml` is of the following structure:

```xml
<files>
    <resources>
        {RESOURCE1}
        {RESOURCE2}
        {...}
    </resources>
    <executables>
        {EXECUTABLE1}
        {EXECUTABLE2}
        {...}
    </executables>
</files>
```


## 9.1. Executables

This section lists all executable files that are compiled from source, such as checkers and validators, without differentiating between them in any way. This section does not store any information that cannot be acquired from other locations of the `problem.xml` file and serves only as a compatibility mechanism.

Executable files are described in the following way:

```xml
<executable>
    <source path="{SOURCE-PATH}" type="{SOURCE-TYPE}" />
    <binary path="{BINARY-PATH}" type="{BINARY-TYPE}" />
</executable>
```

All paths are package-relative. `{SOURCE-TYPE}` and `{BINARY-TYPE}` are described in [3. Types](03-types.md).


## 9.2. Resources

The tag `<resources>` describes each resource file either in the following "short" format:

```xml
<file path="{PATH}" [type="{TYPE}"] />
```

or the following "long" format:

```xml
<file [for-type="{FOR-TYPE}"] path="{PATH}" type="{TYPE}" [location="{LOCATION}"]>
    <stages>
        <stage name="{STAGE1}" />
        <stage name="{STAGE2}" />
        {...}
    </stages>
    <assets>
        <asset name="{ASSET1}" />
        <asset name="{ASSET2}" />
        {...}
    </assets>
</file>
```

Checkers, validators, and other scripts are all listed among resources, as are other files.

If `{TYPE}` is not present in the short format, the file is assumed to be used by the preparation system internally and is ignored. `olymp.sty`, `problem.tex`, and `statements.ftl` are common examples.

Otherwise, the resources represent files and directories that are used during compilation or evaluation of submissions and problem-related programs, such as checkers and interactors. `{TYPE}` is the source type as described in [3. Types](03-types.md).

Stages describe *when* the resource is used, while assets describe *with what* the resource is to be linked.

The following stages are presently supported:

- `compile`
- `run`

The following assets are recognized:

- `validator`
- `interactor`
- `checker`
- `solution`

For `compile` stage, the behavior is just like when multiple arguments are passed to `compile` function by the strategy. Read more at [4. Compilation](04-compilation.md). For example, stage `compile` plus asset `checker` mean that a source code file will be passed to the compiler along with the checker source code.

The `{LOCATION}` field describes where the file should be put on the filesystem. The path is relative to the submission source code during `compile` stage, and the executable file during `run` stage. For example, if a file is stored in location `Eigen/Dense`, a C++ submission can access it using `#include "Eigen/Dense"`. If the `{LOCATION}` field is not present, it is assumed to be equal to the file name specified in `{PATH}`, that is, the file is put next to the program that is being built. This enables a backwards-compatible hierarchical filesystem structure.

If the asset `solution` is listed, `{FOR-TYPE}` MUST be a source type mask as described in [3. Types](03-types.md). If `solution` is not listed, `{FOR-TYPE}` field MUST NOT be present.

As this scheme is non-orthagonal, judges MUST allow the same path to be used in several `<file>` records. However, any two such records MUST NOT refer to the same location, stage, asset, and `{FOR-TYPE}` at once. For compatibility, preparation systems SHOULD abstain from generating duplicates if it is possible to realize the requested behavior without them.

The short format is equivalent to

```xml
<file for-type="*" path="{PATH}" type="{TYPE}">
    <stages>
        <stage name="compile" />
    </stages>
    <assets>
        <asset name="validator" />
        <asset name="interactor" />
        <asset name="checker" />
    </assets>
</file>
```

Notice that the `{FOR-TYPE}` of the short format is always `*`, which means the judge MUST NOT take resource types into consideration. That is, even if the checker is written in C++, short-format resources with types other than `cpp` and `cpp-h` MUST be made available to the compiler too.

Also note that the `compile` stage should be used for libraries and graders even for interpreted languages, and that secure graders are listed in one more place, as described in [10. Assets](10-assets.md).
