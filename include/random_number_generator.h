#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <opencv2/opencv.hpp>

class Random
{
 private:
  static Random* singleton_instance_;
  static cv::RNG* rng_; // TODO Probably a memory leak

 public:
  static const long default_seed_;
  
 public:
  static Random& getInstance();
  void setSeed(long seed);
  int nextInt(int lower_bound, int upper_bound);
  double nextDouble();
    
 private:
 Random() {}
  Random(const Random& other);
  void operator=(const Random& other);
};

#endif
