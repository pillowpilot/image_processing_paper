#include <metric.h>

double Metric::getMetric(const cv::Mat image)
{
  return calculateMetric(image);
}
