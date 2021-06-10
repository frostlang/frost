#pragma once

#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <iostream>
#include <target.h>
#include <debug.h>

namespace Frost{

    /*
    Represents a compilation unit
    */
    class Unit : public Debugable {
    public:
        Unit(){}
        static Unit create(Target target){
            Unit u;
            u.m_target = target;
            return u;
        }
        
        Unit& from_file(std::string file){

           std::ifstream f;
            f.open(file);
            std::stringstream buffer;
            buffer << f.rdbuf();

            m_file = file;
            m_source = buffer.str();

            // load up file
            return *this;
        }

        Unit& from_source(std::string file, std::string_view source){
            m_file = file;
            m_source = source;
            return *this;
        }

        std::string debug() override {
            std::stringstream ss;
            ss << "target="<<m_target.debug()<< "\nfile="<<file()<<"\nsource="<<source()<<"\n";
            return ss.str();
        }

        std::string file(){
            return m_file;
        }
        std::string& source(){
            return m_source;
        }

        Target& target(){
            return m_target;
        }
    private:
        std::string m_file;
        std::string m_source;
        Target m_target;
    };
}