# dxtionary-db
CLI interface to sqlite db for dxtionary



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