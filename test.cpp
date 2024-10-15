#include "regex.h"
#include <string.h>

class tester_t{
    int testnum;
    regex_t curr_regex;
public:

    void update_regex(char *regex_str){
        int ret_val = make_regex(regex_str, &curr_regex);
        printf("Testing new regex: %s...\n", regex_str);
        if(ret_val == -1){printf("Bad regex!\n"); exit(1);}
    }
    void test(char *instr, bool expect_match){
    if(match(curr_regex, (char *) instr) != expect_match){
        printf("Test %d FAILED on input %s\n", testnum, instr);
    } else{
        printf("Test %d passed\n", testnum);

    }
    testnum += 1;
    }

    tester_t(){testnum = 1; curr_regex = {0};}
    ~tester_t(){};
};


int main(int argv, char **argc){

    char *str_regex;
    char *str_input;
    bool matched;

    printf("Starting all tests...\n");

    tester_t tester = tester_t();

    str_regex = "this*that";
    tester.update_regex(str_regex);
    tester.test("thithat", false);
    tester.test("that", true);
    tester.test("this*", false);
    tester.test("thisthisthat", true);

    str_regex = "hithere(like)?";
    tester.update_regex(str_regex);
    tester.test("nothis", false);
    tester.test("withs", false);
    tester.test("++343dd", false);
    tester.test("hithere", true);
    tester.test("hitherelike", true);
    tester.test("hitherelikelike", false);

    str_regex = "(quit+stop*night?|a)";
    tester.update_regex(str_regex);
    tester.test("quitquit", true);
    tester.test("quitstopstopnight", true);
    tester.test("quitstopstopnightnight", false);
    tester.test("stopstopnightnight", false);
    tester.test("a", true);


    printf("Done all tests....\n");

}