/******************************************************************************/
/*
* @file   inputparse.h
* @author Aditya Harsh
* @brief  Input parsing header.
*/
/******************************************************************************/

#include <vector>    /* std::vector                      */
#include <fstream>   /* std::ifstream                    */
#include <sstream>   /* std::string, std::isstringstream */
#include <iterator>  /* std::istream_iterator            */

/**
 * @brief Input parsing to allow for easy reading of files.
 * 
 */
namespace InputParse
{
    /**
     * @brief Returns a vector of vector of data.
     * 
     * @param file_name 
     */
    template <typename T>
    std::vector<std::vector<T>> Parse(const std::string& file_name)
    {
        // data to create
        std::vector<std::vector<T>> data;

        std::ifstream input(file_name);

        // make sure file is valid
        if (input.good())
        {
            std::string str;

            std::getline(input, str);

            auto cases = std::stoul(str); // get the total number of cases to account for

            data.reserve(cases);

            while (std::getline(input, str))
            {
                // ignore blanks
                if (str.empty())
                {
                    if (data.size() >= cases) break;

                    data.emplace_back(std::vector<T>());
                    continue;
                }

                std::istringstream is(str);

                // get the data on the line
                std::vector<T> line_data ((std::istream_iterator<T>(is)), std::istream_iterator<T>());
                
                data.back().insert(std::end(data.back()), std::cbegin(line_data), std::cend(line_data));
            }
        }

        return data;
    }
}
