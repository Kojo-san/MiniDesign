#pragma once

/// Interface Observer
class IObservateur {
public:
    virtual ~IObservateur() = default;
    virtual void actualiser() = 0;
};
