#include <evmsend.hpp>
#include <evm/evm.hpp>

#include <eosio/system.hpp>

using namespace std;
using namespace eosio;
using namespace eosio_evm;

ACTION evmsender::sendtoevm( name native_acct, float num_tokens) {

    require_auth( get_self() );

    asset n_tokens = asset( 
                        num_tokens,
                        symbol(symbol_code( "TLOS" ), 4));

    // Look up the EVM address from the evm contract
    evm_table_t _evmaccounts( name("eosio.evm"), name("eosio.evm").value );
    auto acct_index = _evmaccounts.get_index<"byaccount"_n>();
    auto evm_itr = acct_index.find( native_acct.value ); // Use miner's name to query

    checksum160 evmaddress = evm_itr->address;
    std::array<uint8_t, 32u> bytes = fromChecksum160( evmaddress );

    // Convert to hex and chop off padded bytes
    string memo = "0x" + bin2hex(bytes).substr(24);

    // Inline action for the token transfer
    action(
      permission_level{ get_self(), "active"_n },
      "eosio.token"_n , "transfer"_n,
      std::make_tuple( get_self(), "eosio.evm"_n, n_tokens, memo )
    ).send();

}

EOSIO_DISPATCH(evmsender, (sendtoevm))
