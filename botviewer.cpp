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


#include "botviewer.h"
#include <QApplication>
#include <QPainter>
#include <QStaticText>
#include <iostream>
#include "math.h"


BotViewer::BotViewer(Robot* robot, Map* map, QWidget* parent): QWidget(parent)
{
  this->robot = robot;
  this->map = map;
}


void BotViewer::paintEvent(QPaintEvent* )
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
  //painter.drawRect(left + robot->x * w_scalar - robot_w/2, top + robot->y * h_scalar - robot_h /2, robot_w, robot_h);
  
  
  
  //lazer
  pen = QPen(Qt::red, 2, Qt::SolidLine);
  painter.setPen(pen);
  double dist = sqrt(width * width + height * height) + 1;
  //deg 0
  QVector2D point;
  QVector2D dir(robot->x + cos(robot->dir) * dist, robot->y + cos(PI/2 - robot->dir) * dist);  
  QVector2D origin(robot->x, robot->y);  
  
   
  for(uint i=0; i<map->parts.size(); i++) {
    QVector2D inter;
    bool hit = intersects(origin, dir, map->parts[i].v0, map->parts[i].v1, inter);
    if(hit) {
      double new_dist = distance(origin, inter);
      if(new_dist < dist) {
      dist = new_dist;	  
      point = inter;
      }
    }
  }  
  painter.drawLine(left + origin.x() * w_scalar, top + origin.y() * h_scalar,
		   left + point.x() * w_scalar, top + point.y() * h_scalar);
  
  
  //deg 90
  dist = sqrt(width * width + height * height) + 1;
  dir = QVector2D(robot->x + cos(robot->dir + PI/2) * dist, robot->y + cos(robot->dir) * dist);  
    
  
   
  for(uint i=0; i<map->parts.size(); i++) {
    QVector2D inter;
    bool hit = intersects(origin, dir, map->parts[i].v0, map->parts[i].v1, inter);
    if(hit) {
      double new_dist = distance(origin, inter);
      if(new_dist < dist) {
      dist = new_dist;	  
      point = inter;
      }
    }
  }  
  painter.drawLine(left + origin.x() * w_scalar, top + origin.y() * h_scalar,
		   left + point.x() * w_scalar, top + point.y() * h_scalar);
  
  //deg 180
  dist = sqrt(width * width + height * height) + 1;
  dir = QVector2D(robot->x + cos(robot->dir + PI) * dist, robot->y + cos(robot->dir + PI / 2) * dist);  
    
  
   
  for(uint i=0; i<map->parts.size(); i++) {
    QVector2D inter;
    bool hit = intersects(origin, dir, map->parts[i].v0, map->parts[i].v1, inter);
    if(hit) {
      double new_dist = distance(origin, inter);
      if(new_dist < dist) {
      dist = new_dist;	  
      point = inter;
      }
    }
  }  
  painter.drawLine(left + origin.x() * w_scalar, top + origin.y() * h_scalar,
		   left + point.x() * w_scalar, top + point.y() * h_scalar);
  
  //deg 270
  dist = sqrt(width * width + height * height) + 1;
  dir = QVector2D(robot->x + cos(robot->dir + 3.0*PI/2.0) * dist, robot->y + cos(robot->dir + PI) * dist);  
    
  
   
  for(uint i=0; i<map->parts.size(); i++) {
    QVector2D inter;
    bool hit = intersects(origin, dir, map->parts[i].v0, map->parts[i].v1, inter);
    if(hit) {
      double new_dist = distance(origin, inter);
      if(new_dist < dist) {
      dist = new_dist;	  
      point = inter;
      }
    }
  }  
  painter.drawLine(left + origin.x() * w_scalar, top + origin.y() * h_scalar,
		   left + point.x() * w_scalar, top + point.y() * h_scalar);
  
  
  
           

        
  
    

  
}

