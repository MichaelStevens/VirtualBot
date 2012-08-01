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

#include <QPainter>
#include <QStaticText>
#include <iostream>
#include "particlefilterviewer.h"



ParticleFilterViewer::ParticleFilterViewer(ParticleFilter* particleFilter, Map* map, QWidget* parent): QWidget(parent)
{
  this->particleFilter = particleFilter;
  this->map = map;
  
}

void ParticleFilterViewer::paintEvent(QPaintEvent* )
{
  QPen pen(Qt::black, 2, Qt::SolidLine);

  QPainter painter(this);
  
  double width = this->visibleRegion().boundingRect().width();
  double height = this->visibleRegion().boundingRect().height();
  
  double top = 20;
  double bottom = height - 20;
  
  //draw map
  painter.setPen(pen);
  double backer = width-20;
  painter.drawLine(backer, 20, backer, height - 20);
  double scalar = (height-40) / map->size;
  for(int i=0;i<map->parts.size();i++) {     
   painter.drawLine(backer - map->parts[i].y, top + map->parts[i].x0 * scalar, backer - map->parts[i].y, top + map->parts[i].x1 * scalar); 
   
   painter.drawLine(backer - map->parts[i].y, top + map->parts[i].x0 * scalar, backer, top + map->parts[i].x0 * scalar);
   
   painter.drawLine(backer - map->parts[i].y, top + map->parts[i].x1 * scalar, backer, top + map->parts[i].x1 * scalar);
    
  }
  //particles  

  painter.setPen( QPen( Qt::NoPen ) );
  double sum = 0;
  for(int i=0;i<particleFilter->pnum;i++) {
    painter.setBrush(QBrush(QColor(0,0,0)));
    painter.drawEllipse(backer - map->offset, top + particleFilter->p[i].x * scalar, 5, 5);
    
    
    sum += particleFilter->p[i].weight;
    
  }
  
  //best estimate
  Particle p = particleFilter->getWieghtedAverage();
  
  painter.setBrush(QBrush(QColor(255,0,0)));
  painter.drawEllipse(backer - map->offset, top + p.x * scalar, 5, 5);

  painter.setPen(QPen(QColor(0,0,0)));
  QString status = QString::number(p.x);
  
  QStaticText status_text(status);
  
  painter.drawStaticText(0,0, status_text);
  
  
  
}

