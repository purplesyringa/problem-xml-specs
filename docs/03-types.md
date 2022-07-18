# 3. Types

Although we can talk about compilation and invocation abstractly, it is sometimes desirable to know what language a program is written in and what machine can run that program, and act on that information.

This is done by assigning types to source, executable, and raw files.

A type is an ASCII string containing fields spearated by dots `.`.

Source types contain information about what language and compiler the files are written for and what their formats are. Examples of source types, in increasing order of complexity:

- `python`, `c`,
- `python.cpython`, `c.gcc`,
- `python=3.cpython`, `c=90.gcc`,
- `python=3-7.cpython`, `c=90.gcc=9-4-0`,
- `c=90.gcc=9-4-0.amd64`,
- `c=90.gcc=9-4-0.amd64.posix`.

Formally, the full form of a source type looks like `{LANGUAGE}={LANGUAGE-VERSION}.{IMPLEMENTATION}={IMPLEMENTATION-VERSION}.{ARCH}.{PLATFORM}`. As the dot sign is already used, the dots in version fields MUST be replaced with dashes. For languages with multiple file kinds (e.g. C headers), `-{KIND}` is appended to the language field.

Executable types have a similar format, though they usually don't contain a version. The full form of an executable type is `{KIND}.{FORMAT}.{ARCH}.{PLATFORM}`, where `{KIND}` is one of the following:

- `exe` for stand-alone executables, e.g. normal applications,
- `dylib` for dynamically linked libraries,
- `obj` for object files, e.g. statically linked libraries or archives.

Finally, raw types that don't depend on the architecture use the syntax `raw.{FORMAT}`.


## 3.1. Masks

However, such accuracy is seldom needed. Therefore some fields can be omitted, and versions can be specified less rigidly. When a field is omitted, it is allowed to contain any value. If necessary, an explicit wildcard can be used: `python` is the same as `python=*.*=*.*.*`.

Versions can include wildcards in the middle, e.g. `python=3-7-*` or `c=gnu-*`. Additionally, simple inequalities can be used for the version, matching those used by NPM, Cargo, and other package managers:

- The equal sign `=` indicates an exact version: `python=3-6-9` and `python=3-8-10` mean versions 3.6.9 and 3.8.10 respectively,
- The greater-than-or-equal-to sign `>=` indicates everything starting with the given version: `python>=3-6-9` includes both 3.6.9, 3.6.10, 3.7.1, and 4.1.2,
- The `>`, `<`, `<=`, and `!=` signs work as well,
- The caret sign `^` is like `>=`, but the first number (major version) cannot be changed: `python^3-6-9` includes 3.6.9, 3.6.10, and 3.7.1, but not 4.1.2,
- The tilde sign `~` is like `^`, but the first two numbers (major and minor versions) cannot be changed: `python~3-6-9` includes 3.6.9 and 3.6.10, but not 3.7.1 or 4.1.2.

In all cases, comparison is lexicographic: the version is split by `-` into fields, and purely numeric fields are compared as integers. If a string and a number are in the same position, the string is considered less than the number.

There are two slightly different contexts in which types are used. Firstly, there are user submissions and problem-wide programs, like checkers. In this case, you provide a **rigid type**. Secondly, there are cases when a mask is needed, like with graders: the same grader should work for all implementations of C++. In this case, a **wildcard type** is provided.

For example, a checker written in C++ should perhaps use the type `cpp=17.gcc`, while a grader might use `cpp>=14`. The bounds are, unfortunately, often unclear: while a checker written in Python might use the type `python~3-8-10.cpython`, that would be too limiting: some judges might have slightly older or newer versions of Python installed, so it would probably be better to use `python^3-6.cpython` or alike for a checker, and `python^3-6` or alike for a grader.

If masks match several types supported by the judge, the default implementation MUST be preferred if the judge is aware of the language, and if there are still multiple types left, the newest version of the language MUST be chosen.


## 3.2. Supported platforms

The rarely used fields `{ARCH}` and `{PLATFORM}` in the mask may have the following values:

`{ARCH}`:

- `amd64` for amd64 (x86-64),
- `i386` for x86,
- self-describing: `arm64`, `armel`, `armhf`, `mipsel`, `mips64el`, `ppc64el`, `s390x`.

`{PLATFORM}`:

- `nt` for Windows NT,
- `linux` for Linux,
- `darwin` for macOS,
- `netbsd` for NetBSD,
- `freebsd` for FreeBSD,
- `openbsd` for OpenBSD,
- `systemv` for System V,
- `hurd` for GNU Hurd.


## 3.3. Supported languages

This standard assigns types to some common languages and implementations. If a language from the list below is supported by the judge, it MUST support the appointed type as it is written in this standard. For languages not listed here, the followers of this specification are advised to use their good judgement and create a pull-request updating this specification to include the new type.


### 3.3.1. C

