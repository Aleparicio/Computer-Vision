#include "Pair.h"

// Aplica el nearest neighbour ratio para los matches de nn_matches.
// Los matches que superan el test se almacenan en matched1 y matched2
void Pair::applyNNRatio(vector<vector<DMatch>>& nn_matches, float nn_match_ratio) {
    matched1.clear(), matched2.clear();
    for (size_t i = 0; i < nn_matches.size(); i++) {
        DMatch first = nn_matches[i][0];
        float dist1 = nn_matches[i][0].distance;
        float dist2 = nn_matches[i][1].distance;
        if (dist1 < nn_match_ratio * dist2) {
            matched1.push_back(kpts1[first.queryIdx]);
            matched2.push_back(kpts2[first.trainIdx]);
            matchIdx1.push_back(first.queryIdx);
            matchIdx2.push_back(first.trainIdx);
        }
    }
}
