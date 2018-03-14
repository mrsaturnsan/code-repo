/******************************************************************************/
/*
* @file   inputparse.h
* @author Aditya Harsh
* @brief  Input parsing header (useful for online coding challenges).
*/
/******************************************************************************/

#include <vector>    /* std::vector                      */
#include <fstream>   /* std::ifstream                    */
#include <sstream>   /* std::string, std::isstringstream */
#include <iterator>  /* std::istream_iterator            */

/**
 * @brief Input parsing class to allow for easy reading of files (esp. coding challenge input ;) )
 * 
 */
namespace InputParse
{
    /**
     * @brief The type to store each input as.
     * 
     * @tparam T 
     */
    template <typename T>
    class Parser
    {
        public:
            /**
             * @brief Construct a new Parser object.
             * 
             * @param file_name 
             */
            Parser(const std::string& file_name)
            {
                // open file
                std::ifstream input(file_name);
                std::string str;

                // make sure file is valid
                if (input.good())
                {
                    // get the first line
                    std::getline(input, str);

                    auto cases = std::stoul(str); // get the total number of cases to account for

                    data_.reserve(cases); // reserve space in advance

                    while (std::getline(input, str))
                    {
                        // ignore blanks
                        if (str.empty())
                        {
                            // full
                            if (data_.size() >= cases) break;

                            // otherwise
                            data_.emplace_back(std::vector<T>());
                            continue;
                        }

                        // read all strings on the line
                        std::istringstream is(str);

                        // get the data on the line
                        std::vector<T> line_data ((std::istream_iterator<T>(is)), std::istream_iterator<T>());
                        
                        // store data
                        data_.back().insert(std::end(data_.back()), std::cbegin(line_data), std::cend(line_data));
                    }
                }
            }

            /**
             * @brief Get the data acquired from the input.
             * 
             * @return std::vector<std::vector<T>>& 
             */
            std::vector<std::vector<T>>& Get() noexcept
            {
                return data_;
            }

        private:
            // the raw data
            std::vector<std::vector<T>> data_;
    };
}
