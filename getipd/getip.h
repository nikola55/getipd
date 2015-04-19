#ifndef __GET_IP_H__
#define __GET_IP_H__

#include <stdio.h>
#include <stdlib.h>

#include <glib-2.0/glib.h>

int get_ip(GString * ip_addr, const char * url);

size_t write_callback(char *ptr, size_t size, size_t nmemb, GString * userdata);

#endif