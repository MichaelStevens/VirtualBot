#ifndef VirtualBot_H
#define VirtualBot_H

#include <QtGui/QMainWindow>
#include <QApplication>
#include <QTimer>
#include <QButtonGroup>
#include "botviewer.h"
#include "robot.h"
#include "map.h"
#include "particlefilter.h"
#include "particlefilterviewer.h"



class VirtualBot : public QWidget
{
Q_OBJECT;
public:
   VirtualBot();
   virtual ~VirtualBot();
protected:
  virtual void keyPressEvent(QKeyEvent* );
  
private slots:
  void updateFilter();
private:
  bool start_resample;
  double last_x, last_y, last_dir, loop;
  BotViewer* viewer;
  ParticleFilterViewer* partViewer;
  Robot* robot;
  Map* map;
  ParticleFilter* particleFilter;
  ParticleFilter* correctiveFilter;
  QTimer* updater;
  
  
  
};

#endif // VirtualBot_H
