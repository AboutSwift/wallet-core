// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Base58.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>

#include <cassert>

using namespace TW::Ripple;

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::ripple.decodeCheck(string);
    if (decoded.size() != Address::size) {
        return false;
    }

    return true;
}

Address::Address(const std::string& string) {
    const auto decoded = Base58::ripple.decodeCheck(string);
    if (decoded.size() != Address::size) {
        throw std::invalid_argument("Invalid address string");
    }
    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    /// see type prefix: https://developers.ripple.com/base58-encodings.html
    bytes[0] = 0x00;
    go_ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.data() + 1);
}

std::string Address::string() const {
    return Base58::ripple.encodeCheck(bytes);
}
