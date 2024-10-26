#ifndef REGEX_H
#define REGEX_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <string.h>


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


typedef struct miniregex_t{
    dfa_t *dfa;
} miniregex_t;

int make_regex(char *str, miniregex_t *dest);

bool match_regex(miniregex_t regex, char *str);

#endif