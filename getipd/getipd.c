#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <glib-2.0/glib.h>

#include "getip.h"
#include "sendip.h"

void parse_configuration(char file_name[], GHashTable * map);

void parse_content(gchar * content, GHashTable * map);

void read_file(char file_name[], gchar ** content);

void free_gstr(gpointer data);

void get_val(GHashTable * map, gchar * key, gchar ** value);

int main(int argc, char * argv[]) 
{
	if(argc != 2)
	{
		fprintf(stderr, "No configuration file supplied\n");
		exit(0);
	}
	
	GHashTable * map =  g_hash_table_new_full(g_str_hash, g_str_equal, free_gstr, free_gstr);
	
	parse_configuration(argv[1], map);
	
	gchar *srvc_url, *targ_addr, *usr, *pasw, *logf, *rep_intr;
	
	get_val(map, "service_url", &srvc_url);
	get_val(map, "target_address", &targ_addr);
	get_val(map, "username", &usr);
	get_val(map, "password", &pasw);
	get_val(map, "log_file", &logf);
	get_val(map, "report_interval", &rep_intr);
	
	int report_interval = atoi(rep_intr);
	
	if(report_interval < 10)
	{
		printf("Invalid report interval: %d\n", report_interval);
		exit(0);
	}
	
	GString * service_url = g_string_new(srvc_url);
	GString * target_address = g_string_new(targ_addr);
	GString * username = g_string_new(usr);
	GString * password = g_string_new(pasw);
	GString * log_file = g_string_new(logf);
	
	fprintf(stdout, "Starting getipd service\n");
	
	/* Our process ID and Session ID */
	pid_t pid, sid;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then
	we can exit the parent process. */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	/* Open any logs here */ 

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* Daemon-specific initialization goes here */

	/* The Big Loop */
	while (1) {
		
		GString * ip_addr = g_string_new(NULL);
		
		get_ip(ip_addr, service_url->str);
		
		GString * response = g_string_new(NULL);
		
		send_ip(target_address, username, password, ip_addr, response);
		
		response = g_string_append(response, "\n");
		
		int log_fd = open(log_file->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
		
		write(log_fd, response->str, response->len*sizeof(char));
		
		close(log_fd);
		
		g_string_free (response, TRUE);
		g_string_free (ip_addr, TRUE);
		
		sleep(report_interval);
		
	}
	
	g_hash_table_destroy(map);
	
	g_string_free(service_url, TRUE);
	g_string_free(target_address, TRUE);
	g_string_free(username, TRUE);
	g_string_free(password, TRUE);
	g_string_free(log_file, TRUE);
	
	exit(EXIT_SUCCESS);
}

void parse_configuration(char file_name[], GHashTable * map)
{
	
	gchar * content = NULL;
	
	read_file(file_name, &content);
	
	parse_content(content, map);
	
	free(content);
}

void get_val(GHashTable * map, gchar * key, gchar ** value)
{
	
	value[0] = g_hash_table_lookup(map, key);
	
	if(value[0] == NULL)
	{
		fprintf(stderr, "%s not found in the configuration file\n");
		exit(0);
	}
}

void read_file(char file_name[], gchar ** content)
{
	
	FILE * fd = fopen(file_name, "r");
	
	if(!fd)
	{
		fprintf(stderr, "%s", "Unable to open input file: %s\n", file_name);
		exit(0);
	}
	
	fseek(fd, 0L, SEEK_END);
	long length = ftell(fd);
	rewind(fd);
	
	content[0] = malloc((length+1)*sizeof(gchar));
	
	content[0][length] = '\0';
	
	fread(content[0], sizeof(gchar), length, fd);
	
	fclose(fd);
	
}
void free_gstr(gpointer data)
{
	free(data);
}

void parse_content(gchar * content, GHashTable * map)
{

	gchar ** lines = g_strsplit(content, "\n", -1);
	
	int i;
	
	for(i = 0 ; lines[i] != NULL ; i++)
	{
		
		gchar * current = lines[i];
		int len = strlen(current);
		
		if( len < 2 || g_str_has_prefix(current, "#"))
		{
			continue;
		}
		
		gchar **tokens = g_strsplit(current, "=", 2);
		
		if(g_strv_length(tokens) == 2)
		{
			g_hash_table_insert(map, tokens[0], tokens[1]);
			
		}
		
	}
	
	g_strfreev(lines);
	
}
