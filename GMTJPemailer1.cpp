//A command line program used for sending emails.


//include statements
#include <iostream>
#include <gmime/gmime.h>
#include <glib-2.0/glib.h>
#include <curl/curl.h>
#include <string.h>
#include <cstring>
#include <getopt.h>  //used for arg parsing options
#include <cstdlib>
#include <fstream> //used for binary serialization
#include <chrono>
#include <ctime>
#include <iomanip>


//structure to hold smtp information
struct smtpConfig
{
	char name[50];
	char port[5];
	char user_name[50];
	char password[50];
	char from_name[50];
	char from_email_address[60];
};

//variables
using namespace std;
const char* program_name;
char* to;
char* cc;
char* bcc;
char* subject;
char* message;
char* attachment;
static smtpConfig smtpObj;

//print usage statement for help or incorrect options
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
		<< " -f --configure Used to setup initially. Will request SMTP server name, port number, your name, email address, user name, and password." <<endl;

		exit (exit_code);
	}
//CONFIG FUNC
//function prototype
smtpConfig configSMTP();
//function definition
smtpConfig configSMTP()
{

	smtpConfig mysmtp {};
	int checker;
	checker = 1;
	GRegex* smtpRegex;
	GMatchInfo *match_info;
	//regex...use posix...no \w character!
	smtpRegex = g_regex_new("((smtp|mail)\\.[a-z.]+\\.(com|net))",G_REGEX_CASELESS,G_REGEX_MATCH_PARTIAL,NULL);
//while loop to check and see if smtp address entered correctly or with typo. i.e. smtp,gmail.com
	while (checker == 1)
	{
        cout<<"Please enter the SMTP address. i.e. smtp.gmail.com"<<endl;
        cin>> mysmtp.name;
        cout<<endl;
        cout<<"You entered: "<<mysmtp.name<<endl;
        //cout<<g_regex_match_full(smtpRegex,mysmtp.name,-1,0,G_REGEX_MATCH_ANCHORED,&match_info, NULL)<<endl;
        //cout<<g_match_info_is_partial_match(match_info)<<endl;
        //cout<<g_regex_match(smtpRegex,mysmtp.name,G_REGEX_MATCH_ANCHORED,&match_info)<<endl;

        if(TRUE == g_regex_match(smtpRegex,mysmtp.name,G_REGEX_MATCH_ANCHORED,&match_info))

        //if(TRUE == g_regex_match(smtpRegex,mysmtp.name,G_REGEX_MATCH_NOTEMPTY,NULL))
        {
        g_print("SMTP server address accepted.\n");
        checker = 0;
        }
        else
        {
        g_print("That doesn't look like it will work. Let's try that again.\n");
        }
	}

//get rest of data
//check to make sure that the port number is either 2 or 3 digits.
    checker = 1;
    while(checker ==1)
    {
        cout<<endl<<"Please enter the SMTP port number. i.e. 465"<<endl;
        cin >>mysmtp.port;
        cin.get();
        char * x {mysmtp.port};
        int len = strlen(x);
        //cout<<"X is "<<*x<<endl;
        //cout<<"Len is "<<len<<endl;
        //cout<<"The port number length is "<<len<<endl;
        if(len == 2||
            len == 3)

        {
            g_print("You entered a port number of acceptable length. Adding to SMTP object.\n\n");
            checker = 0;
        }
        else
        {
            g_print("That doesn't look like it will work. Let's try that again.\n\n");

        }
    }
    int arSize = 50;
	cout <<"Please enter your name. i.e. John Smith"<<endl;
	cin.getline(mysmtp.user_name,arSize);
	cout<<endl;

	cout<<"Please enter the email address you will be sending from."<<endl;
	cin >>mysmtp.from_email_address;
	cout<<endl;

	cout<<"Please enter the username you use to access your email account."<<endl;
	cin>>mysmtp.from_name;
    cout<<endl;

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
//smtpConfig theSMTPconfig;
            smtpObj = configSMTP();
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
    cout<<smtpObj.port<<endl;
    //get current date/time http://en.cppreference.com/w/cpp/chrono/time_point
    chrono::system_clock::time_point now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    cout<<put_time(localtime(&now_c), "%F %T")<<endl;
	return 0;
}

//#define FROM    smtpObj.name
//#define TO      "<addressee@example.net>"
#define CC      "<info@example.org>"

static const char *payload_text[] = {
  "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
  //"To: "
  string TO = "To: ";

  TO =string ("To: " + to + "\r\n")//+ "\r\n",
  "From: " && smtpObj.from_email_address &&" (Example User)\r\n",
  "Cc: " && cc &&" (Another example User)\r\n",
  "Bcc:" && bcc &&"blined copied user.\r\n",
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
  "rfcpedant.example.org>\r\n",
  subject,
  "\r\n", /* empty line to divide headers from body, see RFC5322 */
  "The body of the message starts here.\r\n",
  "\r\n",
  "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
  "Check RFC5322.\r\n",
  NULL
};

struct upload_status {
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = payload_text[upload_ctx->lines_read];

  if(data) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;

    return len;
  }

  return 0;
}

int mainCurl(void)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx;

  upload_ctx.lines_read = 0;

  curl = curl_easy_init();
  if(curl) {
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, smtpObj.user_name);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, smtpObj.password);

    /* This is the URL for your mailserver. Note the use of port 587 here,
     * instead of the normal SMTP port (25). Port 587 is commonly used for
     * secure mail submission (see RFC4403), but you should use whatever
     * matches your server configuration. */
    curl_easy_setopt(curl, CURLOPT_URL, smtpObj.name&&":"&&smtpObj.port);

    /* In this example, we'll start with a plain text connection, and upgrade
     * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
     * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
     * will continue anyway - see the security discussion in the libcurl
     * tutorial for more details. */
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

    /* If your server doesn't have a valid certificate, then you can disable
     * part of the Transport Layer Security protection by setting the
     * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
     *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
     *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
     * That is, in general, a bad idea. It is still better than sending your
     * authentication details in plain text though.  Instead, you should get
     * the issuer certificate (or the host certificate if the certificate is
     * self-signed) and add it to the set of certificates that are known to
     * libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See docs/SSLCERTS
     * for more information. */
    curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");

    /* Note that this option isn't strictly required, omitting it will result
     * in libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, smtpObj.from_email_address);

    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
    recipients = curl_slist_append(recipients, TO);
    recipients = curl_slist_append(recipients, CC);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    /* We're using a callback function to specify the payload (the headers and
     * body of the message). You could just use the CURLOPT_READDATA option to
     * specify a FILE pointer to read from. */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* Since the traffic will be encrypted, it is very useful to turn on debug
     * information within libcurl to see what is happening during the transfer.
     */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* Send the message */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* Free the list of recipients */
    curl_slist_free_all(recipients);

    /* Always cleanup */
    curl_easy_cleanup(curl);
  }

  return (int)res;
}
