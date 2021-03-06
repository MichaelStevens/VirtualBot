#include "VirtualBot.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QHBoxLayout>
#include <QKeyEvent>
#include "math.h"
#include <iostream>



VirtualBot::VirtualBot()
{
  start_resample = false;
  loop = 0;
  robot = new Robot();
  last_x = robot->x;
  last_y = robot->y;
  last_dir = robot->dir;
  map = new Map();
  
  particleFilter = new ParticleFilter(map->width, map->height, 30000, map);
  particleFilter->setNoise(robot->move_noise, robot->sense_noise, robot->turn_noise);
  //correctiveFilter = new ParticleFilter(map->width, map->height, 10000, map);
  //correctiveFilter->setNoise(robot->move_noise, robot->sense_noise, robot->turn_noise);
  
  viewer = new BotViewer(robot, map);
  partViewer = new ParticleFilterViewer(particleFilter, map, robot);  
  
  QHBoxLayout *hbox = new QHBoxLayout(this);  

  hbox->addWidget(viewer);
  hbox->addWidget(partViewer);
  
  updater = new QTimer(this);
  connect(updater, SIGNAL(timeout()), this, SLOT(updateFilter()));
  
  updater->start(100);
  Vector2 vec;
}


void VirtualBot::updateFilter()
{
  double dist = sqrt(map->width * map->width + map->height * map->height) + 1;  
  Vector2 dir(robot->x + cos(robot->dir) * dist, robot->y + cos(PI/2 - robot->dir) * dist);
  
  
  
  
  Vector2 origin(robot->x, robot->y);
  
  for(uint i=0; i<map->parts.size(); i++) {
    Vector2 inter;
    bool hit = intersects(origin, dir, map->parts[i].v0, map->parts[i].v1, inter);
    if(hit) {
      double new_dist = distance(origin, inter);
      if(new_dist < dist) {
      dist = new_dist;	  
      }
    }
  }
  
  double dist2 = sqrt(map->width * map->width + map->height * map->height) + 1;
  dir = Vector2(robot->x + cos(robot->dir + PI/2.0) * dist2, robot->y + cos(robot->dir) * dist2);
  
    
  for(uint i=0; i<map->parts.size(); i++) {
    Vector2 inter;
    bool hit = intersects(origin, dir, map->parts[i].v0, map->parts[i].v1, inter);
    if(hit) {
      double new_dist = distance(origin, inter);
      if(new_dist < dist2) {
      dist2 = new_dist;	  
      }
    }
  }
  
  
  double dist3 = sqrt(map->width * map->width + map->height * map->height) + 1;
  dir = Vector2(robot->x + cos(robot->dir + PI) * dist3, robot->y + cos(robot->dir + PI / 2) * dist3);
  
    
  for(uint i=0; i<map->parts.size(); i++) {
    Vector2 inter;
    bool hit = intersects(origin, dir, map->parts[i].v0, map->parts[i].v1, inter);
    if(hit) {
      double new_dist = distance(origin, inter);
      if(new_dist < dist3) {
      dist3 = new_dist;	  
      }
    }
  }
  
  double dist4 = sqrt(map->width * map->width + map->height * map->height) + 1;
  dir =Vector2(robot->x + cos(robot->dir + 3.0*PI/2.0) * dist4, robot->y + cos(robot->dir + PI) * dist4);
  
    
  for(uint i=0; i<map->parts.size(); i++) {
    Vector2 inter;
    bool hit = intersects(origin, dir, map->parts[i].v0, map->parts[i].v1, inter);
    if(hit) {
      double new_dist = distance(origin, inter);
      if(new_dist < dist4) {
      dist4 = new_dist;	  
      }
    }
  }
  
  
  bool reverse = false;
  
  if(robot->x - last_x < 0 || robot->y - last_y < 0) {
  // reverse = false; 
  }
  
  
  
 
  particleFilter->move(distance(robot->x, robot->y, last_x, last_y) + randn_notrig(0, robot->move_noise), 
		       robot->dir - last_dir, reverse);
  
  //correctiveFilter->move(distance(robot->x, robot->y, last_x, last_y) + randn_notrig(0, robot->move_noise), \
		       robot->dir - last_dir, reverse);
  //std::cout << robot->dir - last_dir <<", " <<robot->dir <<std::endl;
  last_x = robot->x;
  last_y = robot->y;
  last_dir = robot->dir;
  
  
  
  particleFilter->sense(dist + randn_notrig(0, robot->sense_noise), 0  + randn_notrig(0, robot->turn_noise));
  particleFilter->sense(dist2 + randn_notrig(0, robot->sense_noise), PI / 2.0 + randn_notrig(0, robot->turn_noise));
  particleFilter->sense(dist3 + randn_notrig(0, robot->sense_noise), PI + randn_notrig(0, robot->turn_noise));
  particleFilter->sense(dist4 + randn_notrig(0, robot->sense_noise), 3.0*PI / 2.0 + randn_notrig(0, robot->turn_noise));
  
  //correctiveFilter->sense(dist + randn_notrig(0, robot->sense_noise), 0  + randn_notrig(0, robot->turn_noise));
  //correctiveFilter->sense(dist2 + randn_notrig(0, robot->sense_noise), PI / 2.0 + randn_notrig(0, robot->turn_noise));
  //correctiveFilter->sense(dist3 + randn_notrig(0, robot->sense_noise), PI + randn_notrig(0, robot->turn_noise));
  //correctiveFilter->sense(dist4 + randn_notrig(0, robot->sense_noise), 3.0*PI / 2.0 + randn_notrig(0, robot->turn_noise));
  
  
  
  if(particleFilter->ESS() < particleFilter->pnum * 0.0004) {
    std::cout<<"est: "<<particleFilter->getBestParticle().weight<<std::endl;
    particleFilter->liuResample();
    std::cout << particleFilter->ESS() << std::endl;
   // std::cout << particleFilter->pnum << std::endl;    
  }
  //if(correctiveFilter->ESS() < correctiveFilter->pnum * 0.0004) {
  // correctiveFilter->liuResample(); 
  //}
  //if(correctiveFilter->isLocalized()) {
  //  particleFilter->correctiveResample(correctiveFilter);
  // correctiveFilter->randomize();
  //  std::cout<<"corrected"<<std::endl;
  //}
  
  std::cout << particleFilter->isLocalized() << std::endl;
  
  
  if(is_nan(particleFilter->ESS())) {
    particleFilter->randomize();    
  }
  
  
  
  partViewer->repaint();
  
  
  
  loop++;
  
 // std::cout << loop << std::endl;
  
  
}

void VirtualBot::keyPressEvent(QKeyEvent* key)
{
  switch(key->key()) {
    case Qt::Key_Up:
      robot->x += cos(robot->dir);
      robot->y += cos(PI / 2.0 - robot->dir);
      break;
    case Qt::Key_Down:
      robot->x -= cos(robot->dir);
      robot->y -= cos(PI / 2.0 - robot->dir);  
      break;
    case Qt::Key_Left:
      robot->dir -= PI / 100.0;
      break;
    case Qt::Key_Right:
      robot->dir += PI / 100.0;
      break;
    case Qt::Key_Escape:
      this->close();
      break;
    case Qt::Key_Space:
      updateFilter();
      break;
    defualt:
      
      break;   
  }
  viewer->repaint();
  
  
}


VirtualBot::~VirtualBot()
{}

#include "VirtualBot.moc"
