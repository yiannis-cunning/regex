// consider (, ), +, *, |
//
// Consider an alphabet Sigma = {a-z, A-Z 0-9, _}
/*

0) Input stream of ASCI characters - 1-255, 0 is EOF.

1) Illegal symbol parsing
Allow a-z, A-Z, 0-9, ...
All inputs are either (a) spcl or (b) nrml

2) Input stream of terminals

3) Define the productions of a valid regex

abc|bca

(abs)+
abs
(stick*stack+[]...)
abs* | sitck?

*****GRAMAR*****
regex,expr,grp,chrset,string = 5 non-terminals
S <-> regex
regex <-> expr | expr <|> regex
expr <-> expr expr | grp spcl expr | grp expr | 
grp <-> <(> regex <)>
grp <-> <[> chrset <]> | string
chrset <nrml> chrset | <nrml> <-> <nrml> chrset |
string <-> nrml string |

*****TOKENS*****
8 terminals
<|> = |
<)> = )
<(> = (
<]> = ]
<[> = [
<-> = -
spcl = +*?
nrml = a-zA-Z0-9

nrml <-> NOT SPECIAL
spcl <-> +-*()[]?


[a-x]?[0-9]back+|3

<[> a <-> x
<[> a <-> x chrset
<[> chrset <]>
grp <?> expr
expr <[> 0 <-> 9
expr <[> 0 <-> 9 chrset
expr <[> chrset <]>
expr grp expr
expr expr
expr nrml
expr nrml nrml nrml nrml string
expr string
expr grp
expr grp <+>
expr grp <+> expr
expr exrp
expr
expr <|> nrml
expr <|> nrml string
expr <|> string
expr <|> grp
expr <|> grp expr
expr <|> expr
expr <|> regex
regex


4) Build a parse tree
table:
---terminals------non-terminals---
|              |                 |
states         |
|              |



LR0: 
state_stack; stack; input buffer; action; 


*/

#include "regex.h"
#include <stdbool.h>

//#include <bits/stdc++.h>
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector

//#define PRINT_DEBUG

static int n_terms = 8;
static int n_non_terms = 3;
static int n_states = 16;

typedef struct entry_t{
    bool isvalid;
    bool isReduce;
    uint32_t dest;
} entry_t;

#include "parse_table_def.h"

static entry_t *parse_table = NULL;


void passert(bool cond, const char *msg){
    if(!cond){printf("%s", msg); exit(1);}
}

const char *names_terms[] = {(char *)"EOF", "STRING", "SPCL", "SQL", "SQR", "CCL", "CCR", "ORR"};
const char *names_non_terms[] = {"REGEX", "EXPR", "GRP"};


void pparse_table(){
    printf("    \n");
    printf("       ");
    for(int i = 0; i < n_terms; i += 1){
        printf(" %7s |", names_terms[i]);
    }
    for(int i =0; i <n_non_terms; i += 1){
        printf(" %7s |", names_non_terms[i]);
    }
    printf("\n");


    for(int i = 0; i < n_states; i += 1){
        printf("%6d|", i);
        for(int j = 0; j < n_terms; j += 1){
            if(parse_table[i*(n_terms + n_non_terms) + j].isvalid){
                printf(" %7d |", parse_table[i*(n_terms + n_non_terms) + j].dest);
            } else{
                printf("         |");
            }
        }
        for(int j =0; j <n_non_terms; j += 1){
            if(parse_table[i*(n_terms + n_non_terms) + j + n_terms].isvalid){
                printf(" %7d |", parse_table[i*(n_terms + n_non_terms) + j + n_terms].dest);
            } else{
                printf("         |");
            }
        }
        printf("\n");
        
    }

}

void pparse_table2(){
    printf("    \n");
    //printf("       ");
    for(int i = 1; i < n_terms; i += 1){
        printf(" %7s |", names_terms[i]);
    }
    for(int i =0; i <n_non_terms; i += 1){
        printf(" %7s |", names_non_terms[i]);
    }
    for(int i = 0; i < 1; i += 1){
        printf(" %7s |", names_terms[i]);
    }
    printf("\n");


    for(int i = 0; i < n_states; i += 1){
        //printf("%6d|", i);
        for(int j = 1; j < n_terms; j += 1){
            if(parse_table[i*(n_terms + n_non_terms) + j].isvalid){
                printf(", {1, %d, %6d}", parse_table[i*(n_terms + n_non_terms) + j].isReduce, parse_table[i*(n_terms + n_non_terms) + j].dest);
            } else{
                printf(", {0, 0,      0}");
            }
        }
        for(int j =0; j <n_non_terms; j += 1){
            if(parse_table[i*(n_terms + n_non_terms) + j + n_terms].isvalid){
                printf(", {1, %d, %6d}", parse_table[i*(n_terms + n_non_terms) + j + n_terms].isReduce, parse_table[i*(n_terms + n_non_terms) + j + n_terms].dest);
            } else{
                printf(", {0, 0,      0}");
            }
        }
        for(int j = 0; j < 1; j += 1){
            if(parse_table[i*(n_terms + n_non_terms) + j].isvalid){
                printf(", {1, %d, %6d}", parse_table[i*(n_terms + n_non_terms) + j].isReduce, parse_table[i*(n_terms + n_non_terms) + j].dest);
            } else{
                printf(", {0, 0,      0}");
            }
        }
        printf("\n");
        
    }

}


