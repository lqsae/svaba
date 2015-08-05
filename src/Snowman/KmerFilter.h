#ifndef SNOWMAN_KMER_FILTER
#define SNOWMAN_KMER_FILTER


#include "CorrectionThresholds.h"
#include "OverlapCommon.h"

#include <map>
#include <string>


#include "SnowTools/BamRead.h"

using SnowTools::BamRead;
using SnowTools::BamReadVector;

typedef std::map<std::string, int> KmerCountMap;

class KmerFilter {

 public:
  
  KmerFilter() : pBWT(nullptr), pSAf(nullptr) {}

    ~KmerFilter() { delete pBWT; delete pSAf; }

  int correctReads(BamReadVector& vec);
  
 private: 

  RLBWT* pBWT;
  SuffixArray* pSAf;

  int m_kmer_len = 31;

  bool attemptKmerCorrection(size_t i, size_t k_idx, size_t minCount, std::string& readSequence, BWTIndexSet& inds);

  void __makeIndex(BamReadVector& vec);

};

#endif