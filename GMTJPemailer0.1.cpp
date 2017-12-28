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
		cout << "\nUsage : "<<program_name<< "-t <to_email> -c <cc_email> -b <bcc_email> -s <subject> -m <message> -a <attachment_path>\n\n"
		 <<endl<<program_name<< " -f <config_prompts>\n"<<endl<< program_name << endl<<endl;
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
		{ "help", 0, 0, 'h' },
		{ "version", 0, 0, 'v' },
		{ "to", 1, NULL, 't' },
		{ "cc", 2, NULL, 'c'},
		{ "bcc", 2, NULL, 'b'},
		{ "subject", 1, NULL, 's'},
		{ "message", 1, NULL, 'm'},
		{ "attachment", 2, NULL, 'a'},
		{ "configure", 0, 0, 'f'},
		
		{ NULL, 0, NULL, 0 }
	}; 
	program_name = argv[0];

	do
	{
		optionCount = getopt_long (argc,argv,short_options,long_options,NULL);
		
		switch (optionCount) 
		{
			cout<<optionCount;
			case 'h':
			print_usage(cout, 0);
			
			int exit_code2;
			case 'v':
			cout<< "This is version 0.2 of GMTJPemailer."<<endl;
			exit (exit_code2);
			
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
//	cout<<cc<<endl;
	cout<<subject<<endl;
	cout<<message<<endl;
	return 0;
}
