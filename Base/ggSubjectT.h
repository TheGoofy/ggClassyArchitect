#ifndef GGSUBJECTT_H
#define GGSUBJECTT_H

// 1) include system

// 2) include own project-related (sort by component dependency)
#include "Base/ggSubject.h"
#include "Base/ggTypes.h"
#include "Base/ggString.h"

// 3) forward declarations

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
typedef ggSubjectT<ggChar> ggSubjectChar;
typedef ggSubjectT<ggInt8> ggSubjectInt8;
typedef ggSubjectT<ggInt16> ggSubjectInt16;
typedef ggSubjectT<ggInt32> ggSubjectInt32;
typedef ggSubjectT<ggInt64> ggSubjectInt64;
typedef ggSubjectT<ggSize> ggSubjectSize;
typedef ggSubjectT<ggUChar> ggSubjectUChar;
typedef ggSubjectT<ggUInt8> ggSubjectUInt8;
typedef ggSubjectT<ggUInt16> ggSubjectUInt16;
typedef ggSubjectT<ggUInt32> ggSubjectUInt32;
typedef ggSubjectT<ggUInt64> ggSubjectUInt64;
typedef ggSubjectT<ggUSize> ggSubjectUSize;
typedef ggSubjectT<ggFloat> ggSubjectFloat;
typedef ggSubjectT<ggDouble> ggSubjectDouble;
typedef ggSubjectT<QString> ggSubjectString;
typedef ggSubjectT<bool> ggSubjectBool;

#endif // GGSUBJECTT_H
