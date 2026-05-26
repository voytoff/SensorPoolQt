#ifndef ISENSORCONVERTER_H
#define ISENSORCONVERTER_H

#include <QtPlugin>

class ISensorConverter {
public:
  virtual ~ISensorConverter() {} // Always include a virtual destructor
  virtual double convert(QByteArray data) = 0; // Pure virtual function
};

// Associate the class with a unique identifier
#define ISensorConverter_iid "com.sensorpull.ISensorConverter"
Q_DECLARE_INTERFACE(ISensorConverter, ISensorConverter_iid)

#endif // ISENSORCONVERTER_H
