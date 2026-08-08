#include "Cli/ICommand.hpp"

class RunCommand final : public ICommand{
private:
    void execute() override;

public:
    void registerCommand(CLI::App& app) override;
};