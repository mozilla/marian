#include "sentences.h"

#include <algorithm>

namespace amunmt {

Sentences::Sentences()
  : maxLength_(0)
{}

Sentences::~Sentences()
{}

SentencePtr Sentences::at(size_t id) const {
  return coll_.at(id);
}

size_t Sentences::size() const {
  return coll_.size();
}

size_t Sentences::GetMaxLength() const {
  return maxLength_;
}

void Sentences::push_back(SentencePtr sentence) {
  const Words &words = sentence->GetWords(0);
  size_t len = words.size();
  if (len > maxLength_) {
    maxLength_ = len;
  }

  coll_.push_back(sentence);
}

class LengthOrderer {
 public:
  bool operator()(const SentencePtr& a, const SentencePtr& b) const {
    return a->GetWords(0).size() < b->GetWords(0).size();
  }
};

void Sentences::SortByLength() {
  std::sort(coll_.rbegin(), coll_.rend(), LengthOrderer());
  //std::sort(coll_.begin(), coll_.end(), LengthOrderer());
  //std::random_shuffle ( coll_.begin(), coll_.end() );
}

SentencesPtr Sentences::NextMiniBatch(size_t batchsize)
{
  SentencesPtr sentences(new Sentences());
  size_t startInd = (batchsize > size()) ? 0 : size() - batchsize;
  for (size_t i = startInd; i < size(); ++i) {
    SentencePtr sentence = at(i);
    sentences->push_back(sentence);
  }

  coll_.resize(startInd);
  return sentences;
}

}

