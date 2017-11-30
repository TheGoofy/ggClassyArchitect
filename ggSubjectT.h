#ifndef GGSUBJECTT_H
#define GGSUBJECTT_H

#include "ggSubject.h"

/**
 * This is a subject, which contains the associated data of interest. Any
 * type of data is supported (template argument "TValueType"). Notification
 * of observers needs to be called explicitly (except "SetValueAndNotify(...)").
 */
template <typename TValue>
class ggSubjectT : public ggSubject {

public:

  ggSubjectT(bool aNotify = true) :
    mNotify(aNotify) {
  }

  template <typename TOtherValue>
  ggSubjectT(const TOtherValue& aValue, bool aNotify = true) :
    mValue(aValue),
    mNotify(aNotify) {
  }

  template <typename TOtherValue>
  inline ggSubjectT& operator = (const TOtherValue& aValue) {
    mValue = aValue;
    if (mNotify) Notify();
    return *this;
  }

  virtual ~ggSubjectT() {
  }

  inline const TValue& GetValue() const {
    return mValue;
  }

  inline void SetValue(const TValue& aValue) {
    mValue = aValue;
    if (mNotify) Notify();
  }

  inline TValue& operator * () {
    return mValue;
  }

  inline const TValue& operator * () const {
    return mValue;
  }

  inline bool GetNotifyAssignment() const {
    return mNotify;
  }

  inline void SetNotifyAssignment(bool aNotify) {
    mNotify = aNotify;
  }

private:

  TValue mValue;
  bool mNotify;

};

/**
 * Some commonly useful subjects predefined...
 */
typedef ggSubjectT<char> ggSubjectChar;
typedef ggSubjectT<unsigned char> ggSubjectUChar;
typedef ggSubjectT<int> ggSubjectInt;
typedef ggSubjectT<unsigned int> ggSubjectUInt;
typedef ggSubjectT<long> ggSubjectLong;
typedef ggSubjectT<unsigned long> ggSubjectULong;
typedef ggSubjectT<float> ggSubjectFloat;
typedef ggSubjectT<double> ggSubjectDouble;
typedef ggSubjectT<bool> ggSubjectBool;
typedef ggSubjectT<std::string> ggSubjectString;

#endif // GGSUBJECTT_H
