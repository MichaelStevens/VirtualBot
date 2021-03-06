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
#include "math.h"
#include <math.h>



ParticleFilterViewer::ParticleFilterViewer(ParticleFilter* particleFilter, Map* map, Robot* robot, QWidget* parent): QWidget(parent)
{
  this->particleFilter = particleFilter;
  this->map = map;
  this->robot = robot;
  
}

void ParticleFilterViewer::mousePressEvent(QMouseEvent* mouse)
{
  particleFilter->liuResample();
  repaint();
}


void ParticleFilterViewer::paintEvent(QPaintEvent* )
{
  QPen pen(Qt::black, 2, Qt::SolidLine);

  QPainter painter(this);
  
  double width = this->visibleRegion().boundingRect().width();
  double height = this->visibleRegion().boundingRect().height();
  
  double top = 20;
  double bottom = height - 20;
  double left = 20;
  double right = width - 20;
  
  double w_scalar = (bottom - top) / map->width;
  double h_scalar = (right - left) / map->height;
  
  //draw map
  painter.setPen(pen);
  
  for(int i=0;i<map->parts.size();i++) {     
   painter.drawLine(left + map->parts[i].v0.x() * w_scalar , top +  map->parts[i].v0.y() * h_scalar,
		    left + map->parts[i].v1.x() * w_scalar, top + map->parts[i].v1.y() * h_scalar );    
  }
  
  //actual
  //robot  
  
  int robot_w = 15 * w_scalar;
  int robot_h = 10 * h_scalar;  
  pen = QPen(Qt::black, 2, Qt::SolidLine);
  painter.setPen(pen);
  painter.save();  
  painter.translate(left + robot->x * w_scalar, top + robot->y * h_scalar);
  painter.rotate(robot->dir * 180 / PI);
  painter.drawRect(-robot_w/2, -robot_h/2, robot_w, robot_h);
  
  painter.restore();
  //particles  

  painter.setPen( QPen( Qt::NoPen ) );
  
  for(int i=0;i<particleFilter->pnum;i++) {
    painter.setBrush(QBrush(QColor(0,0,0)));
    painter.drawEllipse(left + particleFilter->p[i].x * w_scalar, top + particleFilter->p[i].y * h_scalar, 5, 5);    
    painter.setPen(QPen(QColor(255,0,0)));
    
    //painter.drawLine(left + particleFilter->p[i].x * w_scalar, top + particleFilter->p[i].y * h_scalar, \
		      left + particleFilter->p[i].p.x() * w_scalar, top + particleFilter->p[i].p.y() * h_scalar);
    
    //painter.drawLine(left + particleFilter->p[i].x * w_scalar, top + particleFilter->p[i].y * h_scalar, \
		      left + particleFilter->p[i].p1.x() * w_scalar, top + particleFilter->p[i].p1.y() * h_scalar);
  
    
    
  } 
  
  //best estimate
  Particle p = particleFilter->getWieghtedAverage();
  
  painter.setBrush(QBrush(QColor(0,0,255)));
  painter.drawEllipse(left + p.x * w_scalar, top + p.y * h_scalar, 5, 5);
  
  

  painter.setPen(QPen(QColor(0,0,0)));
  double error = d_abs(p.x - robot->x) + d_abs(p.y - robot->y) + d_abs(p.dir - robot->dir);
  QString status = QString::number(error);
  
  QStaticText status_text(status);
  
  painter.drawStaticText(0,0, status_text);
  
  if(particleFilter->isLocalized()) {
   state_error.push_back(error);
   double mean = 0;
   int total = state_error.size();
   for(int i=0;i<total;i++) {
     mean += state_error[i];     
   }
   mean /= total;
   
  QString status = "State error:" + QString::number(mean);
  
  QStaticText status_text(status);
  
  painter.drawStaticText(0,20, status_text);
     
     
  
  } 
  
  
}

