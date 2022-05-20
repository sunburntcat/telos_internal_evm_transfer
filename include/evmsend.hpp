#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

CONTRACT evmsender : public contract {
  public:
    using contract::contract;

    ACTION sendtoevm( name native_acct, float num_tokens );

};
