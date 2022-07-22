# 8. Localization

A problem may have several statements. This feature is commonly used for localization, but other reasons exist. For example, this may serve as an anti-cheating mechanism.

The variable `{LANGUAGE}`, when used in the explanation below, MUST be either an IANA-accepted [language tag](https://www.w3.org/International/articles/language-tags/), such as `en-US` or `ru-RU`, or an identifier from [this fixed list](languages.txt) that maps to the appropriate language tag.

The field `{IDENTIFIER}` can be used to further disambiguate behavior when two statements of one language are used and is subject to the same limitations as `{SHORT-NAME}`.

Localization applies to tags `<names>`, `<statements>`, and `<tutorials>`, the contents of which are described below. The tag `<tags>` is to be handled differently, but it is also localized.


## 8.1. Names

The tag `<names>` has the following structure:

```xml
<names>
    <name language="{LANGUAGE1}" value="{NAME1}" [id="{IDENTIFIER1}"] />
    <name language="{LANGUAGE2}" value="{NAME2}" [id="{IDENTIFIER2}"] />
    {...}
</names>
```

`{NAME}` is the name (title) of a problem, which can be arbitrary Unicode string that will be shown to the user in the judge interface.


## 8.2. Statements

The tag `<statements>` has the following structure:

```xml
<statements>
    {STATEMENT1}
    {STATEMENT2}
    {...}
</statements>
```

`{STATEMENT}` contains information on plain-text statements, usually in TeX format:

```xml
<statement [charset="UTF-8"] language="{LANGUAGE}" [mathjax="{FORMULAE-ENABLED}"] path="{PATH}" type="{TYPE}" [id="{IDENTIFIER}"] {...} />
```

`{PATH}` is a package-relative POSIX path to the statement file. `{TYPE}` MUST be a MIME-type, e.g. `application/x-tex` for TeX, `application/pdf` for PDF, or `text/html` for HTML. For plain-text formats, the field `charset` MUST be present and equal to `UTF-8` (or lowercase). For binary formats, it MUST be omitted.

The flag `{FORMULAE-ENABLED}`, attached to a misnamed property name, if present, MUST be `true` or `false`. The value of `true` indicates that the file contains plain-text formulae that must be rendered programmatically, rather than prerendered (vector) graphics. The default value is `false`.

Judges MUST prefer compiled statements in PDF and HTML formats to plain-text source formats, such as TeX.

Various examples:

```xml
<statement charset="UTF-8" language="russian" mathjax="true" path="statements/russian/problem.tex" type="application/x-tex" />
<statement charset="UTF-8" language="uzbek" mathjax="true" path="statements/uzbek/problem.tex" type="application/x-tex" />
<statement language="russian" path="statements/.pdf/russian/problem.pdf" type="application/pdf" />
<statement language="uzbek" path="statements/.pdf/uzbek/problem.pdf" type="application/pdf" />
<statement charset="UTF-8" language="russian" mathjax="true" path="statements/.html/russian/problem.html" type="text/html" />
<statement charset="UTF-8" language="uzbek" mathjax="true" path="statements/.html/uzbek/problem.html" type="text/html" />
```


## 8.3. Tutorials

The tag `<tutorials>` has the exact same structure as the `<statements>` tag, except that `<tutorial>` is used instead of `<statement>`.


## 8.4. Tags

The tag `<tags>` has the following structure:

```xml
<tags>
    <tag [value="{TAG-EN1}"] [value-{LANGUAGE1-1}="{TAG-LANGUAGE1-1}" value-{LANGUAGE2-1}="{TAG-LANGUAGE2-1}" {...}] />
    <tag [value="{TAG-EN2}"] [value-{LANGUAGE1-2}="{TAG-LANGUAGE1-2}" value-{LANGUAGE2-2}="{TAG-LANGUAGE2-2}" {...}] />
    {...}
</tags>
```

Historically, the tags were only specified in English and then translated into other languages automatically. The author deems this too limiting and enables localization of tags.

For each language `{LANGUAGE}`, the tag should be specified in property `value-{LANGUAGE}`, except if the language is `en` or `english`, in which case it should be present in property `value`.

If `value` is provided and the translation to the user's locale is not, and the tag is listed in the [default list](tags.txt), the judges SHOULD attempt to translate the tag automatically.

Notice that while most tags are informational, there are tags that affect the behavior of the judge. This is abuse of tags, and so this documents only lists two tags that necessiate special handling:

- `run-twice`, which makes the problem implicitly run-twice, mode ALL_RUNS_INTERACTIVE. This is described in more detail later;
- `hide_checker_comment`, which stops the judge from showing the checker's comment on samples. This tag SHOULD NOT be shown literally among other tags.
