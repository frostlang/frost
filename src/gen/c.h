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
        std::string src(){
            return m_src.str();
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
            m_blocks.push_back(Block("globals"));
            m_active = 0;
        }
        void emit(std::string src, u32 offset = 0){
            m_blocks[m_active+offset].emit(src);
        }
        std::vector<Block>& blocks(){
            return m_blocks;
        }
        s32 find(std::string id){
            s32 counter = 0;
            for(auto& block : m_blocks){
                dbg() << "finding... counter="<<counter<<", id=" << block.id() << "\n";
                if(block.id()==id)
                    return counter;
                counter++;
            }
            return -1;
        }
        u32 active(){
            return m_active;
        }

        u32 insert_block(s32 offset, std::string id=""){
            auto ofst = (s32)m_active+offset+1;
            dbg() << "m_active="<<(s32)m_active<<"\n";
            dbg() << "offset="<<(s32)offset<<"\n";
            dbg() << "ofst="  <<(s32)ofst<<"\n";
            if(ofst<0){
                ofst = 0;
            }
            m_blocks.insert(m_blocks.begin()+ofst, Block(id));
            m_active=ofst;
            return ofst;
        }

        u32 new_block(std::string id=""){
            m_blocks.push_back(Block(id));
            m_active++;
            return m_active;
        }
        void set_active(u32 active){
            m_active = active;
        }
        Block* global_block(){
            return &m_blocks[0];
        }
    private:
        u32 m_active = 0;
        std::vector<Block> m_blocks;
    };
}