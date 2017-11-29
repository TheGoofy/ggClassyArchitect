#ifndef GGSUBJECTVALUET_H
#define GGSUBJECTVALUET_H

template <class TValue>

class ggSubjectValueT :
  public ggSubject,
  public TValue

{

public:

  ggSubjectValueT(bool aNotifyAssignment = true) :
    ggSubject(), TValue(), mNotifyAssignment(aNotifyAssignment) {
  }

  template <typename TOtherValue>
  ggSubjectValueT(const TOtherValue& aOther, bool aNotifyAssignment = true) :
    ggSubject(), TValue(aOther), mNotifyAssignment(aNotifyAssignment) {
  }

  ggSubjectValueT(const ggSubjectValueT& aOther, bool aNotifyAssignment = true) :
    ggSubject(aOther), TValue(aOther), mNotifyAssignment(aNotifyAssignment) {
  }

  virtual ~ggSubjectValueT() {
  }

  void Set(const TValue& aValue) {
    TValue::operator = (aValue);
    if (mNotifyAssignment) Notify();
  }

  inline ggSubjectValueT& operator = (const ggSubjectValueT& aOther) {
    ggSubject::operator = (aOther);
    TValue::operator = (aOther);
    mNotifyAssignment = aOther.mNotifyAssignment;
    if (mNotifyAssignment) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator = (const TOtherValue& aOther) {
    TValue::operator = (aOther);
    if (mNotifyAssignment) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator += (const TOtherValue& aOther) {
    TValue::operator += (aOther);
    if (mNotifyAssignment) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator -= (const TOtherValue& aOther) {
    TValue::operator -= (aOther);
    if (mNotifyAssignment) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator *= (const TOtherValue& aOther) {
    TValue::operator *= (aOther);
    if (mNotifyAssignment) Notify();
    return *this;
  }

  template <typename TOtherValue>
  inline ggSubjectValueT& operator /= (const TOtherValue& aOther) {
    TValue::operator /= (aOther);
    if (mNotifyAssignment) Notify();
    return *this;
  }

  inline bool GetNotifyAssignment() const {
    return mNotifyAssignment;
  }

  inline void SetNotifyAssignment(bool aNotify) {
    mNotifyAssignment = aNotify;
  }

private:

  bool mNotifyAssignment;

};

#endif // GGSUBJECTVALUET_H
