#include "regex.h"
#include <string.h>

class tester_t{
    int testnum;
    int passednum;
    regex_t curr_regex;
public:

    void update_regex(const char *regex_str){
        int ret_val = make_regex( (char *) regex_str, &curr_regex);
        printf("Testing new regex: %s...\n", regex_str);
        if(ret_val == -1){printf("Bad regex!\n"); exit(1);}
    }
    void test(const char *instr, bool expect_match){
        if(match(curr_regex, (char *) instr) != expect_match){
            printf("Test %d FAILED on input %s\n", testnum, instr);
        } else{
            printf("Test %d passed\n", testnum);
            passednum += 1;
        }
        testnum += 1;
    }
    void done(){
        printf("Done all tests.... %d/%d passed\n", testnum - 1, passednum);
    }

    tester_t(){
        printf("Starting all tests...\n"); 
        testnum = 1; 
        curr_regex = {0};
        passednum = 0;
    }
    ~tester_t(){};
};


int main(int argv, char **argc){

    tester_t tester = tester_t();

    tester.update_regex("this*that");
    tester.test("thithat", false);
    tester.test("that", true);
    tester.test("this*", false);
    tester.test("thisthisthat", true);

    tester.update_regex("hithere(like|)");
    tester.test("nothis", false);
    tester.test("withs", false);
    tester.test("++343dd", false);
    tester.test("hithere", true);
    tester.test("hitherelike", true);
    tester.test("hitherelikelike", false);

    tester.update_regex("(quit+stop*night?|a)");
    tester.test("quitquit", true);
    tester.test("quitstopstopnight", true);
    tester.test("quitstopstopnightnight", false);
    tester.test("stopstopnightnight", false);
    tester.test("a", true);

    tester.update_regex("spite[a-z]+on");
    tester.test("spiteaaon", true);
    tester.test("spitebbon", true);
    tester.test("spitezzzon", true);
    tester.test("spiteon", false);
    tester.test("spiteZon", false);

    tester.update_regex("([a-z]+[8-9]?)+");
    tester.test("aaa9bbb8ccc", true);
    tester.test("zef8tef9", true);
    tester.test("quiteathing", true);
    tester.test("8", false);
    tester.test("a89", false);
    tester.test("that5this2", false);
    
    tester.update_regex("(sing|bring)+athing(that|will+)slap?");
    tester.test("singathingthat", true);
    tester.test("singathingthatslap", true);
    tester.test("singathingwillslap", true);
    tester.test("singathingwillwillslap", true);
    tester.test("bringathingwillwill", true);
    tester.test("singbringsingbringathingwillwillslap", true);
 

    tester.done();

}