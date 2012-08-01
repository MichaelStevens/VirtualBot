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


#include "particlefilter.h"
#include <cstdlib>  // For srand() and rand()
#include "math.h"

int ParticleFilter::circle(int num, int length)
{
   while (num > length - 1) {
     num -= length;
   }
   while (num < 0) {
     num += length;
   }
   
   return num;  
}

double ParticleFilter::gaussian(double mu, double sigma, double x)
{
  return exp(-pow(mu - x, 2) / pow(sigma, 2) / 2.0) / sqrt(2.0 * M_PI * pow(sigma, 2));
}



ParticleFilter::ParticleFilter(double width, double height, int pnum, Map *map) 
{
    this->width = width;
    this->height = height;
    this->pnum = pnum;
    this->moveNoise = 0;
    this->senseNoise = 0;
    this->map = map;
    
    p = new Particle[pnum];


    for(int i=0; i<pnum; i++) {
	p[i].x = (double(rand()) / RAND_MAX) * width;
	p[i].y = (double(rand()) / RAND_MAX) * height;
	p[i].dir = (double(rand()) / RAND_MAX) * 2 * PI;
	p[i].weight = 1.0 / double(pnum);	
    }
}

Particle ParticleFilter::getBestParticle()
{
  Particle &best = p[0];

  for(int i=0; i<pnum; i++) {
      if(p[i].weight > best.weight) {
	  best = p[i];
      }
  }

  return best;

}

Particle ParticleFilter::getHighestParticle()
{
  Particle &best = p[0];

  for(int i=0; i<pnum; i++) {
      if(p[i].x > best.x) {
	  best = p[i];
      }
  }

  return best;
}
Particle ParticleFilter::getLowestParticle()
{
  Particle &best = p[0];

  for(int i=0; i<pnum; i++) {
      if(p[i].x < best.x) {
	  best = p[i];
      }
  }

  return best;
}

Particle ParticleFilter::getAverageParticle()
{
  Particle mean;
  mean.x = 0;
  mean.y = 0;
  mean.dir = 0;
  mean.weight = 0;
  for(int i=0;i<pnum;i++) {
      mean.x += p[i].x;
      mean.y += p[i].y;
      mean.dir += p[i].dir;
      mean.weight += p[i].weight;
  }
  mean.x /= pnum;
  mean.dir /= pnum;
  mean.weight /= pnum;

  return mean;
}

Particle ParticleFilter::getWieghtedAverage()
{
  double sum = 0;
  Particle mean;
  mean.x = 0;
  mean.y = 0;
  mean.dir = 0;
  mean.weight = 0;
  for(int i=0;i<pnum;i++) {
      mean.x += p[i].x * p[i].weight;
      mean.y += p[i].y * p[i].weight;
      mean.dir += p[i].dir * p[i].weight;
      mean.weight += p[i].weight * p[i].weight;
      sum += p[i].weight;
  }
  mean.x /= sum;
  mean.dir /= sum;
  mean.weight /= sum;

  return mean;
}


double ParticleFilter::ESS()
{
  double cv = 1.0/pnum;
  double sum = 0;
  for(int i=0;i<pnum;i++) {
      sum += pow(pnum * p[i].weight - 1, 2);
  }
  cv *= sum;

  return pnum / (1 + cv);

}

void ParticleFilter::sense(double Z) 
{
  for(int a=0; a<pnum; a++) {
      double dist = 10000;
      for(uint i=0; i<map->parts.size(); i++) {
	  if(p[a].x >= map->parts[i].x0 && p[a].x <= map->parts[i].x1) {
	      dist = map->offset - map->parts[i].y;
	      break;
	  }
      }
      p[a].weight *= gaussian(dist, senseNoise, Z);

  }

  //normalize
  normalize();
}

void ParticleFilter::normalize()
{
  double sum = 0;
  for(int i=0;i<pnum;i++) {
      sum += p[i].weight;
  }

  for(int i=0;i<pnum;i++) {
      p[i].weight /= sum;
  }

}


void ParticleFilter::move(double dist) 
{
  for(int i=0; i<pnum; i++) {
    if(p[i].dir == 1) {
	p[i].x += dist + randn_notrig(0, moveNoise);
    } else {
	p[i].x -= dist + randn_notrig(0, moveNoise);
    }
  }
}

void ParticleFilter::resample()
{
  Particle new_p[pnum];

  double B = 0;
  Particle best = getBestParticle();
  int index = (rand() % pnum);

  for(int i=0; i<pnum; i++) {
      B += (double(rand()) / RAND_MAX) * 2.0 * best.weight;
      while(B > p[index].weight) {
	  B -= p[index].weight;
	  index = circle(index + 1, pnum);
      }
      new_p[i].x     = p[index].x;
      new_p[i].dir = p[index].dir;
      new_p[i].weight  = p[index].weight;
  }


  for(int i=0;i<pnum;i++) {
      p[i].x = new_p[i].x;
      p[i].dir = new_p[i].dir;
      p[i].weight = new_p[i].weight;
  }
  
  normalize();
}

void ParticleFilter::setNoise(double moveNoise, double senseNoise)
{
  this->moveNoise  = moveNoise;
  this->senseNoise = senseNoise;
}