/*
static rule_t r1 = {TERM_START, {TERM_REGEX, TERM_NULL, TERM_NULL}, 1};
static rule_t r2 = {TERM_REGEX, {TERM_EXPR, TERM_ORR, TERM_REGEX}, 3};
static rule_t r3 = {TERM_REGEX, {TERM_EXPR, TERM_NULL, TERM_NULL}, 1};

static rule_t r4 = {TERM_EXPR, {TERM_GRP, TERM_SPCL, TERM_NULL}, 2};
static rule_t r5 = {TERM_EXPR, {TERM_GRP, TERM_NULL, TERM_NULL}, 1};
static rule_t r6 = {TERM_EXPR, {TERM_EXPR, TERM_EXPR, TERM_NULL}, 2};

static rule_t r7 = {TERM_GRP, {TERM_CCL, TERM_REGEX, TERM_CCR}, 3};
static rule_t r8 = {TERM_GRP, {TERM_SQL, TERM_STRING, TERM_SQR}, 3};
static rule_t r9 = {TERM_GRP, {TERM_STRING, TERM_NULL, TERM_NULL}, 1};

static rule_t r10 = {TERM_REGEX, {TERM_EXPR, TERM_ORR, TERM_NULL}, 2};

static rule_t rules[9];*/



//static uint32_t parse_table[n_non_terms + n_terms][n_states];
// Entry is either shift and go to other state, or reduce by rule x
//      make MSB = shift/reduce and lower bits are either 
#define SET_PARSE_ENTRY_TERMINAL(terminal, isreduce, value) parse_table[statenum*(n_terms + n_non_terms) + terminal] = (entry_t) {1, isreduce, value};
#define SET_PARSE_ENTRY_NON_TERMINAL(nonterminal, isreduce, value) parse_table[statenum*(n_terms + n_non_terms) + nonterminal + n_terms] = (entry_t) {1, isreduce, value};

#define SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(value) for(int i = 0; i < n_terms; i += 1){parse_table[statenum*(n_terms + n_non_terms) + i] = (entry_t) {1, 1, value};}

void make_parse_table(){
    if(parse_table != NULL){return;}
    parse_table = (entry_t *)calloc(sizeof(entry_t), n_states*(n_terms + n_non_terms));


    // State 0
    int statenum = 0;
    SET_PARSE_ENTRY_TERMINAL( STRING_T, 0, 7)
    SET_PARSE_ENTRY_TERMINAL( SQL, 0, 11)
    SET_PARSE_ENTRY_TERMINAL( CCL, 0, 10)

    SET_PARSE_ENTRY_NON_TERMINAL( REGEX, 0, 6)
    SET_PARSE_ENTRY_NON_TERMINAL( EXPR, 0, 5)
    SET_PARSE_ENTRY_NON_TERMINAL( GRP, 0, 8)

    // State 1 does not exist

    // State 2
    statenum = 2;
    SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(2);

    // State 3
    statenum = 3;
    SET_PARSE_ENTRY_TERMINAL( STRING_T, 0, 7)
    SET_PARSE_ENTRY_TERMINAL( SQL, 0, 11)
    SET_PARSE_ENTRY_TERMINAL( CCL, 0, 10)

    SET_PARSE_ENTRY_TERMINAL( CCR, 1, 10)
    SET_PARSE_ENTRY_TERMINAL( NULL_TYPE, 1, 10)

    SET_PARSE_ENTRY_NON_TERMINAL( REGEX, 0, 2)
    SET_PARSE_ENTRY_NON_TERMINAL( EXPR, 0, 5)
    SET_PARSE_ENTRY_NON_TERMINAL( GRP, 0, 8)

    // State 4
    statenum = 4;
    SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(6)

    // State 5
    statenum = 5;
    SET_PARSE_ENTRY_TERMINAL( STRING_T, 0, 7)
    SET_PARSE_ENTRY_TERMINAL( SQL, 0, 11)
    SET_PARSE_ENTRY_TERMINAL( CCL, 0, 10)
    SET_PARSE_ENTRY_TERMINAL( CCR, 1, 3)
    SET_PARSE_ENTRY_TERMINAL( ORR, 0, 3)
    SET_PARSE_ENTRY_TERMINAL( NULL_TYPE, 1, 3)
    

    SET_PARSE_ENTRY_NON_TERMINAL( EXPR, 0, 4)
    SET_PARSE_ENTRY_NON_TERMINAL( GRP, 0, 8)

    // State 6
    statenum = 6;
    SET_PARSE_ENTRY_TERMINAL( NULL_TYPE, 1, 1)

    // State 7
    statenum = 7;
    SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(9)

    // State 8
    statenum = 8;
    SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(5)
    SET_PARSE_ENTRY_TERMINAL( SPCL, 0, 9)
    
    // State 9
    statenum = 9;
    SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(4)

    // State 10
    statenum = 10;
    SET_PARSE_ENTRY_TERMINAL( STRING_T, 0, 7)
    SET_PARSE_ENTRY_TERMINAL( SQL, 0, 11)
    SET_PARSE_ENTRY_TERMINAL( CCL, 0, 10)

    SET_PARSE_ENTRY_NON_TERMINAL( REGEX, 0, 14)
    SET_PARSE_ENTRY_NON_TERMINAL( EXPR, 0, 5)
    SET_PARSE_ENTRY_NON_TERMINAL( GRP, 0, 8)

    // State 11
    statenum = 11;
    SET_PARSE_ENTRY_TERMINAL( STRING_T, 0, 12)

    // State 12
    statenum = 12;
    SET_PARSE_ENTRY_TERMINAL( SQR, 0, 13)

    // State 13
    statenum = 13;
    SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(8)

    // State 14
    statenum = 14;
    SET_PARSE_ENTRY_TERMINAL( CCR, 0, 15)

    // State 15
    statenum = 15;
    SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(7)

    /*
    rules[0] = r1;
    rules[1] = r1;
    rules[2] = r2;
    rules[3] = r3;
    rules[4] = r4;
    rules[5] = r5;
    rules[6] = r6;
    rules[7] = r7;
    rules[8] = r8;
    rules[9] = r9;
    rules[10] = r10;*/

    pparse_table();
}