The language identifiers `c` and `c-h` are appointed to the C programming language. The former is used for source files, while the latter is used for header files. Versions of the two languages are versions of the ISO standard: `c=90` is ISO C90, `c=17` is ISO C17, and so on. For GCC extensions, `c=gnu-90` and alike should be used.

The implementation identifier `gcc` is assigned to the GNU C Compiler (e.g. `c.gcc=9-4-0` for GCC 9.4.0), `clang` is assigned to Clang (e.g. `c.clang=10-0-0` for Clang 10.0.0), `msvc` is assigned to the Microsoft C compiler (e.g. `c.msvc=7.0` for MSC 7.0), and `icc` is assigned to the Intel C Compiler (e.g. `c.icc=19.0`). The default implementation is `gcc`.

Additionally, the following type SHOULD be recognized:

- `c.visual`, meaning `c.msvc`.


### 3.3.2. C++

The language identifiers `cpp` and `cpp-h` are appointed to the C++ programming language. The former is used for source files, while the latter is used for header files. Versions of the language are versions of the ISO standard: `cpp=17` is C++17, and so on. For GCC extensions, `cpp=gnu-17` and alike should be used.

The implementation identifier `gcc` is assigned to GCC, `clang` is assigned to Clang, `msvc` is assigned to the Microsoft Visual C++ compiler, and `icc` is assigned to the Intel C++ Compiler (e.g. `cpp.icc=19.0`). The legacy identifier `g++` is an alias for `gcc` and SHOULD NOT be used in new problems. The default implementation is `gcc`.

For compilres with enabled diagnostics, `-diagnostics` is appended to the version of the implementation.

Additionally, the following types SHOULD be recognized:

- `cpp.g++11`, meaning `cpp=11.gcc`,
- `cpp.g++14`, meaning `cpp=14.gcc`,
- `cpp.g++17`, meaning `cpp=17.gcc`,
- `cpp.ms2017`, meaning `cpp=17.msvc`,
- `cpp.msys2-mingw64-9-g++17`, meaning `cpp=17.gcc.*.nt`,
- `cpp.visual`, meaning `cpp.msvc`,
- `h.g++`, meaning `cpp-h`.


### 3.3.3. C#

The language identifier `csharp` is appointed to the C# programming language. The versions match the official releases.

The implementation identifier `net` is assigned to the outdated CSC compiler, and `mono` is assigned to the Mono C# compiler (also known as msc; the versions match Mono releases). The default implementation is `mono`.

Additionally, the following type SHOULD be recognized:

- `csharp.visual`, meaning `csharp`.


### 3.3.4. D

The language identifier `d` is appointed to the D programming language. The versions match the official releases.

The implementation identifier `dmd` is assigned to DMD, `gdc` is assigned to the GNU D Compiler, and `ldc` is assigned to the LLVM-based D Compiler. The default implementation is `dmd`.


### 3.3.5. Go

The language identifier `go` is appointed to the Go programming language. The versions match the official releases.

The implementation identifier `go` is assigned to the official Go compiler, and `gccgo` is assigned to the GNU Go Compiler. The default implementation is `go`.


### 3.3.6. Haskell

The language identifier `haskell` is appointed to the Haskell programming language. `1-0` till `1-4`, `98`, and `2010` are supported versions of the language.

The implementation identifier `ghc` is assigned to the Glorious Glasgow Haskell Compilation System, `uhc` is assigned to the Utrecht Haskell Compiler, and `lhc` is assigned to the LLVM Haskell Compiler. The default implementation is `ghc`.


### 3.3.7. Java

The language identifier `java` is appointed to the Java programming language. The versions match the official releases of Java SE.

The implementation identifier `hotspot` is assigned to HotSpot, the official Java virtual machine, and `graalvm` is assigned to GraalVM. The default implementation is `hotspot`. As OpenJDK has long been the official one, JDKs are not differentiated.

Additionally, the following types SHOULD be recognized:

- `java11`, meaning `java=11`,
- `java8`, meaning `java=8`.


### 3.3.8. JavaScript

The language identifier `javascript` is appointed to the ECMAScript programming language. The versions match the editions of the standard, e.g. `2017` for ES2017.

