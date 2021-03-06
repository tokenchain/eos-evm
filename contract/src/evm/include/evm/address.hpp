#pragma once
#include <evm/hash.hpp>
#include <evm/rlp_encode.hpp>
#include <evm/hex.hpp>
#include <evm/big_int.hpp>
#include <evm/types.h>

class Address {
  public:
    static bytes_t accountIdentifierFromString(const std::string& accountName, const std::string& addressString) {
      bytes_t address = Hex::hexToBytes(addressString.substr(2, addressString.size()));
      return accountIdentifierFromBytes(accountName, address);
    }

    static bytes_t accountIdentifierFromBytes(const std::string& accountName, const bytes_t& address) {
      bytes_t accountNameBytes(accountName.begin(), accountName.end());

      RLPItem accountNameItem {
        RLPType::STRING,
        accountNameBytes
      };

      RLPItem addressItem {
        RLPType::STRING,
        address
      };

      RLPItem accountNameAddressList {
        RLPType::LIST,
        rlp_list_t { accountNameItem, addressItem }
      };

      bytes_t result = RLPEncode::encode(accountNameAddressList);

      bytes_t hashBytes = Hash::keccak256(result);

      bytes_t accountIdentifierBytes(hashBytes.end() - ADDRESS_SIZE, hashBytes.end());

      return accountIdentifierBytes;
    }

    static bytes_t ethereumAddress(const bytes_t& uncompressedPubKey) {
      bytes_t hash = Hash::keccak256(uncompressedPubKey);

      bytes_t address;

      for (int i = hash.size() - 20; i < hash.size(); i++) {
        address.push_back(hash[i]);
      }

      return address;
    }

    static uint256_t ethereumAddressFrom(const uint256_t& address, const uint256_t& nonce) {

      RLPItem addressItem {
        RLPType::STRING,
        BigInt::toBytes(address)
      };

      RLPItem nonceItem {
        RLPType::STRING,
        BigInt::toBytes(nonce)
      };

      RLPItem addressNonceList {
        RLPType::LIST,
        rlp_list_t { addressItem, nonceItem }
      };

      bytes_t result = RLPEncode::encode(addressNonceList);

      bytes_t hashBytes = Hash::keccak256(result);

      bytes_t accountIdentifierBytes(hashBytes.end() - ADDRESS_SIZE, hashBytes.end());

      return BigInt::fromBigEndianBytes(accountIdentifierBytes);
    }

    static uint256_t ethereumAddressFrom(
      const uint256_t& address, 
      const uint256_t& salt, 
      std::shared_ptr<bytes_t> code
    ) {

      bytes_t codeHash = Hash::keccak256(code);

      RLPItem singleByte {
        RLPType::STRING,
        bytes_t { 0xFF }
      };

      RLPItem addressItem {
        RLPType::STRING,
        BigInt::toBytes(address)
      };

      RLPItem saltItem {
        RLPType::STRING,
        BigInt::toBytes(salt)
      };

      RLPItem codeHashItem {
        RLPType::STRING,
        codeHash
      };

      RLPItem addressList {
        RLPType::LIST,
        rlp_list_t { singleByte, addressItem, saltItem, codeHashItem }
      };

      bytes_t result = RLPEncode::encode(addressList);

      bytes_t hashBytes = Hash::keccak256(result);

      bytes_t accountIdentifierBytes(hashBytes.end() - ADDRESS_SIZE, hashBytes.end());

      return BigInt::fromBigEndianBytes(accountIdentifierBytes);
    }

    static uint256_t makeCreateAddress(
      const AddressScheme addressScheme,
      const uint256_t& senderAddress,
      const uint256_t& salt,
      std::shared_ptr<bytes_t> code
    ) {

      uint256_t codeAddress;

      switch (addressScheme) {
        case AddressScheme::SENDER:
          return senderAddress;
        case AddressScheme::LEGACY:
          return Address::ethereumAddressFrom(senderAddress, salt);
        case AddressScheme::EIP_1014:
          return Address::ethereumAddressFrom(senderAddress, salt, code);
      }
    }
};