// Want to make the parse tree by traversing the graph


#define ALPHA(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x >= '0' && x <= '9') || x == '-')



generic_token_t *tokenize(char *inpbuffer, uint32_t length){
    generic_token_t *tokens = (generic_token_t *)malloc(sizeof(generic_token_t)*(length + 1));
    
    unsigned int i = 0;
    int t = 0;
    while(i < length){
        
        tokens[t].start_index = i;
        tokens[t].stop_index = i;
        tokens[t].length = 1;
        tokens[t].type = TERM_NULL;
        if(inpbuffer[i] == '?' || inpbuffer[i] == '*' || inpbuffer[i] == '+'){
            tokens[t].type = TERM_SPCL;
        } else if(inpbuffer[i] == '['){
            tokens[t].type = TERM_SQL;
        } else if(inpbuffer[i] == ']'){
            tokens[t].type = TERM_SQR;
        } else if(inpbuffer[i] == '('){
            tokens[t].type = TERM_CCL;
        } else if(inpbuffer[i] == ')'){
            tokens[t].type = TERM_CCR;
        } else if(inpbuffer[i] == '|'){
            tokens[t].type = TERM_ORR;
        } else if( ALPHA(inpbuffer[i]) ){
            tokens[t].type = TERM_STRING;
            while(ALPHA(inpbuffer[i + 1])){
                i += 1;
            }
            tokens[t].stop_index = i;
            tokens[t].length = tokens[t].stop_index - tokens[t].start_index + 1;
        } else{
            printf("ERROR: invalid character in input stream: %c (%u)\n",inpbuffer[i], (uint8_t)inpbuffer[i]);
            free(tokens);
            return NULL;
        }

        i += 1;
        t += 1;
    }

    tokens[t].type = TERM_EOF;
    tokens[t].length = 0;
    tokens[t].start_index = length;
    tokens[t].stop_index = length;

    return tokens;
}



entry_t get_pt_entry(uint32_t state, enum all_terms term){
    if(term == TERM_NULL){
        printf("Bad index to parse table\n");
        exit(1);
    }
    return parse_table_new[state*parse_table_width + term];

    int col = (term == TERM_EOF) ? 0 : term;
    return parse_table[state*(n_non_terms + n_terms) + col];


}
















typedef struct token_stack_t{
    generic_token_t *base_p;
    generic_token_t *next_p;
    int n_tokens;
    int alloc_size;

} token_stack_t;

token_stack_t init_token_stack(){
    token_stack_t ans;
    ans.base_p = (generic_token_t *)calloc(sizeof(generic_token_t), 16);
    ans.alloc_size = 16;
    ans.n_tokens = 0;
    ans.next_p = ans.base_p;
    return ans;
}

token_stack_t push_token_stack(token_stack_t stack, generic_token_t newone){
    if(stack.alloc_size < stack.n_tokens + 1){
        stack.alloc_size = stack.alloc_size*2;
        stack.base_p = (generic_token_t *)realloc(stack.base_p, sizeof(generic_token_t)*stack.alloc_size);
    }
    stack.n_tokens += 1;
    *(stack.next_p) = newone;
    stack.next_p += 1;
    return stack;
}

token_stack_t pop_token_stack(token_stack_t stack){
    if(stack.n_tokens <= 0){
        printf("POPING FROM A EMPTY STACK!\n");
        exit(1);
    }
    stack.n_tokens -= 1;
    stack.next_p = stack.next_p - 1;
    return stack;
}

generic_token_t top_token(token_stack_t stack){
    if(stack.n_tokens == 0){
        printf("Bad top token access!\n");
        exit(1);
    }
    return *(stack.next_p - 1);
}







typedef struct state_stack_t{
    uint32_t *base_p;
    uint32_t *top_p;
    int n_elems;
    int alloc_size;
} state_stack_t;

state_stack_t init_state_stack(){
    state_stack_t ans;
    ans.base_p = (uint32_t *)calloc(sizeof(uint32_t), 16);
    ans.alloc_size = 16;
    ans.n_elems = 0;
    ans.top_p = ans.base_p;
    return ans;
}

state_stack_t push_state_stack(state_stack_t stack, uint32_t value){
    if(stack.alloc_size < stack.n_elems + 1){
        stack.alloc_size = stack.alloc_size*2;
        stack.base_p = (uint32_t *)realloc(stack.base_p, sizeof(uint32_t)*stack.alloc_size);
    }
    stack.n_elems += 1;
    *(stack.top_p) = value;
    stack.top_p += 1;
    return stack;
}
state_stack_t pop_state_stack(state_stack_t stack){
    if(stack.n_elems <= 0){
        printf("POPING FROM A EMPTY STACK!\n");
        exit(1);
    }
    stack.n_elems -= 1;
    stack.top_p = stack.top_p - 1;
    return stack;
}

