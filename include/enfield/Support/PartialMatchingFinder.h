#ifndef __EFD_PARTIAL_MATCHING_FINDER_H__
#define __EFD_PARTIAL_MATCHING_FINDER_H__

#include <vector>

namespace efd {
    /// \brief Interface for implementing different algorithms for finding
    /// partial matchings (approximate subgraph isomorphism).
    class PartialMatchingFinder {
        public:
            /// \brief Returns a valid matching.
            /// Note that this is not necessairly an exact match (exact 
            /// subgraph isomorphism). That is because this is a NP-Complete
            /// problem.
            virtual std::vector<unsigned> find() = 0;
    };
}

#endif