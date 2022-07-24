# 12. Properties

Properties are various key-value options that control either judge or preparation system behavior.

Properties are specified in the problem.xml file in the following format:

```xml
<properties>
    <property name="{NAME1}" value="{VALUE1}" />
    <property name="{NAME2}" value="{VALUE2}" />
    {...}
</properties>
```

At the moment, the only property is:

- `tests-wellformed` (`true` or `false`): whether all tests are expected to:
  - have each line finished with LF,
  - not contain characters with codes less than 32,
  - not contain leading or trailing spaces,
  - not contain two or more consecutive spaces,
  - not contain leading or trailing empty lines,
  - be non-empty.