uint32_t top_state(state_stack_t stack){
    if(stack.top_p == stack.base_p){
        printf("Bad state access\n");
        exit(1);
    }
    return *(stack.top_p - 1);
}


void pstacks(state_stack_t state_stack, token_stack_t token_stack, std::vector<parse_tree_node_t *> parse_tree_stack){
    
    if(state_stack.n_elems != token_stack.n_tokens || state_stack.n_elems != (int) parse_tree_stack.size()){
        printf("Mismatch in stack sizes!\n");
        exit(1);
    }
    printf("State stack, Token stack\n");
    for(int i =0; i < state_stack.n_elems; i += 1){
        printf("|  %5d |", *(state_stack.top_p - 1 - i) );
        printf("|  %5d |", (token_stack.next_p - i - 1)->type);
        printf("|  %5d (%d-%d) |", (parse_tree_stack[parse_tree_stack.size() - 1 - i])->self_type.type, (parse_tree_stack[parse_tree_stack.size() - 1 - i])->self_type.start_index, \
        (parse_tree_stack[parse_tree_stack.size() - 1 - i])->self_type.stop_index);
        printf("\n");
    }
}

void push_regexes(parse_tree_node_t *ast_S, parse_tree_node_t *parse_tree_Regex);


parse_tree_node_t *get_grp_node(parse_tree_node_t *parse_tree_grp){
    passert(parse_tree_grp->self_type.type == TERM_GRP, "Expected grp type node\n");
    // grp will be either string, chrset, or regex
    parse_tree_node_t *newone = new parse_tree_node_t(parse_tree_grp->self_type);

    if(parse_tree_grp->childs.back()->self_type.type == TERM_CCL){
        newone->self_type.type = TERM_START;
        newone->self_type.start_index = newone->self_type.start_index + 1; // Skip the brackets
        newone->self_type.stop_index = newone->self_type.stop_index - 1; // Skip the brackets
        push_regexes(newone, parse_tree_grp->childs[1]);    // Add the whole regex tree.

    } else if(parse_tree_grp->childs.back()->self_type.type == TERM_SQL){
        newone->self_type.type = TERM_CHRSET;
        newone->self_type.start_index = newone->self_type.start_index + 1; // Skip the brackets
        newone->self_type.stop_index = newone->self_type.stop_index - 1; // Skip the brackets
    } else{
        passert(parse_tree_grp->childs.size() == 1 && parse_tree_grp->childs.front()->self_type.type == TERM_STRING, "Expected childs of group to be string or chrset or regex\n");
        newone->self_type.type = TERM_STRING;
    }
    return newone;

}


void add_expr(parse_tree_node_t *ast_Regex, parse_tree_node_t *parse_tree_expr){

    passert(parse_tree_expr->self_type.type == TERM_EXPR, "Expected expr node\n");

    if(parse_tree_expr->childs.front()->self_type.type == TERM_EXPR){
        add_expr(ast_Regex, parse_tree_expr->childs.back()); // left to right...
        add_expr(ast_Regex, parse_tree_expr->childs.front());
    } else{

        // either grp, or grp spl
        parse_tree_node_t *newone = new parse_tree_node_t(parse_tree_expr->self_type);
        newone->parent = ast_Regex;
        ast_Regex->childs.push_back(newone);

        //      Add the group
        parse_tree_node_t *grp_node = get_grp_node(parse_tree_expr->childs.back());
        passert(grp_node->self_type.type == TERM_STRING || grp_node->self_type.type == TERM_START || grp_node->self_type.type == TERM_CHRSET, "Bad group retured\n");
        grp_node->parent = newone;
        newone->childs.push_back(grp_node);

        //      add spcl
        if(parse_tree_expr->childs.size() == 2){
            parse_tree_node_t * newspcl = new parse_tree_node_t(parse_tree_expr->childs.front()->self_type);
            newspcl->parent = newone;
            newone->childs.push_back(newspcl);
        }


    }


}

void push_regexes(parse_tree_node_t *ast_S, parse_tree_node_t *parse_tree_Regex){
    passert(parse_tree_Regex->self_type.type == TERM_REGEX, "Expected regex node\n");

    parse_tree_node_t *newone;
    parse_tree_node_t *subexpr;
    while(parse_tree_Regex->childs.size() == 3){    // This means childs of Regex are .front. [Regex ORR Expr] .back.
        newone = new parse_tree_node_t(parse_tree_Regex->self_type);
        newone->parent = ast_S;
        ast_S->childs.push_back(newone);
        
        subexpr = parse_tree_Regex->childs.back();
        add_expr(newone, subexpr);
        newone->self_type.start_index = newone->childs.front()->self_type.start_index; // leftmost start indx
        newone->self_type.stop_index = newone->childs.back()->self_type.stop_index; // rightmost end indx

        parse_tree_Regex = parse_tree_Regex->childs.front();
    }

    newone = new parse_tree_node_t(parse_tree_Regex->self_type);
    newone->parent = ast_S;
    ast_S->childs.push_back(newone);

    subexpr = parse_tree_Regex->childs.back(); // back should == front
    add_expr(newone, subexpr);
    newone->self_type.start_index = newone->childs.front()->self_type.start_index; // leftmost start indx
    newone->self_type.stop_index = newone->childs.back()->self_type.stop_index; // rightmost end indx

    // ORRED with nothing
    if(parse_tree_Regex->childs.size() == 2){
        newone->self_type.stop_index -= 1; // Take out the <|> character
        newone = new parse_tree_node_t(parse_tree_Regex->self_type);
        
        newone->self_type.stop_index = -1;
        newone->self_type.start_index = -1; // Represents empty regex
        newone->parent = ast_S;
        ast_S->childs.push_back(newone);
    }

}

    


