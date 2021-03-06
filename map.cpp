/*
    Copyright (c) 2012, <copyright holder> <email>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "map.h"
#include "math.h"

mapseg::mapseg(double x0, double y0, double x1, double y1)
{
  v0 = Vector2(x0, y0);
  v1 = Vector2(x1, y1);
}


Map::Map()
{
  width = 300;
  height = 300;
  
  parts.push_back(mapseg(0, 0, width - 1, 0));
  parts.push_back(mapseg(width - 1, 0, width - 1, height - 1));
  parts.push_back(mapseg(width - 1, height - 1, 0, height - 1));
  parts.push_back(mapseg(0, height - 1, 0, 0));
  
  parts.push_back(mapseg(50, 0 , 50, 100));
  parts.push_back(mapseg(50, 100 , 25, 100));
  parts.push_back(mapseg(25, 100 , 25, 0));
  
  parts.push_back(mapseg(150, 200 , 200, 150));
  
  parts.push_back(mapseg(50, 200 , 230, 20));
  
  parts.push_back(mapseg(230, 20 , 100, 100));
  
  tile_size = 5;
  rows = int(width / tile_size + 1);
  cols = int(height / tile_size + 1);
  
  descritized_map = new int*[rows];
  for(int i=0;i<rows;i++) {
   descritized_map[i] = new int[cols]; 
  }
  
  for(int y=0;y<cols;y++) {
   for(int x=0;x<rows;x++) {
     for(int i=0;i<parts.size();i++) {
       //if(intersectsRect(parts[i].v0, parts[i].v1, )
     }
   }    
  }
}

