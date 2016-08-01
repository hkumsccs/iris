#include <curl/curl.h>  
#include <string>  
#include <exception>  
//#include "restclient-cpp/restclient.h"
  
// http://blog.csdn.net/dgyanyong/article/details/14166217
int main(int argc, char *argv[])   
{  
  //RestClient::Response r = RestClient::post("http://localhost/activate", "application/json", "{\"foo\": \"bla\"}")
  //RestClient::Response r = RestClient::post("http://url.com/post", "text/json", "{\"foo\": \"bla\"}");
    char  szJsonData[2048];  
    memset(szJsonData, 0,  sizeof (szJsonData));  
    std::string strJson =  "{" ;  
    strJson +=  "\"featureVector\" : \"[0, 1]\"" ;  
    strJson +=  "}" ;  
    strcpy(szJsonData, strJson.c_str());  
    try   
    {  
      CURL *pCurl = NULL;  
      CURLcode res;  
      // In windows, this will init the winsock stuff  
      curl_global_init(CURL_GLOBAL_ALL);  

      // get a curl handle  
      pCurl = curl_easy_init();  
      if  (NULL != pCurl)   
      {  
        curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 1);  

        // First set the URL that is about to receive our POST.   
        curl_easy_setopt(pCurl, CURLOPT_URL,  "http://localhost:3001/activate");  

        curl_slist *plist = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8" );  
        curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);  

        // Setup json data
        curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, szJsonData);  

        // Perform the request, res will get the return code   
        res = curl_easy_perform(pCurl);  
        // Check for errors  
        if(res != CURLE_OK)   
        {  
          printf( "curl_easy_perform() failed:%s\n" , curl_easy_strerror(res));  
        }  
        // always cleanup  
        curl_easy_cleanup(pCurl);  
      }  
      curl_global_cleanup();  
    }  
    catch std::exception &ex)  
    {  
      printf( "curl exception %s.\n" , ex.what());  
    }  
    return  0;  
}  

