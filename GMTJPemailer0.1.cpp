//A command line program used for sending emails.

#include <iostream>
#include <regex>
#include <string>
#include <getopt.h>
#include <cstdlib>

using namespace std;
const char* program_name;
char* to;
char* cc;
char* bcc;
char* subject;
char* message;
char* attachment;

void print_usage(ostream& os, int exit_code) 
	{
		cout << "Usage : %s options\n" << program_name << endl;
		cout << " -h --help Display this usage information." << endl
		<< " -v --version Display the version of the application." << endl
		<< " -t --to Requires an argument, email address of recipient." << endl
		<< " -c --cc Requires an argument, email address to be CC'd." <<endl
		<< " -b --bcc Requires an argument, email address to be blind copied." <<endl
		<< " -s --subject Requires an argument, email subject. (enter in '' )" <<endl
		<< " -m --message Requires an argument, email text//body. (enter in '')" <<endl
		<< " -a --attachment Requires an argument, file path to attachment." <<endl
		<< " -f --configure used to setup initially. Will request SMTP server name, port number, email address, and password." <<endl;
		
		exit (exit_code);
	}

int main(int argc, char* argv[])
{
	int optionCount;
	const char* const short_options= "hvt:c:b:s:m:a:f";

	const struct option long_options[] = 
	{
		{ "help", 0, NULL, 'h' },
		{ "version", 0, NULL, 'v' },
		{ "to", 1, NULL, 't' },
		{ "cc", 0, NULL, 'c'},
		{ "bcc", 0, NULL, 'b'},
		{ "subject", 0, NULL, 's'},
		{ "message", 0, NULL, 'm'},
		{ "attachment", 0, NULL, 'a'},
		{ "configure", 0, NULL, 'f'},
		
		{ NULL, 0, NULL, 0 }
	}; 
	program_name = argv[0];

//	while(1) //currently works
	do
	{
		optionCount = getopt_long (argc,argv,short_options,long_options,NULL);
		
//		while(optionCount != -1) //never ends...
		switch (optionCount) 
		{
			cout<<optionCount;
			case 'h':
			print_usage(cout, 0);

			case 'v':
			cout<< "This is version 0.2 of GMTJPemailer."<<endl;
			break;
			
			
			case 't':
			to = optarg;
			break;

			case 'c':
			cc = optarg;
			break;
			
			case 'b':
			bcc = optarg;
			break;
			
			case 's':
			subject = optarg;
			break;	
			
			case 'm':
			message = optarg;
			break;
			
			case 'a':
			attachment = optarg;
			break;
//TODO: FIX CONFIG FILE CONFIGURATION			
			case 'f':
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
	cout<<to<<endl;
	return 0;
}
