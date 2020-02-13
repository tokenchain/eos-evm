#include <vector>
#include "catch.hpp"
#include "hash.h"
#include "big_int.h"
#include "utils.h"

TEST_CASE("keccak256", "[hash]" ) {
  std::vector<uint8_t> bytes = std::vector<uint8_t>();
  uint256_t hash = Hash::keccak256(bytes);
  REQUIRE("c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470" == 
    Utils::uint256_2str(hash)
  );
}