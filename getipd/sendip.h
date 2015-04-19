#ifndef __SEND_IP_H__
#define __SEND_IP_H__

#include <glib-2.0/glib.h>

int send_ip(
	GString * target_address,
	GString * username,
	GString * password,
	GString * ip_addr, 
	GString * response);

#endif