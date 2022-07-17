# problem.xml specification

**[Start reading &rightarrow;](docs/01-backbone.md)**

This repository contains the specification of the **problem.xml format** for competitive competition problems.

It is well-known that many online judge systems use incompatible problem formats, which significantly worses vendor lock-in, makes it much harder to implement a new system, to host a contest locally, or to build new utilities for contest management. To the best of the author's knowledge, no fully standardized format exists, so they have taken upon themselves the responsibility of providing said documentation.

The **problem.xml format** is based on the format used by [Polygon](https://polygon.codeforces.com), a popular tool most a large fraction of the community uses to prepare programming contest problems. This format aims to be fully compatible with the packages produced by Polygon, effectively defining a superset of the format Polygon generates at the moment. The judge-neutral naming of the format is to prevent misunderstanding and signify a new, standardized, and interoperable format.

The author hopes that the similarity of the **problem.xml format** to the Polygon format, which has widespread support and is considered the unofficial industry standard among many circles, will ease its adoption.

It is suggested that the **problem.xml format** is used for storage purposes and inter-system communication, including but not limited to generation, archivation, and publication of problem packages. In contrast, how the problems are represented internally in judge systems is out of scope of this document and should be decided by the developers.

The standard is written in RFC style but with the goal of being accessible to a novice reader, supplied with examples and comments. The backbone of the standard is defined in [1. Backbone](docs/01-backbone.md), which should be considered the starting point of the documentation.

The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in this document are to be interpreted as described in [RFC 2119](https://datatracker.ietf.org/doc/html/rfc2119).
