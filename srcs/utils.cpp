#include "../includes/utils.hpp"

std::vector<std::string> parseCommand(std::string _command)
{
    std::vector<std::string> _vectorCommand;
    std::string temp;

    for(size_t i = 0 ; i < _command.size(); i ++)
    {
        if(_command[i] == ' ')
        {
            if(temp.size() == 0)
                continue;
            else
            {
                _vectorCommand.push_back(temp);
                temp.clear();
            }
        }
        else if (_command[i] == ':' && _vectorCommand.size() != 0 )
        {
            i++;
            while(i < _command.size())
            {
                temp += _command[i];
                i++;
            }
            _vectorCommand.push_back(temp);
            break;
        }
        else
        {
            temp += _command[i];
        }
    }
    if(temp.size() != 0)
        _vectorCommand.push_back(temp);
    return(_vectorCommand);
}