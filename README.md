# dxtionary-db
CLI interface to sqlite db for dxtionary

[![Build status](https://ci.appveyor.com/api/projects/status/vjp4au63vmaqe0c6?svg=true)](https://ci.appveyor.com/project/hpb-htw/dxtionary-db)



## Usage
### Query a word
To get an entry of a word in database:
```sh
./dxtionary-db <DATABASE> <QUERY>
```

Ouput

```text
col1 = value 1
col2 = value 2

col1 = value 1
col2 = value 2
...
```

To get easy-to-parse output, `<QUERY>` can use `json_quote()` function.


### Import raw dictionary file to database

```$bash
import-raw-dict <Database file> <Dictionary file>+
```

require exact one Database file and at least one dictionary file.
`<Dictionary file>+` have following structure:

```csv
header-1<separator>header-2<separator>header-3
cell-1-1<separator>cell-1-2<separator>cell-1-3
cell-2-1<separator>cell-2-2<separator>cell-2-3
....
```

for this version the delimiter must be exactly `<separator`. This rule may be changed in next versions.



