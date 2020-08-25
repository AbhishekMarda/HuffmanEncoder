//
//  main.cpp
//  huffmanencoder
//
//  Created by Abhishek Marda on 6/12/20.
//  Copyright © 2020 Abhishek Marda. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include "characternode.h"
using namespace std;
using frequency = int;

bool compareCharacterNodes(CharacterNode* first, CharacterNode* second);
string getEncodingForCharacter(const char character, CharacterNode* treeroot);
bool encoderHelper(const char character, string &encoding, CharacterNode *node);
void cleanupBinaryTree (CharacterNode* root);


int main(int argc, const char * argv[]) {

    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cout << "File not detected. Please try again."<<endl;
        return 1;
    }

    //getting the text of the input file into a string
    string line, input;
    getline(file,input);
    while (getline(file, line))
    {
        input+= '\n'+line;
    }

    file.close();

    //mapping every character to the number of times it occurs in the input file
    unordered_map<char, frequency> character_collection;

    int len = (int)input.length();
    for (int i=0; i<len; ++i)
    {
        ++character_collection[input[i]];
    }

    vector<CharacterNode*> characters;
    //create a vector of all the characters
    for (auto it = character_collection.begin(); it!= character_collection.end(); ++it)
        characters.push_back(new CharacterNode(it->first, it->second));

    sort(characters.begin(), characters.end(), compareCharacterNodes); //this orders in descending order, making it easier to pop from the back of the vector

    //create the binary tree according to the algorithm for huffman encoding
    while (characters.size()>1)
    {
        CharacterNode* newnode = new CharacterNode;
        newnode->m_left = characters[characters.size()-1];
        newnode->m_right = characters[characters.size()-2];
        newnode->m_frequency = newnode->m_left->m_frequency + newnode->m_right->m_frequency;
        characters.pop_back();
        characters.pop_back();
        characters.push_back(newnode);
        sort(characters.begin(), characters.end(), compareCharacterNodes);
    }

    CharacterNode* root = characters[0];

    //map each character to its encoding
    unordered_map<char, string> encoding;
    for(auto it = character_collection.begin(); it!= character_collection.end(); ++it)
    {
        encoding[it->first]=getEncodingForCharacter(it->first, root);
    }

    len = (int) input.length();
    string output;

    //get the binary output in a string
    for (int i=0; i<len; ++i)
        output+= encoding[input[i]];

    ofstream outputFile("output.dat");

    if (!outputFile.is_open())
    {
        cout<<"Problem with the output file. Please try again"<<endl;
        cleanupBinaryTree(root);
        return 1;
    }

    //output the character and it's corresponding encoding as the overhead of the file
    for (auto it= encoding.begin(); it!= encoding.end(); it++)
    {
        outputFile << it->first << " "<<it->second<<endl;
    }
    outputFile <<endl;

    outputFile<<endl<<(output.length()%8)<<endl<<endl; //this remainder term will be useful later

    len = (int) output.length();
    int i;
    //the lowest granularity of movement for output to a file is a byte, which is why I have to create the byte from the bit pattern and then output it
    int remainder = output.length()%8;
    for (i=0; i<8-remainder; i++)
    {
        output+='0';
    }
    len = (int) output.length();
    if (len%8!=0)
        cerr<<"We have a serious problem";

    
    //at this point len will be a multiple of 8
    
    for (i=0; i<len; i+=8)
    {
        unsigned char c=0;
        for (int j=0; j<7;j++)
        {
            switch (output[i+j])
            {
                case '1': c=++c<<1; break;
                case '0': c=c<<1; break;
                default: cerr<<"You shouldn't have reached here.";
            }
        }
        switch (output[i+7])
        {
            case '1': c=++c; break;
            case '0': break;
            default: cerr<<"You shouldn't have reached here.";
        }
        outputFile<<c;
    }
    
    
    outputFile.close();

    cleanupBinaryTree(root);
    
}

bool compareCharacterNodes(CharacterNode* first, CharacterNode* second)
{
    return (first->m_frequency > second->m_frequency);
}


string getEncodingForCharacter(const char character, CharacterNode* treeroot)
{
    string encoded;
    encoderHelper(character, encoded, treeroot);
    return encoded;
}

bool encoderHelper(const char character, string &encoding, CharacterNode *node)
{
    if (node==nullptr)
        return false;
    
    if (node->m_right == nullptr && node->m_left==nullptr && node->m_char!=character)
        return false;
    
    if (node->m_right == nullptr && node->m_left==nullptr && node->m_char==character)
        return true;
    
    if(encoderHelper(character, encoding, node->m_right))
    {
        encoding = '1'+encoding;
        return true;
    }
    
    if (encoderHelper(character, encoding, node->m_left))
    {
        encoding = '0'+encoding;
        return true;
    }
    
    return false;
}


void cleanupBinaryTree (CharacterNode* root)
{
    if (root == nullptr)
        return;
    cleanupBinaryTree(root->m_left);
    cleanupBinaryTree(root->m_right);
    delete root;
    return;
}