parse_tree_node_t * traverse_parse_tree(std::vector<parse_tree_node_t *> parse_tree_stack){
    // Expected = 2
    #ifdef PRINT_DEBUG
    printf("Size of parse tree stack: %ld\n", parse_tree_stack.size());
    #endif
    parse_tree_node_t *start_node = parse_tree_stack.back();

    parse_tree_node_t *ast_head = new parse_tree_node_t(start_node->self_type);
    ast_head->self_type.type = TERM_START;
    
    passert( (start_node->childs.size() == 1 ) && (start_node->childs.back()->self_type.type == TERM_REGEX), "Bad parse tree given to extract AST\n");
    push_regexes(ast_head, start_node->childs.back());


    return ast_head;
};


void  reduce_stacks(token_stack_t *token_stackp, state_stack_t *state_stackp, int rule, std::vector<parse_tree_node_t *> *parse_tree_stackp){

    token_stack_t token_stack = *token_stackp;
    state_stack_t state_stack = *state_stackp;
    std::vector<parse_tree_node_t *> parse_tree_stack = *parse_tree_stackp;

    rule_t desired_rule = rules[rule];

    if(token_stack.n_tokens < desired_rule.n_srcs){
        printf("Could not reduce rule do to not enought arguments\n");
        exit(1);
    }
    for(int i = 0; i < desired_rule.n_srcs; i += 1){
        if( desired_rule.src[i] != (*(token_stack.next_p - desired_rule.n_srcs + i)).type ){
            printf("Could not reduce rule due to bad stack %d for rule %d\n", i, rule);
            printf("Desired item = %d\n", desired_rule.src[i]);
            printf("Actual item item = %d\n", (*(token_stack.next_p - desired_rule.n_srcs + i)).type);
            exit(1);
        }
    }

    for(int i =0; i < desired_rule.n_srcs; i += 1){
        token_stack = pop_token_stack(token_stack);
        state_stack = pop_state_stack(state_stack);
    }
    generic_token_t newone = {TERM_NULL, 0, 0, 0};
    newone.type = desired_rule.dest;
    newone.start_index = -1;
    newone.stop_index = -1;

    parse_tree_node_t *new_node = new parse_tree_node_t(newone);
    parse_tree_node_t *childnode;
    for(int i =0; i < desired_rule.n_srcs; i += 1){
        childnode = parse_tree_stack.back();
        parse_tree_stack.pop_back();
        new_node->childs.push_back(childnode);
        childnode->parent = new_node;
        
        // always take childs bounds if you havent already, else take the max/min
        new_node->self_type.start_index = (new_node->self_type.start_index < childnode->self_type.start_index && new_node->self_type.start_index != -1) \
                                        ? (new_node->self_type.start_index) : (childnode->self_type.start_index);
        new_node->self_type.stop_index = (new_node->self_type.stop_index > childnode->self_type.stop_index && new_node->self_type.stop_index != -1) \
                                        ? (new_node->self_type.stop_index) : (childnode->self_type.stop_index);
    }

    parse_tree_stack.push_back(new_node);
    token_stack = push_token_stack(token_stack, newone);


    *token_stackp = token_stack;
    *state_stackp = state_stack;
    *parse_tree_stackp = parse_tree_stack;

}



