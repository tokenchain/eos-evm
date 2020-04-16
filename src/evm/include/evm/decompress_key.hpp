#include <evm/types.h>
#include <evm/big_int.hpp>
#include <evm/hex.hpp>

class DecompressKey {
  public:
    static uint256_t p() {
      return BigInt::fromBigEndianBytes(Hex::hexToBytes("fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f"));
    }
    static bytes_t decompress(uint256_t p, compressed_key_t compressedKey) {
      uint8_t yParity = static_cast<uint8_t>(compressedKey[0]) - 2;
      uint256_t x = BigInt::fromCompressedKey(compressedKey);
      uint256_t a = (pow_mod(x, 3, p) + 7) % p;
      uint256_t y = pow_mod(a, (p + 1) / 4, p);
      if (y % 2 != yParity)
        y = p - y;
      
      bytes_t uncompressedKey = BigInt::toBytes(x);
      bytes_t yBytes = BigInt::toBytes(y);

      uncompressedKey.insert(uncompressedKey.end(), yBytes.begin(), yBytes.end());

      return uncompressedKey;
    }
  private:
    static uint256_t pow_mod(uint256_t x, uint256_t y, uint256_t z) {
      uint256_t number = 1;
      while (y != 0) {
        if ((y & 1) != 0)
          number = intx::mulmod(number, x, z);
        y >>= 1;
        x = intx::mulmod(x, x, z);
      }
      return number;
    }
};