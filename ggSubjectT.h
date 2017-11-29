#ifndef GGSUBJECTT_H
#define GGSUBJECTT_H

#include "ggSubject.h"

/**
 * This is a subject, which contains the associated data of interest. Any
 * type of data is supported (template argument "TValueType"). Notification
 * of observers needs to be called explicitly (except "SetValueAndNotify(...)").
 */
template <typename TValueType>
class ggSubjectT : public ggSubject {

public:

  ggSubjectT() {
  }

  virtual ~ggSubjectT() {
  }

  inline const TValueType& GetValue() const {
    return mValue;
  }

  inline void SetValue(const TValueType& aValue) {
    mValue = aValue;
  }

  inline void SetValueAndNotify(const TValueType& aValue) {
    mValue = aValue;
    Notify();
  }

  inline TValueType& operator * () {
    return mValue;
  }

  inline const TValueType& operator * () const {
    return mValue;
  }

private:

  TValueType mValue;

};

/**
 * Some commonly useful subjects predefined...
 */
typedef ggSubjectT<int> ggSubjectInt;
typedef ggSubjectT<unsigned int> ggSubjectUInt;
typedef ggSubjectT<long> ggSubjectLong;
typedef ggSubjectT<unsigned long> ggSubjectULong;
typedef ggSubjectT<float> ggSubjectFloat;
typedef ggSubjectT<double> ggSubjectDouble;
typedef ggSubjectT<std::string> ggSubjectString;

#endif // GGSUBJECTT_H
