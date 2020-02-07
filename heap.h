#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

using namespace std;

template <
         typename T,
         typename KComparator = std::equal_to<T>,
         typename PComparator = std::less<T>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const KComparator& kcomp = KComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item
    void push(const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey - Finds key matching old object &
    /// updates its location in the heap based on the new value
    void decreaseKey(const T& old, const T& newVal);
    void size() const;
    void print() const;
 private:
    /// Add whatever helper functions and data members you need below
    void trickleUp(const T& item, size_t loc);
    void heapify(size_t idx, size_t eff_size);
    void sort();

 private:
   /// Data members - These should be sufficient
    std::vector< T > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, KComparator> keyToLocation_;

};

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const KComparator& kcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, kcomp)

{

}

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::~Heap()
{

}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::push( const T& item)
{
    // If the heap is empty, just push back and update the hash table
    // if the item is already in the heap
    if(empty())
    {
        store_.push_back(item);
        keyToLocation_.insert(make_pair(item, 0));
    }
    else
    {
        if(keyToLocation_.find(item) != keyToLocation_.end())
        {
            return;
        }
        store_.push_back(item);
        trickleUp(item, store_.size()-1);
    }
    sort();
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::trickleUp(const T& item, size_t loc)
{
    size_t p = (loc - 1)/m_;
    while((p >= 0) && c_(store_[loc], store_[p]) )
    {
        // if it needs to swap
        (keyToLocation_.find(store_[p]))->second = loc;
        swap(store_[p], store_[loc]);
        // update the loc and parent in the unordered_map
        // prepare for the next iteration
        loc = p;
        if(loc >= 1)
        {
            p = (loc - 1)/m_;
        }
    }
    // if it find the proper location, just insert into the unordered_map
    keyToLocation_.insert(make_pair(item, loc));
}


// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::decreaseKey(const T& old, 
    const T& newVal)
{
    // get the position of the old value
    if(c_(newVal, old))
    {
        if(keyToLocation_.find(old) != keyToLocation_.end())
        {
            // update the value in the heap and the hash table
            size_t old_pos = keyToLocation_.find(old)->second;
            store_[old_pos] = newVal;
            size_t new_pos = old_pos;
            while(old_pos != 0)
            {
                if(c_(store_[old_pos], store_[old_pos-1]))
                {
                    keyToLocation_.find(store_[old_pos-1])->second = old_pos;
                    new_pos = old_pos-1;
                    swap(store_[old_pos],store_[old_pos-1]);
                }
                old_pos--;
            }
            keyToLocation_.erase(keyToLocation_.find(old));
            keyToLocation_.insert(make_pair(newVal, new_pos));
        }
        else
        {
            throw std::logic_error("Could not find such a item");
        }
        sort();
    }
    else
    {
        return;
    }
}

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
T const & Heap<T,KComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return store_[0];
}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::pop()
{
    //cout << "Pop the item " << endl;
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    else
    {
        keyToLocation_.erase(store_[0]);
        store_[0] = store_.back();
        store_.pop_back();
        heapify(0, 0);
    }
    sort();
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::heapify(size_t idx, size_t eff_size)
{
    size_t child = m_*idx + 1;
    //if it is a child
    if(child > (store_.size() - 1 - eff_size) || (int(store_.size()) - 1 - int(eff_size))<0)
    {
        return;
    }
    else
    {
        // if it has a left child, just start from the left
        size_t smaller_child = child;
        for(int i = 0; i < m_; i++)
        {
            // loop until the last child
            if((child + i) <= (store_.size() - size_t(1) - eff_size) )
            {
                if(c_( store_[child + i], store_[smaller_child]))
                {
                    smaller_child = child + i;

                }
            }
        }
        if(c_(store_[smaller_child], store_[idx]))
        {
            keyToLocation_.find(store_[smaller_child])->second = idx;
            keyToLocation_.find(store_[idx])->second = smaller_child;
            swap(store_[smaller_child], store_[idx]);
            heapify(smaller_child, eff_size);
        }
    }
}


/// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
bool Heap<T,KComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::sort()
{
    //cout << "Sort the item " << endl;
    if(store_.size() <= 1)
    {
        return;
    }
    size_t effsize = 0;
    heapify(0, effsize);
    while(effsize != store_.size() - 1)
    {
        keyToLocation_.find(store_[0])->second = store_.size()- 1 - effsize;
        keyToLocation_.find(store_[store_.size()- 1 - effsize])->second = 0;
        swap(store_[0], store_[store_.size()- 1 - effsize]);
        effsize++;
        heapify(0, effsize);
    }
    size_t i = 0;
    while(i < store_.size()/2)
    {
        keyToLocation_.find(store_[store_.size() - 1 - i])->second = i;
        keyToLocation_.find(store_[i])->second = store_.size() - 1 - i;
        swap(store_[store_.size() - 1 - i], store_[i]);
        i++;
    }

}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::size() const
{
        cout << store_.size() << " ";
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::print() const
{
    for(size_t i = 0; i < store_.size(); i++)
    {
        cout << store_[i] << " ";
    }
    cout << endl;
}
#endif