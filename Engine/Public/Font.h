#pragma once
#include "Base.h"

class CFont :
    public CBase
{
protected:
    explicit CFont();
    virtual ~CFont() = default;

public:
    virtual void Free() override;
};

