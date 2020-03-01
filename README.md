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

You can use `make` to build this tool:

    make

Which produces a binary named `dns-txt-lookup`.

## Examples

Lookup all TXT records for `example.org`:

    dns-txt-lookup example.org

Lookup only one TXT record for `example.org`:

    dns-txt-lookup -m 1 example.org

Use `XXX` as the separator between TXT records:

    dns-txt-lookup -s XXX example.org

Use the null byte (`\0`) as the separator:

    dns-txt-lookup -0 example.org

## Alternatives

Using `dig` and `awk`:

    dig -t txt +short example.org | awk -F'"' '{print $2}'

Using `host` and `awk`:

    host -t txt example.org | awk -F'"' '{print $2}'

## LICENSE

MIT License. See [LICENSE](LICENSE) for more information.

