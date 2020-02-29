# dns-txt-lookup

A CLI tool for looking up DNS TXT records.

## Usage

    dns-txt-lookup [-m max_records_count] [-s separator] [-0] domain

Parameters:

* `-m max_records_count`: The maximum number of TXT records to list.
* `-s separator`: The separator to use between TXT records. Default: new line character (`\n`).
* `-0`: Use null byte (`\0`) as the separator.
* `domain`: The domain to lookup TXT records for.

## Dependencies

* libc
* libresolv

## Build

You can use make to build this tool:

    make

## Alternatives

Using `dig` and `awk`:

    dig -t txt +short example.org | awk -F'"' '{print $2}'

Using `host` and `awk`:

    host -t txt example.org | awk -F'"' '{print $2}'

## LICENSE

MIT License. See [LICENSE](LICENSE) for more information.

