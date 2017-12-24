//A command line program used for sending emails.

#include <iostream>
#include <regex>
#include <string>
#include "cxxopts.hpp"
#include <unistd.h>


cxxopts::Options options("GMTJPemailer", "A command line emailer.");
options.add_options()
	("v,version", "Version", cxxopts::value<std::string>()->default_value(True))
	("h,help", "Help", cxxopts::value<std::string>());
	
int main(int argc, char *argv[])
{
	using namespace std;
	
	cout << "Hello World" <<endl;
	cout << "This is my second line."<<endl;
	cout << "Here's a third line.";
	return 0;
}

