#ifndef GGSUBJECTVALUET_H
#define GGSUBJECTVALUET_H

#include "ggSubject.h"

template <class TValue>

class ggSubjectValueT :
  public ggSubject,
  public TValue

{

public:

  ggSubjectValueT(bool aNotify = true) :
    ggSubject(), TValue(), mNotify(aNotify) {
  }

  template <typename TOtherValue>
  ggSubjectValueT(const TOtherValue& aOther, bool aNotify = true) :
    ggSubject(), TValue(aOther), mNotify(aNotify) {
  }

  ggSubjectValueT(const ggSubjectValueT& aOther, bool aNotify = true) :
    ggSubject(aOther), TValue(aOther), mNotify(aNotify) {
  }

  virtual ~ggSubjectValueT() {
  }

  void Set(const TValue& aValue) {
    TValue::operator = (aValue);
    if (mNotify) Notify();
  }

  inline ggSubjectValueT& operator = (const ggSubjectValueT& aOther) {
    ggSubject::operator = (aOther);
    TValue::operator = (aOther);
    mNotify = aOther.mNotify;
    if (mNotify) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator = (const TOtherValue& aOther) {
    TValue::operator = (aOther);
    if (mNotify) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator += (const TOtherValue& aOther) {
    TValue::operator += (aOther);
    if (mNotify) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator -= (const TOtherValue& aOther) {
    TValue::operator -= (aOther);
    if (mNotify) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator *= (const TOtherValue& aOther) {
    TValue::operator *= (aOther);
    if (mNotify) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator /= (const TOtherValue& aOther) {
    TValue::operator /= (aOther);
    if (mNotify) Notify();
    return *this;
  }

  inline bool GetNotifyAssignment() const {
    return mNotify;
  }

  inline void SetNotifyAssignment(bool aNotify) {
    mNotify = aNotify;
  }

private:

  bool mNotify;

};

#endif // GGSUBJECTVALUET_H
