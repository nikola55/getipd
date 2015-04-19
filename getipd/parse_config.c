
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib-2.0/glib.h>
// gchar **
// g_strsplit (const gchar *string,
            // const gchar *delimiter,
            // gint max_tokens)

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
			
int main(int argc, char * argv[])
{
	
	if(argc != 2) 
	{
		fprintf(stderr, "%s", "No config file supplied\n");
		exit(0);
	}

	gchar * content = NULL;
	
	read_file(argv[1], &content);
	
	GHashTable * map =  g_hash_table_new_full(g_str_hash, g_str_equal, free_gstr, free_gstr);
	
	parse_content(content, map);
	
	free(content);

	g_list_free(keys);
	
	g_hash_table_destroy(map);
	
}