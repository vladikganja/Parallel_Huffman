#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Archiever.h"
#include "Decoder.h"

// THE STRUCTURE OF META

//////////////////////////////////////////////////////////
//                                                      //
//  [first byte of zip file] - block size (entire file) //
//  BLOCK STRUCTURE:                                    //
//  [0][1] - hash size (MAX SIZE: 320 bytes)            //
//  [2][3][4] - main size                               //
//  [5]    - main tail                                  //
//  [6]~[hash size] - hash_tree                         //
//  [hash size + 6]~[main size + 6] - encoded data      //
//                                                      //
//////////////////////////////////////////////////////////

class Parallel_Huffman {
public:
    static void archieve();
    static void unzip();
};

