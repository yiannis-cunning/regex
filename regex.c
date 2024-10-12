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


static int n_terms = 8;
static int n_non_terms = 3;
static int n_states = 16;

typedef struct entry_t{
    bool isvalid;
    bool isReduce;
    uint32_t dest;
} entry_t;

static entry_t *parse_table;


char *names_terms[] = {"EOF", "STRING", "SPCL", "SQR", "SQL", "CCL", "CCR", "ORR"};
char *names_non_terms[] = {"REGEX", "EXPR", "GRP"};


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




static rule_t r1 = {TERM_START, {TERM_REGEX, TERM_NULL, TERM_NULL}, 1};
static rule_t r2 = {TERM_REGEX, {TERM_EXPR, TERM_ORR, TERM_REGEX}, 3};
static rule_t r3 = {TERM_REGEX, {TERM_EXPR, TERM_NULL, TERM_NULL}, 1};

static rule_t r4 = {TERM_EXPR, {TERM_GRP, TERM_SPCL, TERM_NULL}, 2};
static rule_t r5 = {TERM_EXPR, {TERM_GRP, TERM_NULL, TERM_NULL}, 1};
static rule_t r6 = {TERM_EXPR, {TERM_EXPR, TERM_EXPR, TERM_NULL}, 2};

static rule_t r7 = {TERM_GRP, {TERM_CCL, TERM_REGEX, TERM_CCR}, 3};
static rule_t r8 = {TERM_GRP, {TERM_SQL, TERM_STRING, TERM_SQR}, 3};
static rule_t r9 = {TERM_GRP, {TERM_STRING, TERM_NULL, TERM_NULL}, 1};

static rule_t rules[9];



//static uint32_t parse_table[n_non_terms + n_terms][n_states];
// Entry is either shift and go to other state, or reduce by rule x
//      make MSB = shift/reduce and lower bits are either 
#define SET_PARSE_ENTRY_TERMINAL(terminal, isreduce, value) parse_table[statenum*(n_terms + n_non_terms) + terminal] = (entry_t) {1, isreduce, value};
#define SET_PARSE_ENTRY_NON_TERMINAL(nonterminal, isreduce, value) parse_table[statenum*(n_terms + n_non_terms) + nonterminal + n_terms] = (entry_t) {1, isreduce, value};

#define SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(value) for(int i = 0; i < n_terms; i += 1){parse_table[statenum*(n_terms + n_non_terms) + i] = (entry_t) {1, 1, value};}

void make_parse_table(){
    parse_table = calloc(sizeof(entry_t), n_states*(n_terms + n_non_terms));


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
    SET_ALL_PASRSE_ENTRY_TERMINALS_REDUCE(1);

    // State 3
    statenum = 3;
    SET_PARSE_ENTRY_TERMINAL( STRING_T, 0, 7)
    SET_PARSE_ENTRY_TERMINAL( SQL, 0, 11)
    SET_PARSE_ENTRY_TERMINAL( CCL, 0, 10)

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

    pparse_table();
}


// Want to make the parse tree by traversing the graph


#define ALPHA(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x >= '0' && x <= '9') || x == '-')


