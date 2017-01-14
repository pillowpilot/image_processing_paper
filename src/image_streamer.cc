#include <image_streamer.h>

ImageStreamer::ImageStreamer(const boost::filesystem::path root_directory):
  images_(), index_(0)
{
  // TODO Exception if root_directory is not valid
  processDirectory(root_directory);
}

void ImageStreamer::processDirectory(const boost::filesystem::path directory_path)
{
  using namespace boost::filesystem;

  for(directory_entry& entry: directory_iterator(directory_path))
    {
      if(is_directory(entry))
	processDirectory(entry.path());
      else
	processFile(entry.path());
    }
}

void ImageStreamer::processFile(const boost::filesystem::path file_path)
{
  using namespace std;

  auto search_result = find(begin(image_extensions_), end(image_extensions_), file_path.extension());
  if(search_result != end(image_extensions_))
    images_.push_back(file_path);
}

bool ImageStreamer::isEmpty() const
{
  return index_ == images_.size();
}

boost::filesystem::path ImageStreamer::next()
{
  assert(!isEmpty());
  // TODO Add exception
  return images_[index_++];
}
  
