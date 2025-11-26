#pragma once

/// Interface du patron Command (avec undo/redo)
class ICommand {
public:
    virtual ~ICommand() = default;

    virtual void executer() = 0;
    virtual void annuler() = 0;   // nécessaire pour undo
};
