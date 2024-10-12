#ifndef REGEX_H
#define REGEX_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


enum terms{NULL_TYPE=0, STRING_T=1, SPCL=2, SQL=3, SQR=4, CCL=5, CCR=6, ORR=7};
enum non_terms{REGEX=0, EXPR=1, GRP=2, START=3};


enum all_terms{TERM_NULL=0, TERM_STRING=1, TERM_SPCL=2, TERM_SQL=3, TERM_SQR=4, \
                TERM_CCL=5, TERM_CCR=6, TERM_ORR=7, TERM_REGEX=8, TERM_EXPR=9, \
                TERM_GRP=10, TERM_START=11, TERM_EOF=12};
            
typedef struct generic_token_t{
    enum all_terms type;
} generic_token_t;

typedef struct rule_t {
    enum all_terms dest;
    enum all_terms src[3];
    int n_srcs;
} rule_t;



typedef struct token_t{
    enum terms type;
    int start_index;
    int stop_index;
    int length;
} token_t;


void make_parse_table();

token_t *tokenize(char *inpbuffer, uint32_t length);

void traverse_graph(token_t *input_stream);

#endif