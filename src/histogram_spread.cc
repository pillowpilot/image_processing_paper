#include <histogram_spread.h>
#include <iostream> // TODO Debug

HistogramSpread::HistogramSpread(): histogram_(256) // TODO Magic number
{
}

double HistogramSpread::calculateMetric(const cv::Mat image)
{
  buildHistogram(image);
  makeHistogramCumulative();
  normalizeCumulativeHistogram();
  
  const int first_quartile = getFirstQuartile();
  const int third_quartile = getThirdQuartile();
  const int possible_range = 255 - 0; // TODO Magic number

  double histogram_spread = (double)(third_quartile - first_quartile)/possible_range;
  if(histogram_spread < 0)
    histogram_spread = 0;

  {
    using namespace std;
    cout << "First Quartile: " << first_quartile << endl;
    cout << "Third Quartile: " << third_quartile << endl;
  }
  
  return histogram_spread;
}

void HistogramSpread::buildHistogram(const cv::Mat image)
{
  const int rows = image.rows;
  const int columns = image.cols;

  for(int row = 0; row < rows; row++)
    {
      for(int column = 0; column < columns; column++)
	{
	  const int intensity = image.at<uchar>(row, column);
	  histogram_[intensity]++;
	}
    }
}

void HistogramSpread::makeHistogramCumulative()
{
  for(int intensity = 1; intensity < histogram_.size(); intensity++)
    {
      histogram_[intensity] += histogram_[intensity-1];
    }
}

void HistogramSpread::normalizeCumulativeHistogram()
{
  const double maximum = histogram_[histogram_.size()-1];
  for(auto& frecuency: histogram_)
    {
      frecuency /= maximum;
    }
}

int HistogramSpread::getFirstQuartile() const
{
  int intensity = 0;
  while(histogram_.size() < intensity && histogram_[intensity] < 0.25)
    intensity++;

  return intensity;
}

int HistogramSpread::getThirdQuartile() const
{
  int intensity = histogram_.size()-1;
  while(0 <= intensity && histogram_[intensity] > 0.75)
    intensity--;

  return intensity;
}

