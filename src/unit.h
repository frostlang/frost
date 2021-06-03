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

        static Unit create(){
            Unit u;
            return u;
        }
        
        Unit& from_file(std::string file){
            // load up file
            return *this;
        }

        Unit& from_source(std::string file, std::string_view source){
            m_file = file;
            m_source = source;
            return *this;
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