parse_tree_node_t * traverse_graph(generic_token_t *input_stream){
    token_stack_t token_stack = init_token_stack();
    state_stack_t state_stack = init_state_stack();

    std::vector<parse_tree_node_t *> parse_tree_stack;
    parse_tree_node_t *newelem = new parse_tree_node_t((generic_token_t){TERM_NULL, 0, 0, 0});
    parse_tree_stack.push_back(newelem);

    state_stack = push_state_stack(state_stack, 0);
    token_stack = push_token_stack(token_stack, (generic_token_t){TERM_NULL, 0, 0, 0});

    int i = 0;
    generic_token_t next_input;

    entry_t action = {0};      
    while(1){
        next_input = input_stream[i];

        #ifdef PRINT_DEBUG
        printf("Input token = %d (str : %d-%d)\n", next_input.type, next_input.start_index, next_input.stop_index);
        pstacks(state_stack, token_stack, parse_tree_stack);
        #endif
        action = get_pt_entry(top_state(state_stack), next_input.type);



        if(!action.isvalid){
            printf("Could not traverse the graph for this regex!\n");
            exit(1);
        } else if(action.isReduce){
            #ifdef PRINT_DEBUG
            printf("Reducing by rule %d!\n", action.dest);
            #endif
            reduce_stacks(&token_stack, &state_stack, action.dest, &parse_tree_stack);
            // Finish conditon
            if(top_token(token_stack).type == TERM_START && input_stream[i].type == TERM_EOF){
                //printf("Done! token_stack top = %d, == %d", top_token(token_stack).type, TERM_START);
                break;
            }
            uint32_t newstate = get_pt_entry(top_state(state_stack), top_token(token_stack).type).dest;

            state_stack = push_state_stack(state_stack, newstate);
            #ifdef PRINT_DEBUG
            printf("    Going to state %d\n", newstate);
            #endif
            
        } else{
            #ifdef PRINT_DEBUG
            printf("Shifting!\n");
            #endif
            parse_tree_node_t *newnode = new parse_tree_node_t(next_input);
            // Shift
            parse_tree_stack.push_back(newnode);
            token_stack = push_token_stack(token_stack, next_input);
            state_stack = push_state_stack(state_stack, action.dest);
            #ifdef PRINT_DEBUG
            printf("    Going to state %d\n", action.dest);
            #endif
            i += 1;
        }
    }
    parse_tree_node_t *firstpre = parse_tree_stack.back()->childs.back()->childs.back();
    //printf("Printing 1st pre-or head: %d-%d, type = %d\n", \
    firstpre->self_type.start_index, firstpre->self_type.stop_index, firstpre->self_type.type);
    #ifdef PRINT_DEBUG
    printf("Successfully traversed the graph!\n");
    #endif
    //pstacks(state_stack, token_stack, parse_tree_stack);
    parse_tree_node_t *ast = traverse_parse_tree(parse_tree_stack);
    #ifdef PRINT_DEBUG
    printf("AST has %ld childs\n", ast->childs.size());
    #endif

    return ast;
}






nfa_t *make_nfa_string(parse_tree_node_t *node_string, char *src){
    passert(node_string->self_type.type == TERM_STRING, "Expected a string node\n");

    nfa_t *network = new nfa_t();
    
    nfa_node_t *head = new nfa_node_t();
    nfa_node_t *curr = head;
    nfa_node_t *next;
    
    for(int i = node_string->self_type.start_index; i <= node_string->self_type.stop_index; i += 1){
        next = new nfa_node_t();
        curr->arrows.push_back(next);
        curr->input.push_back(src[i]);
        curr = next;
    }
    curr->isfinish = true;
    network->head = head;
    network->ends.push_back(curr);
    return network;
}

nfa_t *make_nfa_chrset(parse_tree_node_t *node_chrset, char *src){
    passert(node_chrset->self_type.type == TERM_CHRSET, "Expected a string node\n");

    nfa_t *network = new nfa_t();
    
    nfa_node_t *head = new nfa_node_t();
    nfa_node_t *next = new nfa_node_t();
    next->isfinish = true;

    for(int i = node_chrset->self_type.start_index; i <= node_chrset->self_type.stop_index; i += 1){
        if(src[i] == '-'){
            passert(i > node_chrset->self_type.start_index, "Bad Character set at ...");
            passert(i < node_chrset->self_type.stop_index, "Bad Character set at ...");
            unsigned char high;
            unsigned char low;
            if( ( (unsigned char *)src )[i + 1] <  ( (unsigned char *)src )[i - 1] ){
                high = ( (unsigned char *)src )[i - 1];
                low = ( (unsigned char *)src )[i + 1];
            } else{
                low = ( (unsigned char *)src )[i - 1];
                high = ( (unsigned char *)src )[i + 1];
            }
            for(unsigned char a = low + 1; a <= high - 1; a += 1){
                head->arrows.push_back(next);
                head->input.push_back( *((char *)(&a)) );
            }
        } else{
            head->arrows.push_back(next);
            head->input.push_back(src[i]);
        }
    }

    //printf("num outputs : %d\n", head->arrows[0]->arrows.size());
    network->head = head;
    network->ends.push_back(next);
    return network;
}


nfa_t *apply_spcl(nfa_t *network, char opp){
    if(opp == '?'){
        network->head->isfinish = true;
        network->ends.push_back(network->head);
    } else if(opp == '*'){
        for(unsigned int i = 0; i < network->ends.size(); i += 1){
            network->ends[i]->arrows.push_back(network->head);  // Go from end to start
            network->ends[i]->input.push_back(0);               // epsilon transition
        }
        network->head->isfinish = true;
        network->ends.push_back(network->head);
    } else if(opp == '+'){
        for(unsigned int i = 0; i < network->ends.size(); i += 1){
            network->ends[i]->arrows.push_back(network->head);  // Go from end to start
            network->ends[i]->input.push_back(0);               // epsilon transition
        }
    } else{
        printf("Error bad operation on network!\n");
        exit(1);
    }
    return network;
}

nfa_t *concat_nwks(nfa_t * network_left, nfa_t * network_right){
    for(unsigned int i = 0; i < network_left->ends.size(); i+= 1){
        network_left->ends[i]->isfinish = false;                        // Lefts ends are not fins
        network_left->ends[i]->arrows.push_back(network_right->head);   // Add arrow to start of right
        network_left->ends[i]->input.push_back(0);                      // epsilon tansition
    }
    
    network_right->head = network_left->head;

    delete network_left; // Dont care about this holder anymore
    return network_right;
}

