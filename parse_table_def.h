#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

/*
typedef struct entry_t{
    bool isvalid;
    bool isReduce;
    uint32_t dest;
} entry_t;*/

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


typedef struct entry_t{
    bool isvalid;
    bool isReduce;
    uint32_t dest;
} entry_t;


int parse_table_width = 12;
int parse_table_height = 16;
entry_t parse_table_new[] = {
       /*      NULL |        STRING |          SPCL |           SQL |           SQR |           CCL |           CCR |           ORR |         REGEX |          EXPR |           GRP |           EOF | */
       {0, 0,      0}, {1, 0,      7}, {0, 0,      0}, {1, 0,     11}, {0, 0,      0}, {1, 0,     10}, {0, 0,      0}, {0, 0,      0}, {1, 0,      6}, {1, 0,      5}, {1, 0,      8}, {0, 0,      0},
       {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0},
       {0, 0,      0}, {1, 1,      2}, {1, 1,      2}, {1, 1,      2}, {1, 1,      2}, {1, 1,      2}, {1, 1,      2}, {1, 1,      2}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 1,      2},
       {0, 0,      0}, {1, 0,      7}, {0, 0,      0}, {1, 0,     11}, {0, 0,      0}, {1, 0,     10}, {1, 1,     10}, {0, 0,      0}, {1, 0,      2}, {1, 0,      5}, {1, 0,      8}, {1, 1,     10},
       {0, 0,      0}, {1, 1,      6}, {1, 1,      6}, {1, 1,      6}, {1, 1,      6}, {1, 1,      6}, {1, 1,      6}, {1, 1,      6}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 1,      6},
       {0, 0,      0}, {1, 0,      7}, {0, 0,      0}, {1, 0,     11}, {0, 0,      0}, {1, 0,     10}, {1, 1,      3}, {1, 0,      3}, {0, 0,      0}, {1, 0,      4}, {1, 0,      8}, {1, 1,      3},
       {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 1,      1},
       {0, 0,      0}, {1, 1,      9}, {1, 1,      9}, {1, 1,      9}, {1, 1,      9}, {1, 1,      9}, {1, 1,      9}, {1, 1,      9}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 1,      9},
       {0, 0,      0}, {1, 1,      5}, {1, 0,      9}, {1, 1,      5}, {1, 1,      5}, {1, 1,      5}, {1, 1,      5}, {1, 1,      5}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 1,      5},
       {0, 0,      0}, {1, 1,      4}, {1, 1,      4}, {1, 1,      4}, {1, 1,      4}, {1, 1,      4}, {1, 1,      4}, {1, 1,      4}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 1,      4},
       {0, 0,      0}, {1, 0,      7}, {0, 0,      0}, {1, 0,     11}, {0, 0,      0}, {1, 0,     10}, {0, 0,      0}, {0, 0,      0}, {1, 0,     14}, {1, 0,      5}, {1, 0,      8}, {0, 0,      0},
       {0, 0,      0}, {1, 0,     12}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0},
       {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 0,     13}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0},
       {0, 0,      0}, {1, 1,      8}, {1, 1,      8}, {1, 1,      8}, {1, 1,      8}, {1, 1,      8}, {1, 1,      8}, {1, 1,      8}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 1,      8},
       {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 0,     15}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0},
       {0, 0,      0}, {1, 1,      7}, {1, 1,      7}, {1, 1,      7}, {1, 1,      7}, {1, 1,      7}, {1, 1,      7}, {1, 1,      7}, {0, 0,      0}, {0, 0,      0}, {0, 0,      0}, {1, 1,      7}
};



static rule_t rules[11] = {
    {TERM_NULL, {TERM_NULL, TERM_NULL, TERM_NULL}, 1},
    {TERM_START, {TERM_REGEX, TERM_NULL, TERM_NULL}, 1},
    {TERM_REGEX, {TERM_EXPR, TERM_ORR, TERM_REGEX}, 3},
    {TERM_REGEX, {TERM_EXPR, TERM_NULL, TERM_NULL}, 1},

    {TERM_EXPR, {TERM_GRP, TERM_SPCL, TERM_NULL}, 2},
    {TERM_EXPR, {TERM_GRP, TERM_NULL, TERM_NULL}, 1},
    {TERM_EXPR, {TERM_EXPR, TERM_EXPR, TERM_NULL}, 2},

    {TERM_GRP, {TERM_CCL, TERM_REGEX, TERM_CCR}, 3},
    {TERM_GRP, {TERM_SQL, TERM_STRING, TERM_SQR}, 3},
    {TERM_GRP, {TERM_STRING, TERM_NULL, TERM_NULL}, 1},

    {TERM_REGEX, {TERM_EXPR, TERM_ORR, TERM_NULL}, 2}
};





#endif