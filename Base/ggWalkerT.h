#ifndef GGWALKER_H
#define GGWALKER_H

// 1) include system
// 2) include own project-related (sort by component dependency)
// 3) forward declarations

/**
 * This class allows a compact way to build a loop using
 * iterators. It also helps to not forget calling "++" of the
 * iterator, because this is done internally while the condition
 * for the next iteration end is checked. See the following
 * example:
 *
 * <code>
 * std::vector<int> vVector;
 * ggWalker<std::vector<int>::iterator> vWalker(vVector);
 * int nSum = 0;
 * while (vWalker) {
 *   nSum += *vWalker;
 * }
 * </code>
 */
template <class TIterator>
class ggWalkerT
{

public:

  /**
   * Creates a new walker object. Iterates over all elements of a
   * constant container (read only).
   */
  template <typename TContainer>
  ggWalkerT(const TContainer& aContainer) :
    mBegin(aContainer.begin()),
    mEnd(aContainer.end()),
    mIterator(aContainer.begin()),
    mIteratorNext(aContainer.begin()) {
  }

  /**
   * Creates a new walker object. Iterates over all elements of a
   * non constant container (read and write allowed).
   */
  template <typename TContainer>
  ggWalkerT(TContainer& i_Container) :
    mBegin(i_Container.begin()),
    mEnd(i_Container.end()),
    mIterator(i_Container.begin()),
    mIteratorNext(i_Container.begin()) {
  }

  /**
   * Creates a new walker object.
   */
  ggWalkerT(const TIterator& i_Begin,
            const TIterator& i_End) :
    mBegin(i_Begin),
    mEnd(i_End),
    mIterator(i_Begin),
    mIteratorNext(i_Begin) {
  }

  /**
   * Returns "true", if there is an element available for
   * iteration. It internally advances already one step, but the
   * element accessed by "Get()" remains at the previous position.
   * The position of the element accessed by "Get()" is
   * incremented with the next call of "Walk()".
   */
  inline bool Walk() {
    if (mIteratorNext != mEnd) {
      mIterator = mIteratorNext;
      ++mIteratorNext;
      return true;
    }
    else {
      mIterator = mIteratorNext;
      return false;
    }
  }

  /**
   * Does the same as "Walk()".
   */
  inline operator bool() {
    return Walk();
  }

  /**
   * Returns the element for the current iteration.
   */
  inline typename TIterator::reference Get() const {
    return *mIterator;
  }

  /**
   * Does the same as "Get()" .
   */
  inline typename TIterator::reference operator * () const {
    return *mIterator;
  }

  /**
   * Does the same as "&Get()" .
   * TIterator::reference* should be TIterator::pointer, but it is not defined for some bpIterators
   */
  inline typename TIterator::pointer operator -> () const {
    return mIterator.operator -> ();
  }

  /**
   * Resets the walker to the initial position (new game - go back
   * to the start).
   */
  inline void Reset() {
    mIterator = mBegin;
    mIteratorNext = mBegin;
  }

  /**
   * return "true" if the iterator is at the beginning
   */
  inline bool IsFirst() const {
    return mIterator == mBegin;
  }

  /**
   * return "true" if the iterator is at the last position
   */
  inline bool IsLast() const {
    return mIteratorNext == mEnd;
  }

private:

  // begin of the iteration
  const TIterator mBegin;

  // end of the iteration
  const TIterator mEnd;

  // position of the current element
  TIterator mIterator;

  // position of the element after "m_Iterator"
  TIterator mIteratorNext;

};

#endif // GGWALKER_H
