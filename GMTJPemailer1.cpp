//A command line program used for sending emails.

//include statements
#include <iostream>
#include <gmime/gmime.h>
#include <glib-2.0/glib.h>
#include <curl/curl.h>

#include <string>
#include <cstring>
#include <getopt.h>  //used for arg parsing options
#include <cstdlib>
#include <fstream> //used for binary serialization
#include <chrono>
#include <ctime>
#include <iomanip>
#include <list>
#include <vector>
#include <sstream>



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
string to;
string cc;
string bcc;
string subject;
string message;
string attachment;
static smtpConfig smtpObj;

//print usage statement for help or incorrect options
void print_usage(ostream& os, int exit_code)
	{
		cout << "\nUsage : "<<program_name<< "  -t <to_email> -c <cc_email> -b <bcc_email> -s <subject> -m <message> -a <attachment_path>\n\n"
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
int mainCurl(void);
//string smtpAddress(smtpConfig);

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

    mainCurl();
	//cout statments for trouble shooting.
//	cout<<to<<endl;
//	cout<<cc<<endl;
//	cout<<subject<<endl;
//	cout<<message<<endl;
//    cout<<smtpObj.port<<endl;

	return 0;
}
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

        if(TRUE == g_regex_match(smtpRegex,mysmtp.name,G_REGEX_MATCH_ANCHORED,&match_info))
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

    checker = 1;
	GRegex* emailRegex;
	GMatchInfo *match_info2;
	//regex...https://www.wired.com/2008/08/four_regular_expressions_to_check_email_addresses/
	emailRegex = g_regex_new("([a-z0-9][-a-z0-9_+.]*[a-z0-9])@([a-z0-9][-a-z0-9.]*[a-z0-9]\\.(arpa|root|aero|biz|cat|com|coop|edu|gov|info|int|jobs|mil|mobi|museum|name|net|org|pro|tel|travel|ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|as|at|au|aw|ax|az|ba|bb|bd|be|bf|bg|bh|bi|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cu|cv|cx|cy|cz|de|dj|dk|dm|do|dz|ec|ee|eg|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kr|kw|ky|kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|me|mg|mh|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|mv|mw|mx|my|mz|na|nc|ne|nf|ng|ni|nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|sk|sl|sm|sn|so|sr|st|su|sv|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|um|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|yu|za|zm|zw)|([0-9]{1,3}\\.{3}[0-9]{1,3}))",G_REGEX_CASELESS,G_REGEX_MATCH_PARTIAL,NULL);
    //while loop to check and see if email address entered looks legit.
	while (checker == 1)
	{
        cout<<"Please enter the email address you will be sending from."<<endl;
        cin >>mysmtp.from_email_address;
        cout<<endl;
        cout<<"You entered: "<<mysmtp.from_email_address<<endl;

        if(TRUE == g_regex_match(emailRegex,mysmtp.from_email_address,G_REGEX_MATCH_ANCHORED,&match_info2))
        {
            g_print("Your email address looks good.\n");
            checker = 0;
        }
        else
        {
            g_print("That doesn't look like it will work. Let's try that again.\n");
        }
    }
	cout<<"Please enter the username you use to access your email account."<<endl;
	cin>>mysmtp.from_name;
    cout<<endl;

	cout<<"Please enter the password that you use to access your email account."<<endl;
	cin>>mysmtp.password;

	//test out statements
