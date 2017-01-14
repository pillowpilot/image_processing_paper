#ifndef _IMAGE_STREAMER_H_
#define _IMAGE_STREAMER_H_

#include <set>
#include <vector>
#include <fstream>
#include <cassert>
#include <boost/filesystem.hpp>

#include <iostream>

class ImageStreamer
{
 private:
  std::vector<boost::filesystem::path> images_;
  std::size_t index_;
  
 public:
  const std::set<std::string> image_extensions_{".tif", ".png", ".jpg"};
  
 public:
  ImageStreamer(const boost::filesystem::path root_directory);
  bool isEmpty() const;
  boost::filesystem::path next();

 private:
  void processDirectory(const boost::filesystem::path directory_path);
  void processFile(const boost::filesystem::path file_path);
  
};

#endif
