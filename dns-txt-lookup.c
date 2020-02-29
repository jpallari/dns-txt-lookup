#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

#define BUF_LEN 4096

struct cli_opts {
    char *domain;
    int max_records;
    char *separator;
};

void print_ns_error(int error, const char *domain);
int parse_cli_opts(struct cli_opts *opts, int argc, char *argv[]);
void print_cli_opts_error(char *argv0, int error_code);

int main(int argc, char **argv) {
    int return_code = 0;
    struct __res_state state;

    // Parse CLI args
    struct cli_opts opts;
    int cli_opts_code = parse_cli_opts(&opts, argc, argv);
    if (cli_opts_code != 0) {
        print_cli_opts_error(argv[0], cli_opts_code);
        goto fail;
    }

    // Find TXT records
    if (res_ninit(&state) != 0) {
        fprintf(stderr, "state initialization failed\n");
        goto fail;
    }
    unsigned char response[BUF_LEN];
    int response_len;
    if ((response_len = res_nquery(&state, opts.domain, C_IN, T_TXT, response, BUF_LEN)) < 0) {
        print_ns_error(h_errno, opts.domain);
        goto fail;
    }

    // Parse the records
    ns_msg handle;
    if (ns_initparse(response, response_len, &handle) < 0) {
        fprintf(stderr, "initparse failed: %s\n", strerror(errno));
        goto fail;
    }

    // Record count
    u_int16_t record_count = ns_msg_count(handle, ns_s_an);
    if (record_count == 0) {
        fprintf(stderr, "no records found\n");
        goto fail;
    }
    if (opts.max_records > 0 && opts.max_records < record_count) {
        record_count = opts.max_records;
    }

    // Print the TXT records
    ns_rr record;
    for (int i = 0; i < record_count; i++) {
        if (ns_parserr(&handle, ns_s_an, i, &record) < 0) {
            fprintf(stderr, "failed to parse record: %s\n", strerror(errno));
            goto fail;
        }
        printf(
            "%.*s%s",
            ns_rr_rdlen(record) - 1,
            ns_rr_rdata(record) + 1,
            opts.separator
        );
    }

    goto exit;

fail:
    return_code = 1;
exit:
    // Clean up and exit
    res_nclose(&state);
    return return_code;
}

void print_ns_error(int error, const char *domain) {
    switch(error) {
    case HOST_NOT_FOUND:
        fprintf(stderr, "unknown zone: %s\n", domain);
        break;
    case NO_DATA:
        fprintf(stderr, "no NS records for %s\n", domain);
        break;
    case TRY_AGAIN:
        fprintf(stderr, "no response for NS query\n");
        break;
    default:
        fprintf(stderr, "unexpected error\n");
        break;
    }
}

int parse_cli_opts(struct cli_opts *opts, int argc, char **argv) {
    int opt;

    // Init config
    opts->max_records = 0;
    opts->separator = "\n";


    while ((opt = getopt(argc, argv, "0m:s:")) != -1) {
        switch (opt) {
            case '0':
                opts->separator = "\0";
                break;
            case 's':
                opts->separator = argv[optind - 1];
                break;
            case 'm':
                opts->max_records = atoi(optarg);
                break;
            default:
                return 1; // Invalid parameters
        }
    }

    if (optind >= argc) {
        return 2; // Missing domain argument
    }
    opts->domain = argv[optind];

    return 0;
}

void print_cli_opts_error(char *argv0, int error_code) {
    switch (error_code) {
        case 1:
            fprintf(stderr, "usage: %s [-m max_records_count] [-s separator] [-0] domain\n", argv0);
            break;
        case 2:
            fprintf(stderr, "no domain specified\n");
            break;
    }
}

