#pragma once

#include <string>
#include <optional>
#include <memory>
#include <fstream>
#include <functional>
#include <iterator>
#include <sstream>
#include <vector>
#include <pointcloud_raster/core/point.hpp>
#include <pointcloud_raster/core/bounding_box.hpp>
#include <pointcloud_raster/io/pointcloud_provider.hpp>

namespace pointcloud_raster::io
{

class TXTReader : public PointcloudProvider
{
public:
    TXTReader(const std::string filename,
              int skipLines = 0,
              char delimiter = ' ',
              std::function<Point(const std::string &line, const char &delimiter)> readLineLambda =
                  [](const std::string &line, const char &delimiter) -> Point {
                      std::istringstream iss(line);
                      std::vector<std::string> tokens;
                      {
                          std::string token;
                          while (std::getline(iss, token, delimiter))
                          {
                              tokens.push_back(token);
                          }
                      }

                      if (tokens.size() == 6)
                      {
                          return Point(
                              std::stod(tokens[0]),
                              std::stod(tokens[1]),
                              std::stod(tokens[2]),
                              static_cast<unsigned char>(std::stoi(tokens[3])),
                              static_cast<unsigned char>(std::stoi(tokens[4])),
                              static_cast<unsigned char>(std::stoi(tokens[5]))
                              );
                      }
                      else
                          throw std::runtime_error("Can't parse line");
    });

    /**
     * Open file handle and try to create LAS object
     * @return True is file could be opened
     */
    bool
    Open();

    /**
     * Move to first point
     * @return True is file could be opened
     */
    virtual bool
    SeekToFirstPoint();

    /**
     * Compute bounding box data from LAS header
     * @return BoundingBox<double> Rounded 3D bounding box
     */
    virtual bool
    ComputeBoundingBox();

    /**
     * Reads a point from las file
     * @return Read point or `nullopt` if end of file has been reached
     */
    std::optional<Point>
    GetNextPoint();

    ~TXTReader();

private:
    const std::string filename_;
    const int skipLines_;
    const char delimiter_;
    std::ifstream inputStream_;
    std::function<Point(const std::string &line, const char &delimiter)> readLineLambda_;
};

}
