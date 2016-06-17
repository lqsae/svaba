#ifndef DISCORDANT_CLUSTER_H__
#define DISCORDANT_CLUSTER_H__

#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <unordered_map>

#include "SnowTools/BamRead.h"

namespace SnowTools 
{

  //class DiscordantCluster;
  
  /** Class to hold clusters of discordant reads */
  class DiscordantCluster 
  {

    friend struct BreakPoint;
    
  public:

    /** Create an empty cluster */
    DiscordantCluster() { 
      m_reg1 = GenomicRegion(); 
      m_reg2 = GenomicRegion(); 
      assert(m_reg1.isEmpty()); 
      ncount = 0; tcount = 0;
      mapq1 = -1; mapq2 = -1;
    }

    /** Make a cluster from a set of reads (pre-clustered) and look up a larger set to find 
     * their mates 
     * @param this_reads Pre-clustered set of discordant reads (but not their mates)
     * @param all_reads A pile of reads to search for mates
     */
    DiscordantCluster(const BamReadVector& this_reads, const BamReadVector& all_reads, int max_mapq_possible);
    
    /** Is this discordant cluster empty? */
    bool isEmpty() const;

    /** Return a string representing the output file header */
    static std::string header() { 
      return "chr1\tpos1\tstrand1\tchr2\tpos2\tstrand2\ttcount\tncount\ttcount_hq\tncount_hq\t\tmapq1\tmapq2\tcname\tregion_string\treads\tcompeting_id"; 
    }
    
    bool hasAssociatedAssemblyContig() const { return m_contig.length(); }

    void addMateReads(const BamReadVector& bav);
    
    /** Return the discordant cluster as a string with just coordinates */
    std::string toRegionString() const;
    
    /** Return the ID associated with this cluster */
    std::string ID() const { return m_id; } 
    
    /** Print this with region string and read counts and mapq */
    friend std::ostream& operator<<(std::ostream& out, const DiscordantCluster& dc);
    
    /** Return as a string for writing to a file */
    std::string toFileString(bool with_read_names = false) const;
    
    /** Sort by coordinate */
    bool operator < (const DiscordantCluster& b) const;

    /** Is this a valid cluster? */
    bool valid() const;

    static void __remove_singletons(BamReadClusterVector& b);

    static std::unordered_map<std::string, DiscordantCluster> clusterReads(const BamReadVector& bav, const GenomicRegion& interval, int max_mapq_possible, const std::unordered_map<std::string, int> * min_isize_for_disc);

    static bool __add_read_to_cluster(BamReadClusterVector &cvec, BamReadVector &clust, const BamRead &a, bool mate);

    static void __cluster_reads(const BamReadVector& brv, BamReadClusterVector& fwd, BamReadClusterVector& rev, int orientation);

    static void __cluster_mate_reads(BamReadClusterVector& brcv, BamReadClusterVector& fwd, BamReadClusterVector& rev);

    static void __convertToDiscordantCluster(std::unordered_map<std::string, DiscordantCluster> &dd, const BamReadClusterVector& cvec, const BamReadVector& bav, int max_mapq_possible);

    /** Query an interval against the two regions of the cluster. If the region overlaps
     * with one region, return the other region. This is useful for finding the partner 
     * region give a query region */
    GenomicRegion GetMateRegionOfOverlap(const GenomicRegion& gr) const; 

    int tcount = 0;
    int ncount = 0; 

    int tcount_hq = 0;
    int ncount_hq = 0;
    
    int max_possible_mapq = 0;

    std::unordered_map<std::string, int> counts; // supporting read counts per sample (e.g. t001 - 4, n001 - 6)

    std::unordered_map<std::string, BamRead> reads;
    std::unordered_map<std::string, BamRead> mates;

    std::string m_contig = "";

    double read_score = 0;
    double mate_score = 0;

    //int rp_orientation = -1; // FR, FF,  RR, RF
    
    GenomicRegion m_reg1;
    GenomicRegion m_reg2;

    int mapq1;
    int mapq2;

    std::string m_id_competing; // id of discordant cluster with same span, different strands

  private:    
    std::string m_id;

    // return the mean mapping quality for this cluster
    double __getMeanMapq(bool mate = false) const;
  };
  
  //! vector of AlignmentFragment objects
  typedef std::vector<DiscordantCluster> DiscordantClusterVector;
  
  //! Store a set of DiscordantCluster objects, indexed by the "id" field
  typedef std::unordered_map<std::string, DiscordantCluster> DiscordantClusterMap;

  
}

#endif
