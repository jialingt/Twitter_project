#ifndef TAG_DATA_H
#define TAG_DATA_H

#include <string>
#include <utility>

struct TagData {
  // key
  std::string tag;
  // value
  int num;
  TagData(std::string t, int occ) : tag(t), num(occ) {}
  bool operator<(const TagData& other) const{
    return num < other.num;
  }
  bool operator>(const TagData& other) const{
    return num > other.num;
  }

};

// Comparator based on num of occurrences (priority)
struct TagIntLess {
  bool operator()(const TagData& t1, const TagData& t2) const {
    return t1.num < t2.num;
  }
};

// Comparator based on num of occurrences (priority)
struct TagIntGreater {
  bool operator()(const TagData& t1, const TagData& t2) const {
    return t1.num > t2.num;
  }
};

// Comparator based on hashtag term itself (key)
struct TagStringEqual {
  bool operator()(const TagData& t1, const TagData& t2) const {
    return t1.tag == t2.tag;
  }
};


// Computes hash of hashtag term itself (key)
struct TagStringHasher {
  size_t operator()(const TagData& td) const
  {
    std::hash<std::string> h;
    return h(td.tag);
  }
};

#endif
