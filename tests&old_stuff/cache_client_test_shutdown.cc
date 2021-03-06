//g++ testing_client_shutdown.cc cache_client_test_shutdown.cc -o cls.out -lboost_system -pthread -lcurl -ljsoncpp
//run server with ./a.out 10 19086
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "h_depen/catch.hpp"
#include "cache.hh"
#include <iostream>
#include <functional>

std::string strcast(Cache::val_type vptr) //takes void ptr to str, returns str
{
    if(vptr != NULL)
        {return *((std::string*) vptr);}
    else
        {return "";}
}

//all tests must be successive because the deconstructor
//shuts down the server in this version
TEST_CASE("test shutdown version functionality")
{
    //initializing tests
    Cache test_cache(10);
    std::string a = "hello";
    Cache::val_type ap = &a;
    int as = sizeof(a);
    std::string b = "goodbye";
    Cache::val_type bp = &b;
    int bs = sizeof(b);
    std::string c = "extralongstring";
    Cache::val_type cp = &c;
    int cs = sizeof(c);
    uint32_t blnk = 0;
    std::string outstr;

//new cache delete test
    REQUIRE(test_cache.del("newk") == 0);

//new cache get test
    outstr = strcast(test_cache.get("newk",blnk)); 
    REQUIRE(outstr == "NULL");

//basic set get test
    test_cache.set("newk", ap, as);
    outstr = strcast(test_cache.get("newk",blnk));
    REQUIRE(outstr == "hello");

//basic delete test
    test_cache.del("newk");
    outstr = strcast(test_cache.get("newk",blnk));
    REQUIRE(outstr == "NULL");

//basic memused test
    test_cache.set("newk", ap, as);
    REQUIRE(test_cache.space_used()==6);

//basic deep copy test
    REQUIRE(test_cache.get("newk",blnk) != ap);

//basic evict test (relies on cache size <=12)
    test_cache.set("newk2",bp,bs);
    outstr = strcast(test_cache.get("newk",blnk));
    REQUIRE(outstr =="NULL");

//testing deleteing evicted item (relies on cache size <=12)
    REQUIRE(test_cache.del("newk") == 0);
/*
//testing "goodbye" was successfully stored (relies on cache size <=12)
    outstr = strcast(test_cache.get("newk2",blnk));
    REQUIRE(outstr == "goodbye");

//testing storing item larger then cache does nothing
    //SCARY TEST THAT SEGFAULTS AND RUINS PORT, DO NOT RUN
    test_cache.set("newk3",cp,cs);
    outstr = strcast(test_cache.get("newk3",blnk));
    REQUIRE(outstr == "NULL");

    //testing that the cache decremented space correctly (relies on cache size <=12)
    REQUIRE(test_cache.space_used()==8);
*/
}




//Test that may fail: putting in 2 null objects then deleting both