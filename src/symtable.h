#pragma once

#include <string_view>
#include <map>
#include <vector>
#include <types.h>
#include <debug.h>
#include <type.h>

/*


SymTable<Opcode> sym_table;

// put x's location in the sym table
sym_table.put("x", Operand::create(Memory::create("rsi", -4)))


// entering a function
sym_table.enter_scope();

*/

//
// do we want the symtable to preserve data in scopes?
// we do if we want to pass it around, but we don't if its only ever traversed by 1 traverser...?
// for now lets not... preserve scope, this could be a bad idea
//

//
// TODO
// ye so for some reason maps don't work...
//
namespace Frost{
    template<typename T>
    class SymTable{
    public:
        class Scope{
        public:
            Scope(){
                m_data = std::map<std::string_view, T>();
            }
            std::map<std::string_view, T>& data(){
                return m_data;
            }
            std::map<std::string_view, T> m_data;
        };

        SymTable(){
            m_scope=0;
            m_scopes.push_back(Scope());
        }

        void put(std::string_view key, T value){
            m_scopes[m_scope].m_data.insert({key, value});
            dbg() << "put... " << s(key) << "\n";
            dbg() << value << "\n";
        }
        
        // TODO
        // we really want this to be Optional<T&> (Reference)
        // but the problem is if the T is null then we can't get a reference to it :(
        Optional<T> get(std::string_view key){
            for(u32 i=m_scope;i>=0;i--){
                if(m_scopes[i].data().count(key))
                    return m_scopes[i].data()[key];
            }
        }
        void enter_scope(){
            m_scope++;
            m_scopes.push_back(Scope());
        }
        void exit_scope(){
            m_scope--;
            m_scopes.pop_back();
        }
    private:
        u32 m_scope = {0};
        std::vector<Scope> m_scopes;
    };
}