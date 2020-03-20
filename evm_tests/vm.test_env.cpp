#include "catch.hpp"
#include <evm/utils.h>
#include <evm/vm.h>
#include <evm/hex.h>
#include <evm/return_data.h>
#include <evm/call.h>
#include <evm/gasometer.h>
#include "external_mock.h"

TEST_CASE("Blockhash (stub)", "[env]") {
  // given
  // (PUSH1 ((60)00))
  // (BLOCKHASH 40)
  std::string bytecode_str = "600040";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK("f1250fd89a1c3e517ae92cc1f73865c594bfad34db20f3b3396af4efe19d3bfb" == 
    Utils::uint256_2str(vm.stack.top())
  );
}

TEST_CASE("Coinbase (stub)", "[env]") {
  // given
  // (COINBASE 41)
  std::string bytecode_str = "41";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK("0000000000000000000000002adc25665018aa1fe0e6bc666dac8fc2697ff9ba" == 
    Utils::uint256_2str(vm.stack.top())
  );
}

TEST_CASE("Difficulty (stub)", "[env]") {
  // given
  // (DIFFICULTY 44)
  std::string bytecode_str = "44";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK("0000000000000000000000000000000000000000000000000000000000000100" == 
    Utils::uint256_2str(vm.stack.top())
  );
}

TEST_CASE("Timestamp", "[env]") {
  // given
  // (TIMESTAMP 42)
  std::string bytecode_str = "42";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(1 == vm.stack.top());
}

TEST_CASE("Number", "[env]") {
  // given
  // (NUMBER 43)
  std::string bytecode_str = "43";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(16339169 == vm.stack.top());
}

TEST_CASE("Gas limit", "[env]") {
  // given
  // (GASLIMIT 45)
  std::string bytecode_str = "45";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(100000 == vm.stack.top());
}

TEST_CASE("Chain id", "[env]") {
  // given
  // (CHAINID 45)
  std::string bytecode_str = "46";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(1 == vm.stack.top());
}

TEST_CASE("Call value", "[env]") {
  // given
  // (CALLVALUE 34)
  std::string bytecode_str = "34";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(34 == vm.stack.top());
}