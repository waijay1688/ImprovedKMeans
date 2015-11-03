What's Word Sense Disambiguation (WSD)?

"In computational linguistics, word-sense disambiguation (WSD) is an open problem of natural language processing and ontology. WSD is identifying which sense of a word (i.e. meaning) is used in a sentence, when the word has multiple meanings. The solution to this problem impacts other computer-related writing, such as discourse, improving relevance of search engines, anaphora resolution, coherence, inference et cetera." --cited from wikipedia.

What's K-means algorithm?

"K-means clustering is a method of vector quantization, originally from signal processing, that is popular for cluster analysis in data mining. k-means clustering aims to partition n observations into k clusters in which each observation belongs to the cluster with the nearest mean, serving as a prototype of the cluster. This results in a partitioning of the data space into Voronoi cells." --also cited from wikipedia.

This improved K-means algorithm achieves the clustering of word sense sample for future disambiguation. The traditional K-means algorithm has a O(NKTD) time complexity where N is the number of samplings, K is the number of clustering center, T is the number of iterations, and D is the dimension of the sampling vector.

We improve this algorithm in the following aspects:

First, we improve the selection of the initial clustering centers. In the traditional version, the intial centers are selected ramdonly. However, K-means algorithm is strongly influence by the intial centers. Intial centers selected by random may lead to bad results. In this improved version, the intial points are selected by a max-min distance algorithm, to fastly and efficiently get the best intial points.

Second, the cluster centers are updated every time after the iteration is finished, where in this improved version, the clusters are updated once a new sample is included, which enhance the astringency of the algorithm.

Third, in order to achieve the goal of "minimize the variance in clusters, maximize the varince between clusters", also taking the consideration that the intial K value, the clustering number, may not be the optimal, we build a mechanism to optimize the k value. When two clustering centers are too closed, we combine them. When the variance of a clustering is too large, we split it. 

