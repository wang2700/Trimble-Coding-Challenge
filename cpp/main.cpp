#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <regex>
#include <vector>
#include <map>

std::string ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(" ");
  return (start == std::string::npos) ? "" : s.substr(start);
}

void toLowerCase(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

class Data
{
  public:
    std::string DoB;
    std::string name;
    std::string color;
    bool likePeas;
    std::string dogsCats;

    Data(std::string input) {
      // use regular expression to verify the inputs
      const char *regexStr[5] = {"[0-9]{4}-[0-9]{2}-[0-9]{2}", "[a-zA-Z ]+", "[a-zA-Z]{2,}", "[yn]", "dogs|cats|both"};
      size_t pos = 0;
      std::string field;
      std::string delimiter = ",";
      int count = 0;
      std::string fields[5] = {};
      while (count < 5)
      {
        // if string is empty, there missing fields
        if (!input.empty())
        {
          //get the next field in input string
          pos = input.find(delimiter);
          field = input.substr(0, pos);
          field = ltrim(field);
          // if the field match with the corresponding regex, remove the field from input string
          if (std::regex_match(field, std::regex(regexStr[count])))
          {
            // std::cout << field << std::endl;
            input.erase(0, pos + 1);
          } else {
            field = "N/A";
          }
        } else {
          field = "N/A";
        }
        fields[count] = field;
        count++;        
      }
      DoB = fields[0];
      name = fields[1];
      color = fields[2];
      likePeas = fields[3].compare("y") == 0;
      dogsCats = fields[4];
    }

    void printData()
    {
      std::cout << "-------------------------" << std::endl;
      std::cout << "Date of Birth: " << DoB << std::endl;
      std::cout << "Name: " << name << std::endl;
      std::cout << "Color: " << color << std::endl;
      std::cout << "Like Peas: " << (likePeas ? "Yes" : "No") << std::endl;
      std::cout << "Dogs or Cats: " << dogsCats << std::endl;
      std::cout << "-------------------------" << std::endl;
    }
};

int main(int argc, char **argv)
{
  // open the file
  std::ifstream file("../test.data");
  if (!file)
  {
    std::cerr << "Unable to open file test.data";
    exit(1);
  }
  std::string line;
  std::vector<Data> dataStore;
  std::map<std::string, int> dataLoc;
  int i = 0;
  // import file line by line
  while (std::getline(file, line))
  {
    line = ltrim(line);
    // if the line is not empty and does not start with #
    if (!line.empty() && line.at(0) != '#')
    {
      Data data = Data(line); // initialize a data object
      dataStore.push_back(data);  // store the data to the back of the vector
      std::string key = data.name;  
      toLowerCase(key); //convert the first letter to lower case for allow for capitalized and not capitalized name
      dataLoc[key] = i; //store the location of the data in the vector with the name as key
      i++;
    }
    
  }
  file.close();

  // print out all data sorted by name
  for (std::map<std::string, int>::iterator i = dataLoc.begin(); i != dataLoc.end(); i++)
  {
    dataStore.at(i->second).printData();
  }
  
  // print names that like peas
  std::cout << "-------------------------" << std::endl;
  std::cout << "Person that likes peas" << std::endl;
  for (std::map<std::string, int>::iterator i = dataLoc.begin(); i != dataLoc.end(); i++)
  {
    Data data = dataStore.at(i->second);
    if (data.likePeas)
    {
      std::cout << data.name << std::endl;
    }
  }
  std::cout << "-------------------------" << std::endl;

  // allows user to find infomation about specific user
  std::string input;
  while (true)
  {
    std::cout << "Please enter the name you want to find: " << std::endl;
    std::getline(std::cin, input);
    // allow for capitalized and not capitalized name
    toLowerCase(input);
    if (dataLoc.find(input) == dataLoc.end())
    {
      std::cerr << "Unfortunately, the person you are looking up is not in the database, please try again.\n" << std::endl;
      continue;
    } 
    
    dataStore.at(dataLoc[input]).printData();
  }
  return 0;
}
