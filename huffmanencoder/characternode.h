//
//  characternode.hpp
//  huffmanencoder
//
//  Created by Abhishek Marda on 6/12/20.
//  Copyright © 2020 Abhishek Marda. All rights reserved.
//

#ifndef characternode_h
#define characternode_h


class CharacterNode
{
public:
    CharacterNode():m_char(0), m_frequency(0), m_left(nullptr), m_right(nullptr){}
    CharacterNode(char c, int freq): m_char(c), m_frequency(freq), m_left(nullptr), m_right(nullptr){}
    char m_char;
    int m_frequency;
    CharacterNode* m_left;
    CharacterNode* m_right;
};


#endif /* characternode_h */
