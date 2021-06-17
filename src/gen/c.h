#pragma once

#include <debug.h>

namespace Frost::Gen::C{

    class Expression{
    public:
    private:
    };
    class Block{
    public:
        Block(){}
        Block(std::string id) : m_id(id){}
        void emit(std::string src){
            m_src << src;
        }
        void dump(){
            dbg() << m_src.str();
        }
        std::string& id(){
            return m_id;
        }
    private:
        std::string m_id;
        std::stringstream m_src;
    };
    class BuildContext{
    public:
        BuildContext(){
            m_blocks.push_back(Block("global"));
            m_blocks.push_back(Block("um..."));
            m_active = &m_blocks[1];
        }
        void emit(std::string src){
            m_active->emit(src);
        }
        std::vector<Block>& blocks(){
            return m_blocks;
        }
        Optional<Block*> find(std::string id){
            for(auto& block : m_blocks)
                if(block.id()==id)
                    return &block;
            return Optional<Block*>();
        }
        Block* active(){
            return m_active;
        }
        Block* new_block(std::string id=""){
            m_blocks.push_back(Block(id));
            return &m_blocks[m_blocks.size()-1];
        }
        Block* set_block(Block* block){
            m_active = block;
            return block;
        }
        Block* global_block(){
            return &m_blocks[0];
        }
    private:
        Block* m_active;
        std::vector<Block> m_blocks;
    };
}