//	cout<<mysmtp.name<<endl;
//	cout<<mysmtp.port<<endl;
//	cout<<mysmtp.user_name<<endl;
//	cout<<mysmtp.from_email_address<<endl;
//	cout<<mysmtp.from_name<<endl;
//	cout<<mysmtp.password<<endl;
	return mysmtp;//mysmtp;
}
//THIS PAYLOAD NEEDS TO BE REPLACED BY GMIME header and parts...
//static const char *payload_text[] = {
//list<string> payload_text {
//  "Date: Sun, 7 Jan 2018 21:54:29 +1100\r\n",
//  "To: " + to + "\r\n"//+ "\r\n",
//  "From: " + smtpObj.from_email_address +" (Example User)\r\n",
//  "Cc: " + cc + " (Another example User)\r\n",
//  "Bcc:" + bcc + "blind copied user.\r\n",
//  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
//  "rfcpedant.example.org>\r\n",
//  subject,
//  "\r\n", /* empty line to divide headers from body, see RFC5322 */
//  "The body of the message starts here.\r\n",
//  "\r\n",
//  "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
//  "Check RFC5322.\r\n",
//  "\0"
//};
list<string> payload_text{
"Date: Sun, 7 Jan 2018 21:54:29 +1100\r\n",
  "To: ua0003@gmail.com \r\n"//+ "\r\n",
  "From: " + string(smtpObj.from_email_address) +" (Example User)\r\n",
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
  "rfcpedant.example.org>\r\n",
  subject,
  "\r\n", /* empty line to divide headers from body, see RFC5322 */
  "The body of the message starts here.\r\n",
  "\r\n",
  "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
  "Check RFC5322.\r\n",
  "\0"
};
struct upload_status {
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
//  struct upload_status *upload_ctx = (struct upload_status *)userp;
//  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }
//TODO need to determine len of payload...

//  data = payload_text[upload_ctx->lines_read];
//    vector<const char *> payloadText
//    {
//        begin(payload_text), end(payload_text)
//    };
//    vector<char> playloadText(payload_text.begin(),payload_text.end());

//    data = payloadText[upload_ctx->lines_read];
//  data = payload_text[upload_ctx->lines_read];
//  if(data) {
//    size_t len = strlen(data);
//    memcpy(ptr, data, len);
//    upload_ctx->lines_read++;
//
//    return len;
//  }

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
    string preformatted = "smtp://";
    string smtpName = string (smtpObj.name);
    string smtpPort = smtpObj.port;
    string formatted =  preformatted+smtpName+":"+smtpPort;
    string usrNam = smtpObj.user_name;
    string passWord = smtpObj.password;
    string fromEmail = smtpObj.from_email_address;


    /* Set username and password */
//    curl_easy_setopt(curl, CURLOPT_USERNAME, smtpObj.user_name);
//    curl_easy_setopt(curl, CURLOPT_PASSWORD, smtpObj.password);
    curl_easy_setopt(curl, CURLOPT_USERNAME, usrNam.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, passWord.c_str());

    /* This is the URL for your mailserver. Note the use of port 587 here,
     * instead of the normal SMTP port (25). Port 587 is commonly used for
     * secure mail submission (see RFC4403), but you should use whatever
     * matches your server configuration. */
     //smtp://user:password;options@mail.example.com
//     cout<<*formatSMTP<<endl;
//    curl_easy_setopt(curl, CURLOPT_URL,*formatSMTP);
    curl_easy_setopt(curl,CURLOPT_URL,formatted.c_str());
//      curl_easy_setopt(curl, CURLOPT_URL,"smtp://smtp.gmail.com:465");
//      curl_easy_setopt(curl, CURLOPT_URL,"smtp://smtp.gmail.com:587");
    /* In this example, we'll start with a plin text connection, and upgrade
     * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
     * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
     * will continue anyway - see the security discussion in the libcurl
     * tutorial for more details. */
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);


    /* Note that this option isn't strictly required, omitting it will result
     * in libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromEmail.c_str());

    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
       if(!to.empty())
        {
            recipients = curl_slist_append(recipients, to.c_str());
        }
        if(!cc.empty())
        {
            recipients = curl_slist_append(recipients, cc.c_str());
        }
        if(!bcc.empty())
        {
            recipients = curl_slist_append(recipients, bcc.c_str());
        }
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
    string dateString()
    {
        //get date for email
        // https://gist.github.com/CaptainJH/11208867
        string dateTime;
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        dateTime = ss.str();
        //http://www.cplusplus.com/forum/beginner/41434/
        string dayOfWeek;
        const string DAY[]={"Sun","Mon","Tue",
        "Wed","Thu","Fri","Sat"};

            time_t rawtime;
            tm * timeinfo;
            time(&rawtime);
            timeinfo=localtime(&rawtime);

        int wday=timeinfo->tm_wday;
        cout << "Today is: " << DAY[wday] << "\n" << endl;
        dayOfWeek = DAY[wday];
        //combine day of week with date and time
        string dateForEmail;
        dateForEmail = dayOfWeek + " " + dateTime;
        return dateForEmail;
    }
