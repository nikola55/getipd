
#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <glib-2.0/glib.h>


size_t write_callback(char *ptr, size_t size, size_t nmemb, GString * userdata)
{
	
	g_string_append(userdata, ptr);
	
	return size*nmemb;
	
}

int get_ip(GString * ip_addr, const char * url)
{
	
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	
	if(curl) {

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, ip_addr);
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		
  }
	
}