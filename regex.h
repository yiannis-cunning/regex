#ifndef REGEX_H
#define REGEX_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <string.h>

enum all_terms{TERM_NULL=0, TERM_STRING=1, TERM_SPCL=2, TERM_SQL=3, TERM_SQR=4, \
                TERM_CCL=5, TERM_CCR=6, TERM_ORR=7, TERM_REGEX=8, TERM_EXPR=9, \
                TERM_GRP=10, TERM_EOF=11, TERM_START=12, TERM_CHRSET=13};
            
typedef struct generic_token_t{
    enum all_terms type;
    int start_index;
    int stop_index;
    int length;
} generic_token_t;

typedef struct rule_t {
    enum all_terms dest;
    enum all_terms src[3];
    int n_srcs;
} rule_t;


// Parse tree and Abstract syntax node
class parse_tree_node_t{
public:
    generic_token_t self_type;
    parse_tree_node_t * parent;
    std::vector<parse_tree_node_t *> childs;

    parse_tree_node_t(generic_token_t type){self_type = type; parent=NULL;}
    virtual ~parse_tree_node_t(){
        for(unsigned i=0;i<childs.size(); i += 1){
            delete childs[i];
        }
    }
};

class nfa_node_t{
public:
    bool isfinish;
    std::vector<nfa_node_t *> arrows;
    std::vector<char> input;    // 0 is epsilon

    nfa_node_t(){isfinish = false;};
};

class nfa_t{
public:
    nfa_node_t *head;
    std::vector<nfa_node_t *> ends;
    nfa_t(){head = NULL;};
};


class dfa_node_t{
public:
    bool isfinish;
    std::vector<dfa_node_t *> arrows;
    std::vector<char> input;    // 0 is epsilon

    dfa_node_t(){isfinish = false;};
};

class dfa_t{
public:
    dfa_node_t *head;
    std::vector<dfa_node_t *> ends;
    dfa_t(){head = NULL;}
};



typedef struct regex_t{
    dfa_t *dfa;
} regex_t;

int make_regex(char *str, regex_t *dest);

bool match(regex_t regex, char *str);

#endif