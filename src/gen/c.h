#pragma once

#include <debug.h>

namespace Frost::Gen::C{

    class Expression{
    public:
    private:
    };
    class Block{
    public:
        void emit(std::string src){
            m_src << src;
        }
        void dump(){
            dbg() << m_src.str();
        }
    private:
        std::stringstream m_src;
    };
    class BuildContext{
    public:
        BuildContext(){
            m_blocks.push_back({});
            m_blocks.push_back({});
            m_active = &m_blocks[1];
        }
        void emit(std::string src){
            m_active->emit(src);
        }
        std::vector<Block>& blocks(){
            return m_blocks;
        }
        Block* active(){
            return m_active;
        }
        Block* new_block(){
            m_blocks.push_back({});
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