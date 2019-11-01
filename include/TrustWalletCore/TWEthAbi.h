#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Algorand transactions.
TW_EXPORT_CLASS
struct TWEthAbi;

/*
 * Handle to a Function object.
 * It can be created, parameters get/set, used in encode, and deleted at the end.
 * Create allocates memory, the delete call should be called at the end to release memory.
 * Example:
 *   TW_EthAbiFunction func = TWEthAbiFunctionCreate(TWStringCreateWithUTF8Bytes("baz"));
 *   int p1index = TWEthAbiFuncAddInParam(func, TWEthAbiCreateParamInt32(69));
 *   int p2index = TWEthAbiFuncAddOutParam(func, TWEthAbiCreateParamInt64(0));
 *   uint8_t p2val = TWEthAbiParamInt32Val(TWEthAbiFuncGetOutParam(func, p1index);
 *   TWEthAbiFunctionDelete(func);
 */
typedef void *_Nonnull TW_EthAbiFunction;

/// Handle to any Parameter
typedef void *_Nonnull TW_EthAbiParam;

/*
 * Handle to a ParamArray object.
 * It can be created, parameters get/set, added to a function, and deleted at the end.
 * Create allocates memory, the delete call should be called at the end to release memory.
 * Example:
 *   TW_EthAbiParamArray paramArray = TWEthAbiParamArrayCreate();
 *   TWEthAbiParamArrayAddParam(paramArray, TWEthAbiCreateParamBool(true));
 *   TWEthAbiParamArrayAddParam(paramArray, TWEthAbiCreateParamUInt32(69));
 *   int pindex = TWEthAbiFuncAddInParam(func, paramArray);
 *   TWEthAbiFunctionDelete(func);
 */
typedef void *_Nonnull TW_EthAbiParamArray;

/// Creates a function object, with the given name and empty parameter list.  It must be deleted at the end.
TW_EXPORT_STATIC_METHOD
TW_EthAbiFunction TWEthAbiFunctionCreate(TWString *_Nonnull name);

/// Deletes a function object created with a 'TWEthAbiFunctionCreate' method.  The handle (passed by address) is nulled.  After delete it must not be used (can segfault)!
TW_EXPORT_STATIC_METHOD
void TWEthAbiFunctionDelete(TW_EthAbiFunction function);
    
/// Return the function type signature, of the form "baz(int32,uint256)"
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWEthAbiFuncGetType(TW_EthAbiFunction func);

/// Param creator functions
TW_EXPORT_STATIC_METHOD
TW_EthAbiParam TWEthAbiCreateParamBool(bool val);
TW_EXPORT_STATIC_METHOD
TW_EthAbiParam TWEthAbiCreateParamUInt32(uint32_t val);
//TW_EXPORT_STATIC_METHOD
TW_EthAbiParam TWEthAbiCreateParamInt32(int32_t val);
TW_EXPORT_STATIC_METHOD
TW_EthAbiParam TWEthAbiCreateParamUInt256(TWData *_Nonnull val);
TW_EXPORT_STATIC_METHOD
TW_EthAbiParam TWEthAbiCreateParamString(TWString *_Nonnull val);
TW_EXPORT_STATIC_METHOD
TW_EthAbiParam TWEthAbiCreateParamByteArray(TWData *_Nonnull val);
TW_EXPORT_STATIC_METHOD
TW_EthAbiParam TWEthAbiCreateParamByteArrayFix(int size, TWData *_Nonnull val);
// add more

/// Param accessor functions
TW_EXPORT_STATIC_METHOD
bool TWEthAbiParamBoolVal(TW_EthAbiParam val);
TW_EXPORT_STATIC_METHOD
uint32_t TWEthAbiParamInt32Val(TW_EthAbiParam val);
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWEthAbiParamUInt256Val(TW_EthAbiParam val);
// add more

/// Adds an input parameter of the given type. Ownership of the param object is taken over (must not be deleted).
/// Returns the index of the parameter (0-based).
TW_EXPORT_STATIC_METHOD
int TWEthAbiFuncAddInParam(TW_EthAbiFunction func, TW_EthAbiParam param);
/// Adds an output parameter of the given type. Ownership of the param object is taken over (must not be deleted).
/// Returns the index of the parameter (0-based).
TW_EXPORT_STATIC_METHOD
int TWEthAbiFuncAddOutParam(TW_EthAbiFunction func, TW_EthAbiParam param);

/// Obtain an output parameter
TW_EXPORT_STATIC_METHOD
TW_EthAbiParam TWEthAbiFuncGetOutParam(TW_EthAbiFunction func, int paramIndex);

/// Encode function to Eth ABI binary
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWEthAbiFuncEncode(TW_EthAbiFunction func);

/// Creates a ParamArray object, with the given name and empty parameter list.  It must be deleted at the end.
TW_EXPORT_STATIC_METHOD
TW_EthAbiParamArray TWEthAbiParamArrayCreate();
TW_EXPORT_STATIC_METHOD
void TWEthAbiParamArrayDelete(TW_EthAbiParamArray param);
/// Adds a parameter to a ParamArray. Ownership of the param object is taken over (must not be deleted).
/// Returns the index of the parameter (0-based).
TW_EXPORT_STATIC_METHOD
int TWEthAbiParamArrayAddParam(TW_EthAbiParamArray array, TW_EthAbiParam param);

TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWEthAbiEncodeString(TWString *_Nonnull string);
//TW_EXPORT_STATIC_METHOD
bool TWEthAbiDecodeString(TWString *_Nonnull encoded_in, TWString *_Nonnull *_Nonnull decoded_out);

TW_EXTERN_C_END
