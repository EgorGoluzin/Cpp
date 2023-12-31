#pragma once
#include <string>
#include <functional>
#include "wrapper.h"

class eng 
{
private:
    std::unordered_map<std::string, std::function<int(std::unordered_map<std::string, int>)>> commands;

public:
    template<typename ClassType, typename... Args>
    void comand(wrapper<ClassType, Args...>* wrapper, const std::string& command) 
    {
        if (commands.find(command) != commands.end())
        {
            throw std::runtime_error("Command " + command + "is  already registered");
        }

        commands[command] = [wrapper](std::unordered_map<std::string, int> args) 
        {
            return (*wrapper)(std::move(args));
        };
    };

    int execute(const std::string& command, std::unordered_map<std::string, int> args) 
    {
        auto command_iterator = commands.find(command);

        if (command_iterator == commands.end())
        {
            throw std::runtime_error("Command not found");
        }

        auto func = &command_iterator->second;

        return (*func)(std::move(args));
    };
}