nfa_t *orr_nwks(nfa_t * network_a, nfa_t * network_b){
    nfa_node_t * newnode = new nfa_node_t();
    newnode->arrows.push_back(network_a->head);
    newnode->input.push_back(0);
    newnode->arrows.push_back(network_b->head);
    newnode->input.push_back(0);

    network_a->head = newnode;
    for(unsigned int i = 0; i < network_b->ends.size(); i+= 1){ // Add all ends of B also
        network_a->ends.push_back(network_b->ends[i]);
    }

    delete network_b;
    return network_a;
}

nfa_t *blank_nfa(){
    nfa_t * ans = new nfa_t();
    nfa_node_t *node = new nfa_node_t();
    node->isfinish = true;
    ans->head = node;
    ans->ends.push_back(node);
    return ans;
}


void print_node(nfa_node_t *node){
    printf("Node has %d arrows\n", node->arrows.size());
    printf("    isfinish = %d\n", node->isfinish);
    for(int i=0; i <node->arrows.size(); i += 1){
        printf("    Input <%c>/%d\n", node->input[i], node->input[i]);
    }
}
void print_dfa(dfa_node_t *node){
    printf("Node has %d arrows\n", node->arrows.size());
    printf("    isfinish = %d\n", node->isfinish);
    for(int i=0; i <node->arrows.size(); i += 1){
        printf("    Input <%c>/%d\n", node->input[i], node->input[i]);
    }
}


nfa_t *make_nfa(parse_tree_node_t *ast, char *src){
    nfa_t *ans;
    if(ast->self_type.type == TERM_START){  // Entries are ORRed
        passert(ast->childs.size() >= 1, "Expected AST start to have children\n");
        ans = make_nfa(ast->childs[0], src);
        for(unsigned int i = 1; i < ast->childs.size(); i+= 1){
            if(ast->childs[i]->self_type.start_index == -1 && ast->childs[i]->self_type.stop_index == -1){
                // Case for: regex is empty
                passert(ast->childs[i]->childs.size() == 0, "Expected empty regext to have no children\n");
                ans = orr_nwks(ans, blank_nfa());
            } else{
                ans = orr_nwks(ans, make_nfa(ast->childs[i], src));
            }
        }
        return ans;
    } else if(ast->self_type.type == TERM_REGEX){ // Entries are concated
        passert(ast->childs.size() >= 1, "Expected AST Regex to have children\n");
        ans = make_nfa(ast->childs[0], src);
        for(unsigned int i = 1; i < ast->childs.size(); i+= 1){
            ans = concat_nwks(ans, make_nfa(ast->childs[i], src));
        }
        return ans;
    } else if(ast->self_type.type == TERM_EXPR){ // Entries (might) be oped on
        ans = make_nfa(ast->childs[0], src);
        if(ast->childs.size() == 2){
            ans = apply_spcl(ans, src[ast->childs[1]->self_type.start_index]);
        }
        return ans;
    } else if(ast->self_type.type == TERM_STRING){
        return make_nfa_string(ast, src);
    } else if(ast->self_type.type == TERM_CHRSET){
        return make_nfa_chrset(ast, src);
    } 
    printf("ERROR: unkown ast node type\n");
    exit(1);
}






















struct set2p_entry{
    std::vector<nfa_node_t *> set;
    dfa_node_t *equivl;
};

// Find epsilon closure of nfa node
std::vector<nfa_node_t *> epsil(nfa_node_t *head, std::vector<nfa_node_t *> already_added){

    for(unsigned int i = 0; i < already_added.size(); i += 1){
        if(already_added[i] == head){
            return already_added;
        }
    }

    already_added.push_back(head);

    for(unsigned int i = 0; i < head->input.size(); i += 1){
        if(head->input[i] == 0){ // epsilon
            nfa_node_t *next = head->arrows[i];
            already_added = epsil(next, already_added);
        }
    }
    return already_added;
}
std::vector<nfa_node_t *> epsilon_closure(nfa_node_t *head){
    return epsil(head, {});
}

bool cmp(nfa_node_t *a, nfa_node_t *b){
    return a < b;
}

std::vector<set2p_entry> add_to_set_map(std::vector<set2p_entry> set_map, std::vector<nfa_node_t *> nfa_set, dfa_node_t *new_node){
    std::sort(nfa_set.begin(), nfa_set.end(), cmp); // Sort the pointer
    set2p_entry entry;
    entry.set = nfa_set;
    entry.equivl = new_node;
    set_map.push_back(entry);
    return set_map;
    /*set2p_entry entry;
    for(int i = 0; i < set_map.size(); i += 1){
        entry = set_map[i];
        // compare entry to nfa_set
        if(entry == nfa_set){

        }
    }*/ 
}
dfa_node_t *get_dfa_node(std::vector<set2p_entry> set_map, std::vector<nfa_node_t *> nfa_set){
    std::sort(nfa_set.begin(), nfa_set.end(), cmp); // Sort the pointer
    set2p_entry entry;
    for(unsigned int i = 0; i < set_map.size(); i += 1){
        entry = set_map[i];
        // compare entry to nfa_set
        if(entry.set == nfa_set){
            return entry.equivl;
        }
    }
    return NULL;
}



