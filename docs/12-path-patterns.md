# 12. Path patterns

Path patterns were mentioned in [10. Assets](10-assets.md) and [11. Judging](11-judging.md) when describing how tests are referenced.

Instead of a list of file paths, one for each test, a single C format string is provided. This pattern MUST be a string containing exactly one `%` character, immediately followed either by `d`, or `0` and then a number followed by `d`. For instance, `tests/%d`, `tests/%d.in`, and `tests/%02d.in` are valid patterns.

This limitation also implies that the file name cannot contain `%`, but this is quite reasonable for cross-platform file paths.

If the percent sign is immediately followed by `d`, it means the decimal representation of the test number replaces `%d`. Otherwise, if the pattern looks like `%0{PADDING}d`, the decimal representation of the test number is padded with zeroes at the left until it is at least `{PADDING}` characters long.
