//A command line program used for sending emails.

#include <iostream>
#include <regex>
#include <string>
#include <getopt.h>
#include <cstdlib>

using namespace std;
const char* program_name;
char* to, cc, bcc, subject,message;

void print_usage(ostream& os, int exit_code) 
	{
		cout << "Usage : %s options [ inputfile ... ]\n" << program_name << endl;
		cout << " -h --help Display this usage information." << endl
		<< " -t --to Requires an argument as to what email address you would like ot send to." << endl
		<< " -v --version Display the version of the application." << endl;
		exit (exit_code);
	}

int main(int argc, char*argv[])
{
	int optionCount;
	const char* const short_options= "hvt:";

	const struct option long_options[] = 
	{
		{ "help", 0, NULL, 'h' },
		{ "version", 0, NULL, 'v' },
		{ "to", 1, NULL, 't' },
		{ NULL, 0, NULL, 0 }
	}; 
program_name = argv[0];

	do 
	{
		optionCount = getopt_long (argc,argv,short_options,long_options,NULL);

		switch (optionCount) 
		{
			case 'h':
			print_usage(cout, 0);

			case 'v':
			cout<< "This is version 0.2 of GMTJPemailer.";
			break;
			
			
			case 't':
			to = optarg;
			break;

			case '?':
			print_usage (cerr, 1);

			case -1:
			break;

			default:
			abort();
		}
	} 		
	while (optionCount != -1);
	cout<<to;
	return 0;
}
