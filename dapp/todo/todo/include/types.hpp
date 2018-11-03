#include <eosiolib/eosio.hpp>
#include <eosiolib/types.hpp>
#define ACTION_CREATE_ORG std::string("createorg")
#define ACTION_JOIN_TEAM std::string("jointeam")
#define ACTION_ADD_TODO std::string("addtodo")
#define ACTION_CHECK_TODO std::string("checktodo")
#define ACTION_CLOSE_TODO std::string("closetodo")
#define NST_SYMBOL S(4,NST)
#define NST_CONTRACT N(nsealstokens)
#define NST_CREATE_ORG N(createorg)
#define NST_JOIN_TEAM N(jointeam)
#define NST_ADD_TODO N(addtodo)
#define NST_CHECK_TODO N(checktodo)
#define NST_CLOSE_TODO N(closetodo)
#define NST_UNKOWN N(unknow)



using namespace std;
using namespace eosio;

//
#define NST_COST_CREATE_ORG eosio::asset(2000000,NST_SYMBOL)
#define NST_COST_JOIN_TEAM eosio::asset(100000,NST_SYMBOL)
#define NST_COST_ADD_TODO eosio::asset(100000,NST_SYMBOL)
#define NST_COST_EXCHANGE eosio::asset(50000,NST_SYMBOL)
#define NST_COST_ZERO eosio::asset(0,NST_SYMBOL)
//@abi table orgs i64
struct todo_org {
    uint64_t id;
    string orgname;
    account_name creator;
    
    uint64_t primary_key() const { return id; }
};
//@abi table users i64
struct todo_usr {
    uint64_t id;
    uint64_t orgid;
    account_name useraccount;
    string nickname;
    string teamname;
    string otherprofile;
    uint64_t primary_key() const { return id; }
    uint64_t by_org() const { return orgid; }
    uint64_t by_ua() const { return useraccount; }
};

//@abi table todos i64
struct todo_item{
    uint64_t id;
    uint64_t orgid;
    account_name owner;
    account_name payer;
    string title;
    string content;
    uint64_t endtime;
    asset bonus;
    asset ownerpay;
    char r;//0 inited 1 due 2 fail 3 close
    uint64_t primary_key() const { return id; }
    uint64_t by_org() const { return orgid; }
};
//@abi table refs i64
struct todo_ref{
    uint64_t ref;
    uint64_t type;
    account_name payer;
    asset pay;
    char r; //0:pending 1:complete
    uint64_t primary_key() const { return ref; }
};



typedef multi_index<N(orgs), todo_org>
    tb_orgs;



typedef multi_index<N(refs), todo_ref>
    tb_refs;
//the table for store all todos,can be indexed by orgid
typedef multi_index<
    N(todos),
    todo_item,
    indexed_by<N(by_org),
               const_mem_fun<todo_item, uint64_t, &todo_item::by_org>>>
    tb_todos;

typedef multi_index<N(users),todo_usr,
indexed_by<N(by_org),const_mem_fun<todo_usr, uint64_t, &todo_usr::by_org>>,
indexed_by<N(by_ua),const_mem_fun<todo_usr, uint64_t, &todo_usr::by_ua>>
>
    tb_users;
