// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "ParamNumber.h"

#include <vector>
#include <string>
#include <cassert>

namespace TW::Ethereum::ABI {

/// A set of parameters
class ParamSet {
private:
    std::vector<ParamBase*> _params;

public:
    ParamSet() = default;
    ParamSet(ParamBase* param1) { addParam(param1); }
    ParamSet(std::vector<ParamBase*> params) { addParams(params); }
    ~ParamSet();

    /// Returns the index of the parameter
    int addParam(ParamBase* param);
    void addParams(std::vector<ParamBase*> params);
    bool getParam(int paramIndex, ParamBase** param_out) const;
    ParamBase* getParamUnsafe(int paramIndex) const;
    size_t getCount() const { return _params.size(); }
    std::vector<ParamBase*> const& getParams() const { return _params; }
    /// Return the function type signature, of the form "baz(int32,uint256)"
    std::string getType() const;
    size_t getSize() const;
    size_t getHeadSize() const;
    virtual void encode(Data& data) const;
    virtual bool decode(const Data& encoded, size_t& offset_inout);
};

/// Collection of different parameters, dynamic length.
class Parameters: public ParamCollection
{
private:
    ParamSet _params;

public:
    Parameters() = default;
    Parameters(std::vector<ParamBase*> params) : ParamCollection(), _params(ParamSet(params)) {}
    void addParam(ParamBase* param) { _params.addParam(param); }
    void addParams(std::vector<ParamBase*> params) { _params.addParams(params); }
    ParamBase* getParam(int paramIndex) const { return _params.getParamUnsafe(paramIndex); }
    virtual std::string getType() const { return _params.getType(); }
    virtual size_t getSize() const { return _params.getSize(); }
    virtual bool isDynamic() const { return true; }
    virtual size_t getCount() const { return _params.getCount(); }
    virtual void encode(Data& data) const { _params.encode(data); }
    virtual bool decode(const Data& encoded, size_t& offset_inout) { return _params.decode(encoded, offset_inout); }
};

} // namespace TW::Ethereum::ABI
