#pragma once

#include <string>
#include <string_view>
#include <debug.h>

namespace Frost{

    /*
    Represents a compilation unit
    */
    class Unit : public Debugable {
    public:

        static Unit create(std::string file, std::string_view source){
            Unit u;
            u.m_file = file;
            u.m_source = source;
            return u;
        }
        
        std::string debug() override {
            return file();
        }

        std::string file(){
            return m_file;
        }
        std::string_view& source(){
            return m_source;
        }

    private:
        std::string m_file;
        std::string_view m_source;
    };
}