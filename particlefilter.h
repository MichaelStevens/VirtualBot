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


#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H
#include "map.h"
#include <QVector2D>
struct Particle{
  double x, y, dir, weight, a;
  QVector2D p, p1;
};

class ParticleFilter
{
private:  
  Map* map;
  int circle(int, int);
  double gaussian(double, double, double);
  void normalize();  

public:
  Particle* p;
  int pnum, resample_pnum;
  double width, height, moveNoise, senseNoise, turnNoise;
  ParticleFilter(double, double, int, Map*);
  Particle getBestParticle();
  Particle getHighestParticleX();
  Particle getLowestParticleX();
  Particle getHighestParticleY();
  Particle getLowestParticleY();
  Particle getHighestParticleDir();
  Particle getLowestParticleDir();  
  Particle getAverageParticle();  
  Particle getWieghtedAverage();
  double ESS();
  void randomize();
  void sense(double, double);
  void move(double, double, bool reverse = false);
  void resample();
  void liuResample();
  void correctiveResample(ParticleFilter* filter);
  void setNoise(double, double, double);
  bool isLocalized();
  int getConfidence();
};

#endif // PARTICLEFILTER_H