token_t *tokenize(char *inpbuffer, uint32_t length){
    token_t *tokens = malloc(sizeof(token_t)*(length + 1));
    
    int i = 0;
    int t = 0;
    while(i < length){
        
        tokens[t].start_index = i;
        tokens[t].stop_index = i;
        tokens[t].length = 1;
        tokens[t].type = NULL_TYPE;
        if(inpbuffer[i] == '?' || inpbuffer[i] == '*' || inpbuffer[i] == '+'){
            tokens[t].type = SPCL;
        } else if(inpbuffer[i] == '['){
            tokens[t].type = SQL;
        } else if(inpbuffer[i] == ']'){
            tokens[t].type = SQR;
        } else if(inpbuffer[i] == '('){
            tokens[t].type = CCL;
        } else if(inpbuffer[i] == ')'){
            tokens[t].type = CCR;
        } else if(inpbuffer[i] == '|'){
            tokens[t].type = ORR;
        } else if( ALPHA(inpbuffer[i]) ){
            tokens[t].type = STRING_T;
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
    return tokens;
}

generic_token_t *tokenize2(char *inpbuffer, uint32_t length){
    generic_token_t *tokens = malloc(sizeof(token_t)*(length + 1));
    
    int i = 0;
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
    tokens[t].start_index = 0;
    tokens[t].stop_index = 0;

    return tokens;
}



entry_t get_pt_entry(uint32_t state, enum all_terms term){
    if(term == TERM_NULL){
        printf("Bad index to parse table\n");
        exit(1);
    }
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
    ans.base_p = calloc(sizeof(generic_token_t), 16);
    ans.alloc_size = 16;
    ans.n_tokens = 0;
    ans.next_p = ans.base_p;
    return ans;
}

token_stack_t push_token_stack(token_stack_t stack, generic_token_t newone){
    if(stack.alloc_size < stack.n_tokens + 1){
        stack.alloc_size = stack.alloc_size*2;
        stack.base_p = realloc(stack.base_p, sizeof(generic_token_t)*stack.alloc_size);
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
    ans.base_p = calloc(sizeof(uint32_t), 16);
    ans.alloc_size = 16;
    ans.n_elems = 0;
    ans.top_p = ans.base_p;
    return ans;
}

state_stack_t push_state_stack(state_stack_t stack, uint32_t value){
    if(stack.alloc_size < stack.n_elems + 1){
        stack.alloc_size = stack.alloc_size*2;
        stack.base_p = realloc(stack.base_p, sizeof(uint32_t)*stack.alloc_size);
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


void pstacks(state_stack_t state_stack, token_stack_t token_stack){
    if(state_stack.n_elems != token_stack.n_tokens){
        printf("Mismatch in stack sizes!\n");
        exit(1);
    }
    printf("State stack, Token stack\n");
    for(int i =0; i < state_stack.n_elems; i += 1){
        printf("|  %5d |", *(state_stack.top_p - 1 - i) );
        printf("|  %5d |", (token_stack.next_p - i - 1)->type);
        printf("\n");
    }
}


enum non_terms reduce_stacks(token_stack_t *token_stackp, state_stack_t *state_stackp, int rule){

    token_stack_t token_stack = *token_stackp;
    state_stack_t state_stack = *state_stackp;

    rule_t desired_rule = rules[rule];

    if(token_stack.n_tokens < desired_rule.n_srcs){
        printf("Could not reduce rule do to not enought arguments\n");
        exit(1);
    }
    for(int i = 0; i < desired_rule.n_srcs; i += 1){
        if( desired_rule.src[i] != (*(token_stack.next_p - desired_rule.n_srcs + i)).type ){
            printf("Could not reduce rule due to bad stack\n");
            exit(1);
        }
    }

    for(int i =0; i < desired_rule.n_srcs; i += 1){
        token_stack = pop_token_stack(token_stack);
        state_stack = pop_state_stack(state_stack);
    }
    generic_token_t newone = {0};
    newone.type = desired_rule.dest;
    token_stack = push_token_stack(token_stack, newone);


    *token_stackp = token_stack;
    *state_stackp = state_stack;

    switch (desired_rule.dest){
        case TERM_REGEX:
            return REGEX;
        case TERM_EXPR:
            return EXPR;
        case TERM_GRP:
            return GRP;
        case TERM_START:
            return START;
        default:
            printf("BAD rule!\n");
            exit(1);
    }

}



generic_token_t token2generic(token_t t){
    generic_token_t ans = {0};
    switch (t.type){
    case NULL_TYPE:
        ans.type = TERM_EOF;
        break;
    case STRING_T:
        ans.type = TERM_STRING;
        break;
    case SPCL:
        ans.type = TERM_SPCL;
        break;
    case SQL:
        ans.type = TERM_SQL;
        break;
    case SQR:
        ans.type = TERM_SQR;
        break;
    case CCL:
        ans.type = TERM_CCL;
        break;
    case CCR:
        ans.type = TERM_CCR;
        break;
    case ORR:
        ans.type = TERM_ORR;
        break;
    default:
        printf("BAD CONVERSION!\n");
        exit(1);
        break;
    }
    return ans;
}


void traverse_graph(token_t *input_stream, generic_token_t *inp2){
    token_stack_t token_stack = init_token_stack();
    state_stack_t state_stack = init_state_stack();

    state_stack = push_state_stack(state_stack, 0);
    token_stack = push_token_stack(token_stack, (generic_token_t){0});

    int i = 0;
    generic_token_t next_input;
    token_t next_input_old;

    entry_t action = {0};
    entry_t action2 = {0};        
    while(1){
        next_input = inp2[i]; //token2generic(input_stream[i]);
        next_input_old = input_stream[i];
        printf("Input token = %d\n", next_input.type);
        pstacks(state_stack, token_stack);
        //action2 = parse_table[top_state(state_stack)*(n_terms + n_non_terms) + next_input_old.type];

        action = get_pt_entry(top_state(state_stack), inp2[i].type);
        //if(action.dest != action2.dest || action.isReduce != action2.isReduce || action.isvalid != action2.isvalid){
        //    printf("Entries are differnt!\n");
        //    exit(1);
        //}


        if(!action.isvalid){
            printf("Could not traverse the graph for this regex!\n");
            exit(1);
        } else if(action.isReduce){
            printf("Reducing by rule %d!\n", action.dest);
            enum non_terms newtopstack = reduce_stacks(&token_stack, &state_stack, action.dest);
            if(newtopstack == START &&  inp2[i].type == TERM_EOF){//next_input_old.type == NULL_TYPE){
                break;
            }
            uint32_t otherstate = parse_table[top_state(state_stack)*(n_terms + n_non_terms) + n_terms + newtopstack].dest;


            uint32_t newstate = get_pt_entry(top_state(state_stack), top_token(token_stack).type).dest;
            if(otherstate != newstate){
                printf("BAD STATES\n");
                exit(1);
            }
            
            state_stack = push_state_stack(state_stack, newstate);
            printf("    Going to state %d\n", newstate);
            
        } else{
            printf("Shifting!\n");
            // Shift
            //printf("top token on stack: %d\n", (token_stack.next_p -1)->type);
            token_stack = push_token_stack(token_stack, next_input);
            //printf("top token on stack: %d\n", (token_stack.next_p -1)->type);
            state_stack = push_state_stack(state_stack, action.dest);
            printf("    Going to state %d\n", action.dest);
            i += 1;
        }
    }

    printf("Successfully traversed the graph!\n");
}