/*
    - Given a DFA node. -> No arrows and No Input.
    - Given the set of NFAs that DFA represents.

    - Make new nodes, OR, attach to existing nodes all children of the DFA.
*/ 
void connect_all_nodes(dfa_node_t *dfa_node, std::vector<nfa_node_t *> nfa_set, std::vector<set2p_entry> set_map){
    passert(get_dfa_node(set_map, nfa_set) == dfa_node, "Error NFA set and DFA do not corrolate\n");

    // 1) Find all inputs where you can leave the set with.
    //          (only add once to dfa_node.input<vec>)
    //          ( BONUS: Keep track if any of the nfas in the set are a finish node...)
    for(unsigned int i = 0; i < nfa_set.size(); i += 1){
        for(unsigned int j = 0; j < nfa_set[i]->input.size(); j += 1){   // For each outgoing (arrow,input) from the set
            char inp = nfa_set[i]->input[j];
            if( inp == 0){continue;}                            // Skip epsilons...
            if(std::count(dfa_node->input.begin(), dfa_node->input.end(), inp) == 0){
                dfa_node->input.push_back(inp);
            }
        }
        if(nfa_set[i]->isfinish == true){
            dfa_node->isfinish = true;
        }
    }


    // 2) Find the desination sets for all possible inputs
    for(unsigned int s = 0; s < dfa_node->input.size(); s += 1){
        char inp = dfa_node->input[s];          // Input
        std::vector<nfa_node_t *> nfa_set_new;  // Desination set for input 'Input'

        for(unsigned int i = 0; i < nfa_set.size(); i += 1){
            for(unsigned int j = 0; j < nfa_set[i]->input.size(); j += 1){
                if(nfa_set[i]->input[j] != inp){continue;}          // For each possile transition out of the set

                nfa_node_t *other_node = nfa_set[i]->arrows[j];     // Add E(dest node)
                std::vector<nfa_node_t *> toadd = epsilon_closure(other_node);
                nfa_set_new.insert(nfa_set_new.end(), toadd.begin(), toadd.end());  // Concat to running list

            }
        }
        // Given input 'inp' you will tansition to the set 'nfa_set_new'
        //      Does this set exist as a DFA?
        
        dfa_node_t *next_node = get_dfa_node(set_map, nfa_set_new);
        if(next_node == NULL){  // This set of nfas is not yet a DFA
            next_node = new dfa_node_t();
            set_map = add_to_set_map(set_map, nfa_set_new, next_node);
            connect_all_nodes(next_node, nfa_set_new, set_map);
        }
        dfa_node->arrows.push_back(next_node);
    }

}


dfa_t *nfa_to_dfa_conv(nfa_t *nfa){

    std::vector<set2p_entry> set_map;
    dfa_node_t *dfa_head = new dfa_node_t();

    std::vector<nfa_node_t *> start_node_nfas = epsilon_closure(nfa->head);
    #ifdef PRINT_DEBUG
    printf("Number of NFA nodes in the DFA start: %ld\n", start_node_nfas.size());
    printf("Number of NFA set - to - DFA mappings %ld\n", set_map.size() );
    #endif
    set_map = add_to_set_map(set_map, start_node_nfas, dfa_head);
    #ifdef PRINT_DEBUG
    printf("Number of NFA set - to - DFA mappings %ld\n", set_map.size() );
    #endif
    connect_all_nodes(dfa_head, start_node_nfas, set_map);

    dfa_t *ans = new dfa_t();
    ans->head = dfa_head;
    return ans;
}




bool traverse_dfa(dfa_t *dfa, char *input){
    
    dfa_node_t *head = dfa->head;
    int i = 0;
    while(input[i] != 0){
        //printf("At node. Input is %c\n", input[i]);
        // Check if there is a outgoing arrow with this input
        int found_route = 0;
        for(int j = 0; j < head->input.size(); j += 1){
            if(input[i] == head->input[j]){
                //printf("Found route!\n");
                // Go to next state
                head = head->arrows[j];
                found_route = 1;
                break;
            }
        }
        if(found_route == 0){
            //printf("Could not find route!\n");
            return false;
        }
        i += 1;
    }
    //printf("Done!\n");
    return head->isfinish;
}






int make_regex(char *str, regex_t *dest){
    // 1) Tokenize
    generic_token_t * tokens = tokenize(str, strlen(str));
    if(tokens == NULL){return -1;}
    make_parse_table();

    // 2) Make parse tree and AST
    parse_tree_node_t * ast = traverse_graph(tokens);
    if(ast == NULL){
        return -1;
    }
    // 3) Make the NFA
    #ifdef PRINT_DEBUG
    printf("Starting NFA creation\n");
    #endif
    nfa_t *nfa = make_nfa(ast, str);
    if(nfa == NULL){return -1;}
    #ifdef PRINT_DEBUG
    printf("Starting DFA conversion\n");
    #endif
    dfa_t *dfa = nfa_to_dfa_conv(nfa);
    if(dfa == NULL){return -1;}
    regex_t newone = {0};
    newone.dfa = dfa;
    *dest = newone;
    return 0;
}

bool match(regex_t regex, char *matchstr){
    return traverse_dfa(regex.dfa, matchstr);
}
/*
regex_t::regex_t(char *str){
    generic_token_t * tokens = tokenize(str, strlen(str));
    if(tokens == NULL){
        return;
    }
    make_parse_table();
    parse_tree_node_t * ast = traverse_graph(tokens);
    nfa_t *nfa = make_nfa(ast, str);
    dfa_t *dfa_new = nfa_to_dfa_conv(nfa);
    dfa = dfa_new;
}

bool regex_t::match(char *matchstr){
    traverse_dfa(dfa, matchstr);
}*/