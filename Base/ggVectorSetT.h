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

class ggVectorSetT :
  public std::set<TKey, TCompare>

{

  typedef std::set<TKey, TCompare> tSet;
  typedef std::vector<const TKey*> tVector;
  typedef std::map<TKey, typename tSet::size_type, TCompare> tIndices;

public:

  ggVectorSetT() :
    mValid(false) {
  }

  inline std::pair<typename tSet::iterator, bool> insert(const typename tSet::value_type& aValue) {
    mValid = false;
    return tSet::insert(aValue);
  }

  template <class TIterator>
  inline void insert(TIterator aFirst, TIterator aLast) {
    mValid = false;
    return tSet::insert(aFirst, aLast);
  }

  inline typename tSet::iterator erase(typename tSet::iterator aPos) {
    mValid = false;
    return tSet::erase(aPos);
  }

  inline typename tSet::size_type erase(const typename tSet::key_type& aKey) {
    mValid = false;
    return tSet::erase(aKey);
  }

  inline void clear() {
    mValid = false;
    tSet::clear();
  }

  inline typename tSet::size_type indexOf(const TKey& aKey) const {
    typename tIndices::const_iterator vIndicesIterator = Indices().find(aKey);
    if (vIndicesIterator != Indices().end()) return vIndicesIterator->second;
    else return tSet::size();
  }

  inline typename tSet::reference operator[] (typename tSet::size_type aPos) {
    return *(Vector().operator [](aPos));
  }

  inline typename tSet::const_reference operator[] (typename tSet::size_type aPos) const {
    return *(Vector().operator [](aPos));
  }

private:

  inline void Update() const {
    mValid = true;
    mVector.resize(tSet::size());
    mIndices.clear();
    typename tSet::size_type vIndex = 0;
    typename tSet::iterator vIterator = tSet::begin();
    while (vIterator != tSet::end()) {
      mVector[vIndex] = &(*vIterator);
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
