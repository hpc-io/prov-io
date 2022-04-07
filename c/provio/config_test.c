// #include "config.h"
#include "provio.h"
#include <assert.h>

prov_config config;

int main() {
	load_config(&config);
	if(config.prov_base_uri) {
		printf("%s\n", config.prov_base_uri);
	}
	else
		printf("prov_base_uri is null\n");

	if(config.prov_prefix) {
		printf("%s\n", config.prov_prefix);
	}
	else
		printf("prov_prefix is null\n");

	if(config.stat_file_path) {
		printf("%s\n", config.stat_file_path);
	}
	else
		printf("stat_file_path is null\n");

	if(config.new_graph_path) {
		printf("%s\n", config.new_graph_path);
	}
	else
		printf("new_graph_path is null\n");

	if(config.legacy_graph_path) {
		printf("%s\n", config.legacy_graph_path);
	}
	else
		printf("legacy_graph_path is null\n");

	if(config.prov_line_format) {
		printf("%s\n", config.prov_line_format);
	}
	else
		printf("prov_line_format is null\n");

	if(config.enable_stat_file) {
		printf("%d\n", config.enable_stat_file);
	}
	else
		printf("enable_stat_file is null\n");

	if(config.prov_level) {
		printf("%d\n", config.prov_level);
	}
	else
		printf("prov_level is null\n");

}