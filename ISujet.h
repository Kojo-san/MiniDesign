#pragma once
#include "IObservateur.h"
#include <vector>

/// Interface Sujet (Observable)
class ISujet {
public:
    virtual ~ISujet() = default;

    virtual void attacher(IObservateur* obs) = 0;
    virtual void detacher(IObservateur* obs) = 0;
    virtual void notifier() = 0;
};
