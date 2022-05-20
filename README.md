# evmsend example

This example is to demonstrate a basic situation where you want to send Native Telos to a user's Telos EVM account

The header files define the structures used by the EVM. To get the EVM address associated with the native Telos account, we do a lookup on the Account's third index.

From there, we can send an inline action to send tokens to that account.