The implementation identifier `node` is assigned to Node.js, and `v8` is assigned to [d8](https://v8.dev/docs/d8). The default implementation is `node`.

Additionally, the following types SHOULD be recognized:

- `js.{SOMETHING}`, meaning `javascript.{SOMETHING}`,
- `nodejs`, meaning `javascript.node`.


### 3.3.9. Kotlin

The language identifier `kotlin` is appointed to the Kotlin programming language. The versions match the official releases.

The implementation identifier `hotspot` is assigned to HotSpot, the official Java virtual machine, and `graalvm` is assigned to GraalVM. The default implementation is `hotspot`. As OpenJDK has long been the official one, JDKs are not differentiated.

Additionally, the following type SHOULD be recognized:

- `kotlin16`, meaning `kotlin=1-6-0`.


### 3.3.10. OCaml

The language identifier `ocaml` is appointed to the OCaml programming language. The versions match the official releases.

The implementation identifier `opt` is assigned to `ocamlopt`, and `bytecode` is assigned to `ocamlc`. The default implementation is `opt`.


### 3.3.11. Pascal

The language identifier `pas` is appointed to the Pascal programming language. The only version of the standard is `default`, referring to ISO 7185.

The implementation identifier `fpc` is assigned to Free Pascal, `dpr` is assigned to Delphi, `abc` is assigned to PascalABC.NET, and `borland` is assigned to Turbo (Borland) Pascal. The default implementation is `fpc`.

Additionally, the following type SHOULD be recognized:

- `delphi.borland`, meaning `pascal.dpr`.


### 3.3.12. Perl

The language identifier `perl` is appointed to the Perl programming language. The versions match the official releases. Perl 6 is `raku`, not `perl=6`.

The one and only implementation identifier is `perl`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.13. PHP

The language identifier `php` is appointed to the PHP programming language. The versions match the official releases.

The one and only implementation identifier is `php`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.14. Python

The language identifier `python` is appointed to the Python programming language. The versions match the official releases.

The implementation identifier `cpython` is assigned to CPython, and `pypy` is assigned to PyPy. The default implementation is `cpython`. Versions of CPython are equal to those of the language itself.

Additionally, the following types SHOULD be recognized:

- `python.2`, meaning `python^2`,
- `python.3`, meaning `python^3`,
- `python2`, meaning `python^2`,
- `python3`, meaning `python^3`,
- `python.pypy2`, meaning `python^2.pypy`,
- `python.pypy3`, meaning `python^3.pypy`,
- `pypy2`, meaning `python^2.pypy`,
- `pypy3`, meaning `python^3.pypy`.


### 3.3.15. Raku

The language identifier `raku` is appointed to the Raku programming language, formerly known as Perl 6. The versions match the official releases.

The one and only implementation identifier is `raku`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.16. Ruby

The language identifier `ruby` is appointed to the Ruby programming language. The versions match the official releases.

The one and only implementation identifier is `ruby`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.17. Rust

The language identifier `rust` is appointed to the Rust programming language. The versions are `{EDITION}-{VERSION}`, where `{EDITION}` is the year, and `{VERSION}` matches the official releases.

The implementation identifier `rustc` is assigned to the official compiler, and `mrustc` is assigned to [mrustc](https://github.com/thepowersgang/mrustc). The default implementation is `rustc`. Versions of rustc are equal to those of the language itself, but without the `{EDITION}-` prefix. Nightly versions have `-nightly` suffix.


### 3.3.18. Scala

The language identifier `scala` is appointed to the Scala programming language. The versions match the official releases.

The implementation identifier `hotspot` is assigned to HotSpot, the official Java virtual machine, and `graalvm` is assigned to GraalVM. The default implementation is `hotspot`. As OpenJDK has long been the official one, JDKs are not differentiated.


## 3.4. Supported binary types

This standard also assigns types to common binary types. If a format from the list below is supported by the judge, it MUST support the appointed type as it is written in this standard. For formats not listed here, the followers of this specification are advised to use their good judgement and create a pull-request updating this specification to include the new type.


### 3.4.1. Archive

The format identifier `ar` is appointed to the System V/GNU Archive format, produced by the `ar` utility and having extension .a. It only supports `obj` format.

The supported platforms are `linux`, `systemv`, and `hurd`.


### 3.4.2. ELF

The format identifier `elf` is appointed to the Executable and Linkable Format. It supports all three kinds: `exe` for executables, `dylib` for .so files, and `obj` for .o files.

The supported platforms are `linux`, `netbsd`, `freebsd`, `openbsd`, `systemv`, and `hurd`.


### 3.4.3. Mach-O

The format identifier `macho` is appointed to the Mach-O format. It supports all three kinds: `exe` for executables, `dylib` for bundles and .dylib files, and `obj` for .o files.

The only supported platform is `macos`.


### 3.4.4. PE

The format identifier `pe` is appointed to the Portable Executable format. It supports all three kinds: `exe` for executables, `dylib` for .dll files, and `obj` for .obj files.

The only supported platform is `nt`.

This specification includes both PE32 and PE32+ into the PE format.


## 3.5. Supported raw types

This standard also assigns types to common raw types. If a format from the list below is supported by the judge, it MUST support the appointed type as it is written in this standard. For formats not listed here, the followers of this specification are advised to use their good judgement and create a pull-request updating this specification to include the new type.


### 3.5.1. Plain-text

The format identifier `text` is appointed to plain text data.

Additionally, the following type SHOULD be recognized:

- `text`, meaning `raw.text`.


### 3.5.2. Tar and compressed tar

The format identifier `tar` is appointed to the non-compressed Tape Archive format. `tar-gz` and `tar-bz2` are appointed to gzipped tar and bzipped tar formats, respectively.


### 3.5.3. ZIP

The format identifier `zip` is appointed to the ZIP format.
