#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <glib-2.0/glib.h>

#include "getip.h"

int send_ip(
	GString * target_address,
	GString * username,
	GString * password,
	GString * ip_addr, 
	GString * response)
{
	
	CURL *curl;
	CURLcode res;

	GString * post_fields = g_string_new(NULL);
	
	g_string_append(post_fields, "ip_address=");
	g_string_append(post_fields, ip_addr->str);
	
	GString * authentication = g_string_new(username->str);
	authentication = g_string_append(authentication, ":");
	authentication = g_string_append(authentication, password->str);
	
	/* In windows, this will init the winsock stuff */ 
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */ 
	curl = curl_easy_init();
	if(curl) {
		
		struct curl_slist *list = NULL;
		list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded");
		
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */ 
		
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_URL, target_address->str);
		
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
		curl_easy_setopt(curl, CURLOPT_USERPWD, authentication->str);
		
		/* Now specify the POST data */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields->str);
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
		
		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		
		/* Check for errors */ 
		if(res != CURLE_OK) {
			g_string_append(response, curl_easy_strerror(res));
		}
		
		/* always cleanup */ 
		curl_easy_cleanup(curl);
		
	}
	
	g_string_free (post_fields, TRUE);
	curl_global_cleanup();
	
	return 0;
	
}