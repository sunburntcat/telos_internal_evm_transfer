// Copyright (c) Microsoft Corporation. All rights reserved.
// Copyright (c) 2020 Syed Jafri. All rights reserved.
// Licensed under the MIT License..

#pragma once

namespace bigint {
  using checksum256 = intx::uint256;
}

namespace eosio_evm
{

  static constexpr auto WORD_SIZE = 32u;

  /**
   * Conversions
   */
  static inline std::string bin2hex(const std::vector<uint8_t>& bin)
  {
    std::string res;
    const char hex[] = "0123456789abcdef";
    for(auto byte : bin) {
      res += hex[byte >> 4];
      res += hex[byte & 0xf];
    }

    return res;
  }

  template<unsigned N, typename T>
  static inline std::string bin2hex(const std::array<T, N>& bin)
  {
    std::string res;
    const char hex[] = "0123456789abcdef";
    for(auto byte : bin) {
      res += hex[byte >> 4];
      res += hex[byte & 0xf];
    }

    return res;
  }

  inline constexpr bool is_precompile(uint256_t address) {
    return address >= 1 && address <= 65535;
  }

  inline constexpr int64_t num_words(uint64_t size_in_bytes)
  {
    return (static_cast<int64_t>(size_in_bytes) + (WORD_SIZE - 1)) / WORD_SIZE;
  }

  template <typename T>
  static T shrink(uint256_t i)
  {
    return static_cast<T>(i & std::numeric_limits<T>::max());
  }

  inline std::array<uint8_t, 32u> toBin(const Address& address)
  {
    std::array<uint8_t, 32> address_bytes = {};
    intx::be::unsafe::store(address_bytes.data(), address);
    return address_bytes;
  }

  inline uint8_t toBin(char c) {
      if (c >= '0' && c <= '9') return c - '0';
      if (c >= 'a' && c <= 'f') return c - 'a' + 10;
      if (c >= 'A' && c <= 'F') return c - 'A' + 10;
      return 0;
  }

  inline std::array<uint8_t, 20u> toBin(const std::string& input) {
      std::array<uint8_t, 20> output = {};
      auto i = input.begin();
      uint8_t* out_pos = (uint8_t*)& output;
      uint8_t* out_end = out_pos + 20;
      while (i != input.end() && out_end != out_pos) {
          *out_pos = toBin((char)(*i)) << 4;
          ++i;
          if (i != input.end()) {
              *out_pos |= toBin((char)(*i));
              ++i;
          }
          ++out_pos;
      }
      return output;
  }

  inline const std::array<uint8_t, 32u> fromChecksum160(const eosio::checksum160 input)
  {
    std::array<uint8_t, 32U> output = {};
    auto input_bytes = input.extract_as_byte_array();
    std::copy(std::begin(input_bytes), std::end(input_bytes), std::begin(output) + 12);
    return output;
  }

  inline eosio::checksum160 toChecksum160(const std::array<uint8_t, 32u>& input)
  {
    std::array<uint8_t, 20> output = {};
    std::copy(std::begin(input) + 12, std::end(input), std::begin(output));
    return eosio::checksum160(output);
  }
  

  inline eosio::checksum160 toChecksum160(const std::string& input)
  {
    return eosio::checksum160( toBin(input) );
  }

  inline eosio::checksum256 toChecksum256(const Address& address)
  {
    return eosio::checksum256( toBin(address) );
  }

  static inline eosio::checksum256 pad160(const eosio::checksum160 input)
  {
    return eosio::checksum256( fromChecksum160(input) );
  }

  static inline Address checksum160ToAddress(const eosio::checksum160& input) {
    const std::array<uint8_t, 32u>& checksum = fromChecksum160(input);
    return intx::be::unsafe::load<uint256_t>(checksum.data());
  }
  static inline eosio::checksum160 addressToChecksum160(const Address& input) {
    return toChecksum160( toBin(input) );
  }

  // Do not use for addresses, only key for Account States
  static inline uint256_t checksum256ToValue(const eosio::checksum256& input) {
    std::array<uint8_t, 32U> output = {};
    auto input_bytes = input.extract_as_byte_array();
    std::copy(std::begin(input_bytes), std::end(input_bytes), std::begin(output));

    return intx::be::unsafe::load<uint256_t>(output.data());
  }

  /**
   *  Serialize a fixed size int
   *
   *  @param ds - The stream to write
   *  @param v - The value to serialize
   *  @tparam Stream - Type of datastream buffer
   *  @tparam T - Type of the object contained in the array
   *  @tparam N - Number of bits
   *  @return datastream<Stream>& - Reference to the datastream
   */
  template<typename Stream>
  eosio::datastream<Stream>& operator<< ( eosio::datastream<Stream>& ds, const bigint::checksum256& v ) {
      auto bytes = toBin(v);
      ds << bytes;
      return ds;
  }

  /**
   *  Deserialize a fixed size int
   *
   *  @brief Deserialize a fixed size std::array
   *  @param ds - The stream to read
   *  @param v - The destination for deserialized value
   *  @tparam Stream - Type of datastream buffer
   *  @tparam N - Number of bits
   *  @return datastream<Stream>& - Reference to the datastream
   */
  template<typename Stream>
  eosio::datastream<Stream>& operator>> ( eosio::datastream<Stream>& ds, bigint::checksum256& v ) {
      std::array<uint8_t, 32> tmp;
      ds >> tmp;
      v = intx::be::unsafe::load<uint256_t>(tmp.data());
      return ds;
  }

 
} // namespace eosio_evm