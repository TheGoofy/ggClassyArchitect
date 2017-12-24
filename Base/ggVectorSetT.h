#ifndef GGVECTORSET_H
#define GGVECTORSET_H

// 1) include system
#include <vector>
#include <set>
#include <map>

// 2) include own project-related (sort by component dependency)
// 3) forward declarations

/**
 * Access a std::set also like a std::vector (except "push_back").
 */
template <class TKey,
          class TCompare = std::less<TKey> >

class ggVectorSet :
  public std::set<TKey, TCompare>

{

  typedef std::set<TKey, TCompare> tBaseSet;
  typedef std::vector<typename tBaseSet::iterator> tVector;
  typedef std::map<TKey, typename tBaseSet::size_type, TCompare> tIndices;

public:

  ggVectorSet() :
    mValid(false) {
  }

  inline std::pair<typename tBaseSet::iterator, bool> insert(const typename tBaseSet::value_type& aValue) {
    mValid = false;
    return tBaseSet::insert(aValue);
  }

  template <class TIterator>
  inline void insert(TIterator aFirst, TIterator aLast) {
    mValid = false;
    return tBaseSet::insert(aFirst, aLast);
  }

  inline typename tBaseSet::iterator erase(typename tBaseSet::iterator aPos) {
    mValid = false;
    return tBaseSet::erase(aPos);
  }

  inline typename tBaseSet::size_type erase(const typename tBaseSet::key_type& aKey) {
    mValid = false;
    return tBaseSet::erase(aKey);
  }

  inline void clear() {
    mValid = false;
    tBaseSet::clear();
  }

  inline typename tBaseSet::size_type indexOf(const TKey& aKey) const {
    typename tIndices::const_iterator vIndicesIterator = Indices().find(aKey);
    if (vIndicesIterator != Indices().end()) return vIndicesIterator->second;
    else return tBaseSet::size();
  }

  inline typename tBaseSet::reference operator[] (typename tBaseSet::size_type aPos) {
    return *(Vector().operator [](aPos));
  }

  inline typename tBaseSet::const_reference operator[] (typename tBaseSet::size_type aPos) const {
    return *(Vector().operator [](aPos));
  }

private:

  inline void Update() const {
    mValid = true;
    mVector.resize(tBaseSet::size());
    mIndices.clear();
    typename tBaseSet::size_type vIndex = 0;
    typename tBaseSet::iterator vIterator = tBaseSet::begin();
    while (vIterator != tBaseSet::end()) {
      mVector[vIndex] = vIterator;
      mIndices[*vIterator] = vIndex;
      ++vIterator;
      ++vIndex;
    }
  }

  inline tVector& Vector() const {
    if (!mValid) Update();
    return mVector;
  }

  inline tIndices& Indices() const {
    if (!mValid) Update();
    return mIndices;
  }

  mutable bool mValid;
  mutable tVector mVector;
  mutable tIndices mIndices;

};

#endif // GGVECTORSET_H
