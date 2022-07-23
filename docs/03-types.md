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

Executable types have a similar format, though they usually don't contain a version. The full form of an executable type is `{KIND}.{FORMAT}={FORMAT-VERSION}.{ARCH}.{PLATFORM}`, where `{KIND}` is one of the following:

- `exe` for stand-alone executables, e.g. normal applications,
- `dylib` for dynamically linked libraries,
- `obj` for object files, e.g. statically linked libraries or archives.

The `.{ARCH}.{PLATFORM}` or `.{PLATFORM}` suffix can be omitted for some formats, just like `={FORMAT-VERSION}`.

Raw, non-executable types that don't depend on the architecture (e.g. ZIP archives) use the syntax `raw.{FORMAT}`.


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

For compilers with enabled diagnostics, `-diagnostics` is appended to the version of the implementation.

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


### 3.3.4. Common Lisp

The language identifier `cl` is appointed to the Common Lisp programming language. The only version is `default`, referring to ANSI INCITS 226-1994.

The implementation identifier `clisp` is assigned to CLISP, `ccl` is assigned to Clozure CL, and `gcl` is assigned to GNU Common Lisp. The default implementation is `clisp`.

As Common Lisp is an oversimple language, the following Quicklisp modules SHOULD be provided:

- [bordeaux-threads](https://github.com/sionescu/bordeaux-threads),
- [cl-containers](https://cl-containers.common-lisp.dev/),
- [cl-data-structures](https://sirherrbatka.github.io/cl-data-structures/main.html),
- [fset](https://quickdocs.org/fset),
- [genhash](https://github.com/pnathan/genhash),
- [hash-set](https://github.com/samebchase/hash-set),
- [listopia](https://github.com/Dimercel/listopia),
- [pileup](https://nikodemus.github.io/pileup/),
- [str](https://github.com/vindarel/cl-str),
- [syscamore](https://ndantam.github.io/sycamore/).


### 3.3.5. D

The language identifier `d` is appointed to the D programming language. The versions match the official releases.

The implementation identifier `dmd` is assigned to DMD, `gdc` is assigned to the GNU D Compiler, and `ldc` is assigned to the LLVM-based D Compiler. The default implementation is `dmd`.


### 3.3.6. Dart

The language identifier `dart` is appointed to the Dart programming language. The versions match the official releases.

The one and only implementation identifier is `dart`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.7. Elixir

The language identifier `elixir` is appointed to the Elixir programming language. The versions match the official releases.

The one and only implementation identifier is `elixir`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.8. Erlang

The language identifier `erlang` is appointed to the Erlang programming language. The versions match the official releases.

The one and only implementation identifier is `erlang`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.9. Fortran

The language identifier `fortran` is appointed to the Fortran programming language. The versions are `90`, `95`, `2003`, `2008`, and `2018`.

The only implementation identifier supported at the moment is `gfortran`. Ergo this is the default implementation.

As Fortran does not have a built-in standard library, the [unofficial stdlib](https://stdlib.fortran-lang.org/) SHOULD be provided.


### 3.3.10. Go

The language identifier `go` is appointed to the Go programming language. The versions match the official releases.

The implementation identifier `go` is assigned to the official Go compiler, and `gccgo` is assigned to the GNU Go Compiler. The default implementation is `go`.


### 3.3.11. Haskell

The language identifier `haskell` is appointed to the Haskell programming language. `1-0` till `1-4`, `98`, and `2010` are supported versions of the language.

The implementation identifier `ghc` is assigned to the Glorious Glasgow Haskell Compilation System, `uhc` is assigned to the Utrecht Haskell Compiler, and `lhc` is assigned to the LLVM Haskell Compiler. The default implementation is `ghc`.

Additionally, the following packages SHOULD be available:

- array,
- bytestring,
- containers,
- heaps,
- mtl,
- random,
- text,
- unordered-containers,
- vector.


### 3.3.12. Java

The language identifier `java` is appointed to the Java programming language. The versions match the official releases of Java SE.

The implementation identifier `hotspot` is assigned to HotSpot, the official Java virtual machine, and `graalvm` is assigned to GraalVM. The default implementation is `hotspot`. As OpenJDK has long been the official one, JDKs are not differentiated.

Additionally, the following types SHOULD be recognized:

- `java11`, meaning `java=11`,
- `java8`, meaning `java=8`.


### 3.3.13. JavaScript

The language identifier `javascript` is appointed to the ECMAScript programming language. The versions match the editions of the standard, e.g. `2017` for ES2017.

The implementation identifier `node` is assigned to Node.js, and `v8` is assigned to [d8](https://v8.dev/docs/d8). The default implementation is `node`.

Additionally, the following types SHOULD be recognized:

- `js.{SOMETHING}`, meaning `javascript.{SOMETHING}`,
- `nodejs`, meaning `javascript.node`.


### 3.3.14. Kotlin

The language identifier `kotlin` is appointed to the Kotlin programming language. The versions match the official releases.

The implementation identifier `hotspot` is assigned to HotSpot, the official Java virtual machine, and `graalvm` is assigned to GraalVM. The default implementation is `hotspot`. As OpenJDK has long been the official one, JDKs are not differentiated.

Additionally, the following type SHOULD be recognized:

- `kotlin16`, meaning `kotlin=1-6-0`.


### 3.3.15. Lua

The language identifier `lua` is appointed to the Lua programming language. The versions match the official releases.

The one and only implementation identifier is `lua`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.16. Nim

The language identifier `nim` is appointed to the Nim programming language. The versions match the official releases.

The one and only implementation identifier is `nim`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.17. OCaml

The language identifier `ocaml` is appointed to the OCaml programming language. The versions match the official releases.

The implementation identifier `opt` is assigned to `ocamlopt`, and `bytecode` is assigned to `ocamlc`. The default implementation is `opt`.


### 3.3.18. Pascal

The language identifier `pas` is appointed to the Pascal programming language. The only version is `default`, referring to ISO 7185.

The implementation identifier `fpc` is assigned to Free Pascal, `dpr` is assigned to Delphi, `abc` is assigned to PascalABC.NET, and `borland` is assigned to Turbo (Borland) Pascal. The default implementation is `fpc`.

For Free Pascal, the Free Component Library SHOULD be available.

Additionally, the following type SHOULD be recognized:

- `delphi.borland`, meaning `pascal.dpr`.


### 3.3.19. Perl

The language identifier `perl` is appointed to the Perl programming language. The versions match the official releases. Perl 6 is `raku`, not `perl=6`.

The one and only implementation identifier is `perl`, with version matching that of the language. Ergo this is the default implementation.


### 3.3.20. PHP

The language identifier `php` is appointed to the PHP programming language. The versions match the official releases.

The one and only implementation identifier is `php`, with version matching that of the language. Ergo this is the default implementation.

The extensions SPL and DS SHOULD be available.


### 3.3.21. Python

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


### 3.3.22. Raku

The language identifier `raku` is appointed to the Raku programming language, formerly known as Perl 6. The versions match the official releases.

The implementation identifier `rakudo` is assigned to Rakudo, the official Raku compiler, and `nqp` is assigned to NQP. The default implementation is `rakudo`.


### 3.3.23. Ruby

The language identifier `ruby` is appointed to the Ruby programming language. The versions match the official releases.

The one and only implementation identifier is `yarv`, assigned to Yet another Ruby VM, with version matching that of the language. Ergo this is the default implementation.


### 3.3.24. Rust

The language identifier `rust` is appointed to the Rust programming language. The versions are `{EDITION}-{VERSION}`, where `{EDITION}` is the year, and `{VERSION}` matches the official releases.

The implementation identifier `rustc` is assigned to the official compiler, and `mrustc` is assigned to [mrustc](https://github.com/thepowersgang/mrustc). The default implementation is `rustc`. Versions of rustc are equal to those of the language itself, but without the `{EDITION}-` prefix. Nightly versions have `-nightly` suffix.

The following crates SHOULD be available:

```
cfg-if = "1.0.0"
itertools = "0.10.2"
lazy-static = "1.4.0"
libc = "0.2"
nix = "0.24.1"
pin-project = "1"
rand = "0.8.5"
regex = "1.5.6"
smallvec = "1.9.0"
```


### 3.3.25. Scala

The language identifier `scala` is appointed to the Scala programming language. The versions match the official releases.

The implementation identifier `hotspot` is assigned to HotSpot, the official Java virtual machine, and `graalvm` is assigned to GraalVM. The default implementation is `hotspot`. As OpenJDK has long been the official one, JDKs are not differentiated.


## 3.4. Supported binary types

This standard also assigns types to common binary types. If a format from the list below is supported by the judge, it MUST support the appointed type as it is written in this standard. For formats not listed here, the followers of this specification are advised to use their good judgement and create a pull-request updating this specification to include the new type.

Executable files can be reasonably split into two big groups: native executables which the operating system can execute directly, and interpreted executables which need a runtime to work. While the former list only contains a few entries, the latter often includes an entry per language implementation, which somewhat complicates manners. Both groups are described here.


### 3.4.1. Archive

The format identifier `ar` is appointed to the System V/GNU Archive format, produced by the `ar` utility and having extension .a. It only supports `obj` kind. The only version is `default`.

The supported platforms are `linux`, `systemv`, and `hurd`.


### 3.4.2. ELF

The format identifier `elf` is appointed to the Executable and Linkable Format. It supports all three kinds: `exe` for executables, `dylib` for .so files, and `obj` for .o files. The only version is `default`.

The supported platforms are `linux`, `netbsd`, `freebsd`, `openbsd`, `systemv`, and `hurd`.


### 3.4.3. Mach-O

The format identifier `macho` is appointed to the Mach-O format. It supports all three kinds: `exe` for executables, `dylib` for bundles and .dylib files, and `obj` for .o files. The only version is `default`.

The only supported platform is `macos`.


### 3.4.4. PE

The format identifier `pe` is appointed to the Portable Executable format. It supports all three kinds: `exe` for executables, `dylib` for .dll files, and `obj` for .obj files. The only version is `default`.

The only supported platform is `nt`.

This specification includes both PE32 and PE32+ into the PE format.


### 3.4.5. CLISP FAS

The format identifier `fas` is appointed to the compiled files generated by CLISP. It only supports `obj` kind. The versions match CLISP versions.

Both the architecture and the platform fields are omitted.


### 3.4.6. BEAM

The format identifier `beam` is appointed to the BEAM VM files, used by Elixir and Erlang. It only supports `obj` kind. The versions match Erlang versions.

Both the architecture and the platform fields are omitted.


### 3.4.6. JVM

The format identifier `jvm` is appointed to the files used by the JVM machine and produced by Java, Scala, and Kotlin compilers. It supports the `obj` kind for class files and the `exe` kind for JAR files. The versions match official version of the class format.

Both the architecture and the platform fields are omitted.


### 3.4.7. Luac

The format identifier `luac` is appointed to the compiled files generated by Lua compiler. It only supports `obj` kind. The versions match Lua versions.

Both the architecture and the platform fields are omitted.


### 3.4.8. CPython

The format identifier `cpython` is appointed to the compiled files generated by CPython. It only supports `obj` kind. The versions match CPython versions.

Both the architecture and the platform fields are omitted.


### 3.4.9. PyPy

The format identifier `pypy` is appointed to the compiled files generated by PyPy. It only supports `obj` kind. The versions match PyPy versions.

Both the architecture and the platform fields are omitted.


### 3.4.10. MoarVM

The format identifier `moarvm` is appointed to MoarVM. It only supports `obj` kind. The versions match official versions.

Both the architecture and the platform fields are omitted.


### 3.4.11. YARV

The format identifier `yarv` is appointed to the bytecode created by [RubyVM](https://ruby-doc.org/core-2.5.1/RubyVM/InstructionSequence.html). It only supports `obj` kind. The versions match Ruby versions.

Both the architecture and the platform fields are omitted.


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
