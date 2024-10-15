#include <string.h>
#include "regex.h"

void ptokens(char *buffer, generic_token_t *tokens){
    int t = 0;
    
    generic_token_t token = tokens[t];
    while(token.type != TERM_EOF){
        printf("Token %d of length %d: -", t, token.length);
        for(int i = token.start_index; i <= token.stop_index; i += 1){
            printf("%c", buffer[i]);
        }
        printf("-\n");
        t += 1;
        token = tokens[t];
    }
}

void print_node(parse_tree_node_t *node, char *src){
    for(int i = node->self_type.start_index; i <= node->self_type.stop_index; i += 1){
        printf("%c", src[i]);
    }
}

void print_ast(parse_tree_node_t *ast, char *src, int width){
    //printf("Printing ast head: %d-%d\n", ast->self_type.start_index, ast->self_type.stop_index);
    switch(ast->self_type.type){
        case TERM_START:
            printf("START (");
            print_node(ast, src);
            printf(")\n");
            for(unsigned int i = 0; i < ast->childs.size(); i += 1){
                printf("     ");
                print_node(ast->childs[i], src);
                if(i != ast->childs.size() - 1){
                    printf("     |");
                }
            }
            printf("\n");
            for(unsigned int i = 0; i < ast->childs.size(); i += 1){
                print_ast(ast->childs[i], src, width);
            }
            break;
        case TERM_REGEX:
            printf("REGEX (");
            print_node(ast, src);
            printf(")\n");
            for(unsigned int i = 0; i < ast->childs.size(); i += 1){
                printf("     ");
                print_node(ast->childs[i], src);
                if(i != ast->childs.size() - 1){
                    printf("     ,");
                }
            }
            printf("\n");
            for(unsigned int i = 0; i < ast->childs.size(); i += 1){
                print_ast(ast->childs[i], src, width);
            }
            break;
        case TERM_EXPR:
            printf("EXPR (");
            print_node(ast, src);
            printf(")\n");
            for(unsigned int i = 0; i < ast->childs.size(); i += 1){
                printf("     ");
                print_node(ast->childs[i], src);
                if(i != ast->childs.size() - 1){
                    printf("     OPP ");
                }
            }
            printf("\n");
            for(unsigned int i = 0; i < ast->childs.size(); i += 1){
                //printf("printing child %d\n", i);
                print_ast(ast->childs[i], src, width);
            }
            break;
        case TERM_STRING:
            printf("STRING (");
            print_node(ast, src);
            printf(")\n");
            break;
        case TERM_CHRSET:
            printf("CHRSET [");
            print_node(ast, src);
            printf("]\n");
            break;
        case TERM_SPCL:
            printf("SPCL (");
            print_node(ast, src);
            printf(")\n");
            break;  
        default:
            printf("Could not print!\n");
            break;
    }
    //print_node(ast, src);
    printf("\n");
}



int main(int argv, char **argc){


    printf("Running test strings...\n");
    char *teststr = (char *)"this*that"; // |targets+|(stab[iy]?|34)
    generic_token_t * tokens = tokenize(teststr, strlen(teststr));

    make_parse_table();
    if(tokens == NULL){
        printf("Bad regex string\n");
        return 1;
    }
    ptokens(teststr, tokens);
    parse_tree_node_t * ast = traverse_graph(tokens);
    nfa_t *nfa = make_nfa(ast, teststr);

    dfa_t *dfa = nfa_to_dfa_conv(nfa);


    print_ast(ast, teststr, 50);
    
    printf("Testing inputs ...\n");
    printf("\"%s\" = %d\n", "that", traverse_dfa(dfa, (char *)"that"));

}