// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthAbi.h>
#include <TrustWalletCore/TWString.h>

#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"

#include <gtest/gtest.h>
#include <string>

namespace TW::Ethereum {

TEST(TWABI, FuncCreateEmpty) {
    TW_EthAbiFunction func = TWEthAbiFunctionCreate(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    TWString* type = TWEthAbiFuncGetType(func);
    std::string type2 = TWStringUTF8Bytes(type);
    EXPECT_EQ("baz()", type2);

    // delete
    TWEthAbiFunctionDelete(func);
}

TEST(TWABI, FuncCreate1) {
    TW_EthAbiFunction func = TWEthAbiFunctionCreate(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    int p1index = TWEthAbiFuncAddInParam(func, TWEthAbiCreateParamInt32(69));
    EXPECT_EQ(0, p1index);
    int p2index = TWEthAbiFuncAddOutParam(func, TWEthAbiCreateParamInt32(0));
    EXPECT_EQ(0, p2index);
    // check back get value
    int p2val2 = TWEthAbiParamInt32Val(TWEthAbiFuncGetOutParam(func, p2index));
    EXPECT_EQ(0, p2val2);

    TWString* type = TWEthAbiFuncGetType(func);
    std::string type2 = TWStringUTF8Bytes(type);
    EXPECT_EQ("baz(int32)", type2);

    // delete
    TWEthAbiFunctionDelete(func);
}

TEST(TWABI, FuncCreate2) {
    TW_EthAbiFunction func = TWEthAbiFunctionCreate(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    TWString* p1valStr = TWStringCreateWithUTF8Bytes("0045");
    TWData* p1val = TWDataCreateWithHexString(p1valStr);
    int p1index = TWEthAbiFuncAddInParam(func, TWEthAbiCreateParamUInt256(p1val));
    EXPECT_EQ(0, p1index);
    //TWDataDelete(p1val);
    TWStringDelete(p1valStr);

    int p2index = TWEthAbiFuncAddOutParam(func, TWEthAbiCreateParamUInt256(TWDataCreateWithSize(0)));
    EXPECT_EQ(0, p2index);

    // check back get value
    TWData* p2val2 = TWEthAbiParamUInt256Val(TWEthAbiFuncGetOutParam(func, p2index));
    Data p2val3 = data(TWDataBytes(p2val2), TWDataSize(p2val2));
    EXPECT_EQ("00", hex(p2val3));

    TWString* type = TWEthAbiFuncGetType(func);
    EXPECT_EQ("baz(uint256)", std::string(TWStringUTF8Bytes(type)));

    // delete
    TWEthAbiFunctionDelete(func);
}

TEST(TWABI, EncodeFuncCase1) {
    TW_EthAbiFunction func = TWEthAbiFunctionCreate(TWStringCreateWithUTF8Bytes("sam"));
    EXPECT_TRUE(func != nullptr);
    
    EXPECT_EQ(0, TWEthAbiFuncAddInParam(func, TWEthAbiCreateParamByteArray(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("64617665")))));
    EXPECT_EQ(1, TWEthAbiFuncAddInParam(func, TWEthAbiCreateParamBool(true)));
    TW_EthAbiParamArray paramArray = TWEthAbiParamArrayCreate();
    EXPECT_EQ(0, TWEthAbiParamArrayAddParam(paramArray, TWEthAbiCreateParamUInt256(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("01")))));
    EXPECT_EQ(1, TWEthAbiParamArrayAddParam(paramArray, TWEthAbiCreateParamUInt256(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("02")))));
    EXPECT_EQ(2, TWEthAbiParamArrayAddParam(paramArray, TWEthAbiCreateParamUInt256(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("03")))));
    EXPECT_EQ(2, TWEthAbiFuncAddInParam(func, paramArray));

    TWString* type = TWEthAbiFuncGetType(func);
    EXPECT_EQ("sam(bytes,bool,uint256[])", std::string(TWStringUTF8Bytes(type)));

