#pragma once

#include "CELLMath.hpp"
namespace   CELL
{
    class   CELLObject
    {
    protected:
        void*   _user;
    public:
        CELLObject(void* user) : _user(user)
        {}

        virtual ~CELLObject()
        {}

        void    setUser(void* user)
        {
            _user   =   user;
        }
        void*   getUser()
        {
            return  _user;
        }

    };
}