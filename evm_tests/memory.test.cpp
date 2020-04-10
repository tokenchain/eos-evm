#include <vector>
#include "catch.hpp"
#include <memory>
#include <evm/memory.h>
#include <evm/types.h>
#include <evm/utils.h>
#include <evm/hex.h>
#include <evm/return_data.h>
#include <evm/call.h>
#include <evm/big_int.h>

TEST_CASE("Memory write and read", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(0x80 + 32);

  uint256_t writeWord = uint256_t(0xabcdef);
  memory->write(uint256_t(0x80), writeWord);

  // then
  uint256_t slice1 = memory->read(uint256_t(0x80));
  REQUIRE("0000000000000000000000000000000000000000000000000000000000abcdef" == Utils::uint256_2str(slice1)); 
}

TEST_CASE("Memory write and read (1)", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(0x80 + 32);

  uint256_t writeWord = uint256_t(0xabcdef);
  memory->write(uint256_t(0x80), writeWord);

  // then
  uint256_t slice1 = memory->read(uint256_t(0x80));
  REQUIRE("0000000000000000000000000000000000000000000000000000000000abcdef" == Utils::uint256_2str(slice1)); 
}

TEST_CASE("Memory write and read (2)", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(32);

  uint256_t writeWord = uint256_t(0x06);
  memory->write(uint256_t(0x00), writeWord);

  // then
  uint256_t slice1 = memory->read(uint256_t(0x00));
  REQUIRE("0000000000000000000000000000000000000000000000000000000000000006" == Utils::uint256_2str(slice1)); 
}

TEST_CASE("Memory write by byte and read", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(32);
  memory->writeByte(uint256_t(0x1d), uint256_t(0xab));
  memory->writeByte(uint256_t(0x1e), uint256_t(0xcd));
  memory->writeByte(uint256_t(0x1f), uint256_t(0xef));

  // then
  uint256_t slice1 = memory->read(uint256_t(0x00));
  REQUIRE("0000000000000000000000000000000000000000000000000000000000abcdef" == Utils::uint256_2str(slice1)); 
}

TEST_CASE("Memory write single byte and read", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(32);
  memory->writeByte(uint256_t(0x1F), uint256_t(0x06));

  // then
  uint256_t slice1 = memory->read(uint256_t(0x00));
  REQUIRE("0000000000000000000000000000000000000000000000000000000000000006" == Utils::uint256_2str(slice1)); 
}

TEST_CASE("Memory read slice and write slice", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(32);

  bytes_t slice1 = Hex::hexToBytes("aabbccee112233445566778899");
  memory->writeSlice(uint256_t(0x00), slice1);
  uint256_t index1 = uint256_t(0x00);
  uint256_t index2 = uint256_t(0x0D);
  std::shared_ptr<bytes_t> readSlice1 = memory->readSlice(index1, index2);
  REQUIRE("aabbccee112233445566778899" == Hex::bytesToHex(readSlice1)); 
  
  bytes_t slice2 = Hex::hexToBytes("FFFF");
  memory->writeSlice(uint256_t(0x01), slice2);
  uint256_t index3 = uint256_t(0x00);
  uint256_t index4 = uint256_t(0x06);
  std::shared_ptr<bytes_t> readSlice2 = memory->readSlice(index3, index4);
  REQUIRE("aaffffee1122" == Hex::bytesToHex(readSlice2)); 

  bytes_t emptySlice = bytes_t();
  memory->writeSlice(uint256_t(0x1000), emptySlice);
  REQUIRE(32 == memory->length()); 
}

TEST_CASE("Memory write at index", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(32);

  uint256_t word = Utils::bigIntFromBigEndianBytes("0000000000000000000000000000000000000000000000000000000000000000");
  memory->write(
    Utils::bigIntFromBigEndianBytes("0000000000000000000000000000000000000000000000000000000000000000"),
    word
  );

  // then
  uint256_t slice1 = memory->read(uint256_t(0x00));
  REQUIRE("0000000000000000000000000000000000000000000000000000000000000000" == 
    Utils::uint256_2str(slice1)); 
}

TEST_CASE("Memory write at index (1)", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(32);

  uint256_t word = Utils::bigIntFromBigEndianBytes("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
  memory->write(
    Utils::bigIntFromBigEndianBytes("0000000000000000000000000000000000000000000000000000000000000000"),
    word
  );

  // then
  uint256_t slice1 = memory->read(uint256_t(0x00));
  REQUIRE("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" == 
    Utils::uint256_2str(slice1)); 
}

TEST_CASE("Memory copy data", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);

  bytes_t copyBytes = Hex::hexToBytes("608060405234801561001057600080fd5b507fb8a00d6d8ca1be30bfec34d8f97e55f0f0fd9eeb7fb46e030516363d4cfe1ad630604051808273ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200191505060405180910390a160b8806100826000396000f3fe6080604052348015600f57600080fd5b506004361060285760003560e01c8063771602f714602d575b600080fd5b606060048036036040811015604157600080fd5b8101908080359060200190929190803590602001909291905050506076565b6040518082815260200191505060405180910390f35b600081830190509291505056fea265627a7a723158209551755b4e59ca6cf78d79f5356d91565950805937074458700610f23c6ecf9b64736f6c63430005100032");

  // when
  memory->expand(314);
  memory->copyData(
    uint256_t(0x00),
    uint256_t(0x82),
    uint256_t(0xB8),
    std::make_shared<bytes_t>(copyBytes)  
  );

  uint256_t index1 = uint256_t(0x00);
  uint256_t index2 = uint256_t(0xB8);
  std::shared_ptr<bytes_t> slice = memory->readSlice(index1, index2);
  REQUIRE("6080604052348015600f57600080fd5b506004361060285760003560e01c8063771602f714602d575b600080fd5b606060048036036040811015604157600080fd5b8101908080359060200190929190803590602001909291905050506076565b6040518082815260200191505060405180910390f35b600081830190509291505056fea265627a7a723158209551755b4e59ca6cf78d79f5356d91565950805937074458700610f23c6ecf9b64736f6c63430005100032" == 
    Hex::bytesToHex(slice)
  ); 
}


TEST_CASE("Memory to return", "[memory]") {
  // given
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> memory = std::make_shared<Memory>(memoryBytes);
  
  // when
  memory->expand(64);

  uint256_t word1 = Utils::bigIntFromBigEndianBytes("000000000000000000000000000000000000000000000000000000000000000a");
  memory->write(
    Utils::bigIntFromBigEndianBytes("0000000000000000000000000000000000000000000000000000000000000000"),
    word1
  );

  uint256_t word2 = Utils::bigIntFromBigEndianBytes("0000000000000000000000000000000000000000000000000000000000000000");
  memory->write(
    Utils::bigIntFromBigEndianBytes("0000000000000000000000000000000000000000000000000000000000000020"),
    word2
  );

  uint256_t slice1 = memory->read(uint256_t(0x00));
  CHECK("000000000000000000000000000000000000000000000000000000000000000a" == 
    Utils::uint256_2str(slice1)); 

  uint256_t slice2 = memory->read(uint256_t(0x20));
  CHECK("0000000000000000000000000000000000000000000000000000000000000000" == 
    Utils::uint256_2str(slice2)); 

  uint256_t index1 = uint256_t(0x1f);
  uint256_t index2 = uint256_t(0x20);
  std::shared_ptr<bytes_t> returnDataBytes = memory->readSlice(index1, index2);

  CHECK("0a00000000000000000000000000000000000000000000000000000000000000" ==
    Hex::bytesToHex(returnDataBytes)
  );
}