    TWData* encoded = TWEthAbiFuncEncode(func);
    Data enc2 = data(TWDataBytes(encoded), TWDataSize(encoded));
    EXPECT_EQ("a5643bf2"
        "0000000000000000000000000000000000000000000000000000000000000060"
        "0000000000000000000000000000000000000000000000000000000000000001"
        "00000000000000000000000000000000000000000000000000000000000000a0"
        "0000000000000000000000000000000000000000000000000000000000000004"
        "6461766500000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000003"
        "0000000000000000000000000000000000000000000000000000000000000001"
        "0000000000000000000000000000000000000000000000000000000000000002"
        "0000000000000000000000000000000000000000000000000000000000000003",
        hex(enc2));

    // delete
    TWEthAbiFunctionDelete(func);
}

TEST(TWABI, EncodeFuncCase2) {
    TW_EthAbiFunction func = TWEthAbiFunctionCreate(TWStringCreateWithUTF8Bytes("f"));
    EXPECT_TRUE(func != nullptr);
    
    EXPECT_EQ(0, TWEthAbiFuncAddInParam(func, TWEthAbiCreateParamUInt256(TWDataCreateWithHexString(
        TWStringCreateWithUTF8Bytes("0123")))));
    TW_EthAbiParamArray paramArray = TWEthAbiParamArrayCreate();
    EXPECT_EQ(0, TWEthAbiParamArrayAddParam(paramArray, TWEthAbiCreateParamUInt32(0x456)));
    EXPECT_EQ(1, TWEthAbiParamArrayAddParam(paramArray, TWEthAbiCreateParamUInt32(0x789)));
    EXPECT_EQ(1, TWEthAbiFuncAddInParam(func, paramArray));
    EXPECT_EQ(2, TWEthAbiFuncAddInParam(func, TWEthAbiCreateParamByteArrayFix(10, TWDataCreateWithHexString(
        TWStringCreateWithUTF8Bytes("31323334353637383930")))));
    EXPECT_EQ(3, TWEthAbiFuncAddInParam(func, TWEthAbiCreateParamString(TWStringCreateWithUTF8Bytes("Hello, world!"))));

    TWString* type = TWEthAbiFuncGetType(func);
    EXPECT_EQ("f(uint256,uint32[],bytes10,string)", std::string(TWStringUTF8Bytes(type)));

    TWData* encoded = TWEthAbiFuncEncode(func);
    Data enc2 = data(TWDataBytes(encoded), TWDataSize(encoded));
    EXPECT_EQ("47b941bf"
        "0000000000000000000000000000000000000000000000000000000000000123"
        "0000000000000000000000000000000000000000000000000000000000000080"
        "3132333435363738393000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000e0"
        "0000000000000000000000000000000000000000000000000000000000000002"
        "0000000000000000000000000000000000000000000000000000000000000456"
        "0000000000000000000000000000000000000000000000000000000000000789"
        "000000000000000000000000000000000000000000000000000000000000000d"
        "48656c6c6f2c20776f726c642100000000000000000000000000000000000000",
        hex(enc2));

    // delete
    TWEthAbiFunctionDelete(func);
}

TEST(TWABI, EncodeString) {
    TWString* string = TWStringCreateWithUTF8Bytes("Hello");
    TWString* encoded = TWEthAbiEncodeString(string);
    EXPECT_EQ(std::string(
        "0000000000000000000000000000000000000000000000000000000000000005"\
        "48656c6c6f000000000000000000000000000000000000000000000000000000"), 
        std::string(TWStringUTF8Bytes(encoded)));
}

TEST(TWABI, DecodeString) {
    TWString* encoded = TWStringCreateWithUTF8Bytes(
        "0000000000000000000000000000000000000000000000000000000000000005"\
        "48656c6c6f000000000000000000000000000000000000000000000000000000"
    );
    TWString* decoded;
    bool res = TWEthAbiDecodeString(encoded, &decoded);
    EXPECT_TRUE(res);
    EXPECT_EQ(std::string("Hello"), std::string(TWStringUTF8Bytes(decoded)));
}
} // namespace TW::Ethereum
