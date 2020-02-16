#include <evm/hex.h>

std::string Hex::bytesToHex(bytes_t& bytes) {
  std::string result;
  result.reserve(bytes.size() * 2);

  for (uint8_t c : bytes) {
    result.push_back(hex[c / 16]);
    result.push_back(hex[c % 16]);
  }

  return result;
}

 bytes_t Hex::hexToBytes(std::string hex) {
  bytes_t bytes;
  for (unsigned int i = 0; i < hex.length(); i+=2) {
    std::string byteString = hex.substr(i, 2);
    bytes.push_back((uint8_t) strtol(byteString.c_str(), NULL, 16));
  }
  return bytes;
 }