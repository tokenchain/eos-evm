#include "transaction.h"
#include "rlp.h"
#include "big_int.h"

transaction_t Transaction::parse(std::vector<uint8_t> bytes) {
  std::vector<RLPItem> items = std::vector<RLPItem>();
  RLPDecode::decode(bytes, items);

  address_t address = items[0].values[3].bytes;

  return {
    address.size() == 0 ? 
      transaction_action_t::TRANSACTION_CREATE :
      transaction_action_t::TRANSACTION_CALL,
    BigInt::fromBigEndianBytes(items[0].values[0].bytes), /* nonce */
    BigInt::fromBigEndianBytes(items[0].values[1].bytes), /* gas_price */
    BigInt::fromBigEndianBytes(items[0].values[2].bytes), /* gas_limit */
    address, /* to */
    BigInt::fromBigEndianBytes(items[0].values[4].bytes), /* value */
    items[0].values[5].bytes, /* data */
    BigInt::fromBigEndianBytes(items[0].values[6].bytes), /* v */
    items[0].values[7].bytes, /* r */
    items[0].values[8].bytes /* s */
  };
}