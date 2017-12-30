//A command line program used for sending emails.


//include statements
#include <iostream>
// need to add gmime still
//#include "glib/*"
#include <string>
#include <getopt.h>  //used for arg parsing options
#include <cstdlib>
#include <fstream> //used for binary serialization 
//variables
using namespace std;
const char* program_name;
char* to;
char* cc;
char* bcc;
char* subject;
char* message;
char* attachment;
//structure to hold smtp information
struct smtpConfig
{
	string name;
	int port;
	string user_name;
	string password;
	string from_name;
	string from_email_address;
};
//print usage statement for help or incorrect option
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
	//setup short options and whether they take arguments
	const char* const short_options= "hvt:c:b:s:m:a:f";
	//setup long options for verbose 
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
//begin do - while loop
	do
	{
		optionCount = getopt_long (argc,argv,short_options,long_options,NULL);
		
		switch (optionCount) 
		{
			cout<<optionCount;
			case 'h':
			print_usage(cout, 0);

			case 'v':
			cout<< "This is version 1.0 of GMTJPemailer."<<endl;
			exit (0);
			
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
//CONFIG FUNC
int config();
{
	smtpConfig one {};
	cout<<"Please enter the SMTP address. i.e. smtp.gmail.com"<<endl;
	cin>> one.name;
	//cin.get(one.name,100).get();

	cout<<"Please enter the SMTP port number. i.e. 587"<<endl;
	cin >>one.port;
	cin.get();
	//cin.get(one.port,3).get();

	cout <<"Please enter your name. i.e. John Smith"<<endl;
	getline(cin,one.user_name);
	//cin.get(one.from_name,100).get();
	
	cout<<"Please enter the email address you will be sending from."<<endl;
	cin >>one.from_email_address;
	//cin.get(one.from_email_address).get();
	
	cout<<"Please enter the username you use to access your email account."<<endl;
	cin>>one.from_name;
	//cin.get(one.user_name).get();
	
	cout<<"Pleae enter the password youuse to access your email account."<<endl;
	cin>>one.password;
	
	//test out statements
	cout<<one.name<<endl;
	cout<<one.port<<endl;
	cout<<one.user_name<<endl;
	cout<<one.from_email_address<<endl;
	cout<<one.from_name<<endl;
	cout<<one.password<<endl;
}
	//cout statments for trouble shooting.
	cout<<to<<endl;
//	cout<<cc<<endl;
	cout<<subject<<endl;
	cout<<message<<endl;
	return 0;
}