#include <random_number_generator.h>

Random* Random::singleton_instance_ = nullptr;
cv::RNG* Random::rng_ = nullptr;
const long Random::default_seed_ = 0;

Random& Random::getInstance()
{
  if(Random::singleton_instance_ == nullptr)
    {
      singleton_instance_ = new Random();
      rng_ = new cv::RNG(default_seed_);
    }
  return *singleton_instance_;
}

void Random::setSeed(long new_seed)
{
  rng_ = new cv::RNG(new_seed);
}

int Random::nextInt(int lower_bound, int upper_bound)
{
  return rng_->uniform(lower_bound, upper_bound);
}

double Random::nextDouble()
{
  return rng_->uniform(0.0, 1.0);
}
