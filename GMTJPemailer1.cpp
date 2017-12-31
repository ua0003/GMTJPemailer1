//A command line program used for sending emails.


//include statements
#include <iostream>
// need to add gmime still
#include <glib-2.0/glib.h>

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
	char * name = new char [50];
	int port;
	char * user_name = new char [50];
	char * password = new char [50];
	char * from_name = new char [50];
	char * from_email_address = new char [60];
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
//CONFIG FUNC
smtpConfig configSMTP()
{

	smtpConfig mysmtp {};
	int checker;
	checker = 1;
	GRegex* smtpRegex;

	GMatchInfo *match_info;
	//Gerror *error=NULL
	smtpRegex = g_regex_new("(smtp[a-z.]*[a-z.]{4})",G_REGEX_CASELESS,G_REGEX_MATCH_PARTIAL,NULL);

	while (checker == 1)
	{

        cout<<"Please enter the SMTP address. i.e. smtp.gmail.com"<<endl;
        cin>> mysmtp.name;
        cout<<"You entered: "<<mysmtp.name<<endl;
        //cout<<g_regex_match_full(smtpRegex,mysmtp.name,-1,0,G_REGEX_MATCH_ANCHORED,&match_info, NULL)<<endl;
        //cout<<g_match_info_is_partial_match(match_info)<<endl;
        //cout<<g_regex_match(smtpRegex,mysmtp.name,G_REGEX_MATCH_ANCHORED,&match_info)<<endl;


        if(TRUE == g_regex_match(smtpRegex,mysmtp.name,G_REGEX_MATCH_ANCHORED,&match_info))


        //if(TRUE == g_regex_match(smtpRegex,mysmtp.name,G_REGEX_MATCH_NOTEMPTY,NULL))
        {
        g_print("SMTP server address accepted.");
        checker = 0;
        }
        else
        {
        g_print("That doesn't look like it will work. Let's try that again.");
        }
	}

//get rest of data

	cout<<"Please enter the SMTP port number. i.e. 587"<<endl;
	cin >>mysmtp.port;
	cin.get();

    int arSize = 50;
	cout <<"Please enter your name. i.e. John Smith"<<endl;
	cin.getline(mysmtp.user_name,arSize);

	cout<<"Please enter the email address you will be sending from."<<endl;
	cin >>mysmtp.from_email_address;

	cout<<"Please enter the username you use to access your email account."<<endl;
	cin>>mysmtp.from_name;

	cout<<"Pleae enter the password youuse to access your email account."<<endl;
	cin>>mysmtp.password;

	//test out statements
	cout<<mysmtp.name<<endl;
	cout<<mysmtp.port<<endl;
	cout<<mysmtp.user_name<<endl;
	cout<<mysmtp.from_email_address<<endl;
	cout<<mysmtp.from_name<<endl;
	cout<<mysmtp.password<<endl;
	return mysmtp;//mysmtp;
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

			case 'f':
			configSMTP();
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


	//cout statments for trouble shooting.
	cout<<to<<endl;
//	cout<<cc<<endl;
	cout<<subject<<endl;
	cout<<message<<endl;
	return 0;
}

