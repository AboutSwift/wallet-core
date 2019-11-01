// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "ParamNumber.h"
#include "Parameters.h"

#include <cassert>

namespace TW::Ethereum::ABI {

/// Dynamic array of the same types
class ParamArray: public ParamCollection
{
private:
    ParamSet _params;

public:
    ParamArray() = default;
    ParamArray(ParamBase* param1) : ParamCollection() { addParam(param1); }
    ParamArray(std::vector<ParamBase*> params) : ParamCollection() { setVal(params); }
    void setVal(std::vector<ParamBase*> params) { addParams(params); }
    std::vector<ParamBase*> const& getVal() const { return _params.getParams(); }
    int addParam(ParamBase* param);
    void addParams(std::vector<ParamBase*> params);
    std::string getFirstType() const;
    ParamBase* getParam(int paramIndex) { return _params.getParamUnsafe(paramIndex); }
    virtual std::string getType() const { return getFirstType() + "[]"; }
    virtual size_t getSize() const { return _params.getSize(); }
    virtual bool isDynamic() const { return true; }
    virtual size_t getCount() const { return _params.getCount(); }
    virtual void encode(Data& data) const;
    virtual bool decode(const Data& encoded, size_t& offset_inout);
};

} // namespace TW::Ethereum::ABI
