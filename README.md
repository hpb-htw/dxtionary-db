# dxtionary-db
CLI interface to sqlite db for dxtionary



## Usage

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
