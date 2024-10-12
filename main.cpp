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

int main(int argv, char **argc){


    printf("Running test strings...\n");
    char *teststr = (char *)"regex(3)is(not)?co9ol?-";
    generic_token_t * tokens = tokenize(teststr, strlen(teststr));

    make_parse_table();
    if(tokens == NULL){
        printf("Bad regex string\n");
        return 1;
    }
    ptokens(teststr, tokens);
    traverse_graph(tokens);


}