#include <TrustWalletCore/TWEthAbi.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWData.h>

#include "Ethereum/ABI.h"
#include "Data.h"
#include "HexCoding.h"
#include "../uint256.h"

#include <string>
#include <vector>

using namespace TW;
using namespace TW::Ethereum;
using namespace TW::Ethereum::ABI;


TW_EthAbiFunction TWEthAbiFunctionCreate(TWString *_Nonnull name) {
    auto func = new Function(TWStringUTF8Bytes(name));
    TWStringDelete(name);
    return static_cast<TW_EthAbiFunction>(func);
}

void TWEthAbiFunctionDelete(TW_EthAbiFunction function) {
    auto func = static_cast<const Function*>(function);
    assert(func != nullptr);
    delete func;
}

TWString *_Nonnull TWEthAbiFuncGetType(TW_EthAbiFunction func) {
    auto function = static_cast<Function*>(func);
    assert(function != nullptr);
    std::string sign = function->getType();
    return TWStringCreateWithUTF8Bytes(sign.c_str());
}

TW_EthAbiParam TWEthAbiCreateParamBool(bool val) {
    auto param = new ParamBool(val);
    return static_cast<TW_EthAbiParam>(param);
}

TW_EthAbiParam TWEthAbiCreateParamUInt32(uint32_t val) {
    auto param = new ParamUInt32(val);
    return static_cast<TW_EthAbiParam>(param);
}

TW_EthAbiParam TWEthAbiCreateParamInt32(int32_t val) {
    auto param = new ParamInt32(val);
    return static_cast<TW_EthAbiParam>(param);
}

TW_EthAbiParam TWEthAbiCreateParamUInt256(TWData *_Nonnull val) {
    uint256_t val2 = load(*static_cast<const Data*>(val));
    TWDataDelete(val); // remove original
    auto param = new ParamUInt256(val2);
    return static_cast<TW_EthAbiParam>(param);
}

TW_EthAbiParam TWEthAbiCreateParamString(TWString *_Nonnull val) {
    std::string valStr(TWStringUTF8Bytes(val));
    auto param = new ParamString(valStr);
    return static_cast<TW_EthAbiParam>(param);
}

TW_EthAbiParam TWEthAbiCreateParamByteArray(TWData *_Nonnull val) {
    Data dataDat = TW::data(TWDataBytes(val), TWDataSize(val));
    auto param = new ParamByteArray(dataDat);
    TWDataDelete(val);
    return static_cast<TW_EthAbiParam>(param);
}

TW_EthAbiParam TWEthAbiCreateParamByteArrayFix(int size, TWData *_Nonnull val) {
    Data dataDat = TW::data(TWDataBytes(val), TWDataSize(val));
    auto param = new ParamByteArrayFix(size, dataDat);
    TWDataDelete(val);
    return static_cast<TW_EthAbiParam>(param);
}

bool TWEthAbiParamBoolVal(TW_EthAbiParam val) {
    auto param = static_cast<ParamBool*>(val);
    assert(param != nullptr);
    return param->getVal();
}

uint32_t TWEthAbiParamInt32Val(TW_EthAbiParam val) {
    auto param = static_cast<ParamInt32*>(val);
    assert(param != nullptr);
    return param->getVal();
}

TWData *_Nonnull TWEthAbiParamUInt256Val(TW_EthAbiParam val) {
    auto param = static_cast<ParamUInt256*>(val);
    assert(param != nullptr);
    uint256_t val256 = param->getVal();
    TW::Data valDat = TW::store(val256);
    return TWDataCreateWithData(&valDat);
}

int TWEthAbiFuncAddInParam(TW_EthAbiFunction func, TW_EthAbiParam param) {
    auto function = static_cast<Function*>(func);
    assert(function != nullptr);
    auto param2 = static_cast<ParamBase*>(param);
    assert(param2 != nullptr);
    return function->addInParam(param2);
}

int TWEthAbiFuncAddOutParam(TW_EthAbiFunction func, TW_EthAbiParam param) {
    auto function = static_cast<Function*>(func);
    assert(function != nullptr);
    auto param2 = static_cast<ParamBase*>(param);
    assert(param2 != nullptr);
    return function->addOutParam(param2);
}

TW_EthAbiParam TWEthAbiFuncGetOutParam(TW_EthAbiFunction func, int paramIndex) {
    auto function = static_cast<Function*>(func);
    assert(function != nullptr);
    ParamBase* baseParam = nullptr;
    if (!function->getOutParam(paramIndex, &baseParam) || baseParam == nullptr) {
        return static_cast<TW_EthAbiParam>(nullptr);
    }
    assert(baseParam != nullptr);
    return static_cast<TW_EthAbiParam>(baseParam);
}

TWData *_Nonnull TWEthAbiFuncEncode(TW_EthAbiFunction func) {
    auto function = static_cast<Function*>(func);
    assert(function != nullptr);
    Data* encoded = new Data(); // allocated on heap
    function->encode(*encoded);
    return static_cast<TWData*>(encoded);
}

TW_EthAbiParamArray TWEthAbiParamArrayCreate() {
    auto param = new ParamArray();
    return static_cast<TW_EthAbiParamArray>(param);
}

void TWEthAbiParamArrayDelete(TW_EthAbiParamArray param) {
    auto param2 = static_cast<const ParamArray*>(param);
    assert(param2 != nullptr);
    delete param2;
}

int TWEthAbiParamArrayAddParam(TW_EthAbiParamArray array, TW_EthAbiParam param) {
    auto paramArray = static_cast<ParamArray*>(array);
    assert(paramArray != nullptr);
    auto param2 = static_cast<ParamBase*>(param);
    assert(param2 != nullptr);
    return paramArray->addParam(param2);
}


TWString *_Nonnull TWEthAbiEncodeString(TWString *_Nonnull string) {
    auto str2 = std::string(TWStringUTF8Bytes(string));
    Data encoded;
    ParamString::encodeString(str2, encoded);
    TWString* encHex = TWStringCreateWithUTF8Bytes(hex(encoded).data());
    return encHex;
}

bool TWEthAbiDecodeString(TWString *_Nonnull encoded_in, TWString *_Nonnull *_Nonnull decoded_out) {
    Data encoded = parse_hex(std::string(TWStringUTF8Bytes(encoded_in))); //data(std::string(TWStringUTF8Bytes(encoded_in)));
    std::string decoded;
    size_t offset = 0;
    bool res = ParamString::decodeString(encoded, decoded, offset);
    *decoded_out = TWStringCreateWithUTF8Bytes(decoded.data());
    return res;
}
