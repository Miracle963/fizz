/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>

#include <fizz/crypto/exchange/X25519.h>
#include <folly/Range.h>
#include <folly/String.h>

using namespace folly;

namespace fizz {
namespace test {

TEST(X25519KeyExchange, KeyExchange) {
  static constexpr StringPiece keyShareHex =
      "c81e57c7485ba417280bc2d48d864afd3966ff77b684bfaf85f418f9b4583347";
  auto keyShare = unhexlify(keyShareHex);
  X25519KeyExchange kex;
  kex.generateKeyPair();
  auto out = kex.generateSharedSecret(folly::range(keyShare));
}

TEST(X25519KeyExchange, SmallKeyExchange) {
  static constexpr StringPiece keyShareHex = "c81e57c7485ba4";
  auto keyShare = unhexlify(keyShareHex);
  X25519KeyExchange kex;
  kex.generateKeyPair();
  EXPECT_THROW(
      kex.generateSharedSecret(folly::range(keyShare)), std::runtime_error);
}
} // namespace test
} // namespace fizz