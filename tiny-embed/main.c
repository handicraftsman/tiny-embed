#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <getopt.h>

void display_usage(char* pname) {
  fprintf(stderr, "Usage: %s [OPTIONS]\n\n", pname);
  fprintf(stderr, "Available options: \n");
  fprintf(stderr, "  -h        --help              Show this message\n");
  fprintf(stderr, "  -c FILE   --config-file FILE  Config file       [./tiny-embed.txt]\n");
  fprintf(stderr, "  -d LEVEL  --working-dir DIR   Working directory [./]\n");
  fprintf(stderr, "  -o FILE   --output      FILE  Output .c file    [./output.c]\n");
  fprintf(stderr, "  -H FILE   --header      FILE  Output .h file    [./output.h]\n");
  exit(1);
}

int main(int argc, char** argv) {
  int opt;
  
  static struct option long_options[] = {
    {"help",          0,                 NULL, 'h'},
    {"config-file",   required_argument, NULL, 'c'},
    {"working-dir",   required_argument, NULL, 'd'},
    {"output",        required_argument, NULL, 'o'},
    {"header",        required_argument, NULL, 'H'}
  };
  
  char* p_config_file = "./tiny-embed.txt";
  char* p_working_dir = "./";
  char* p_output      = "./output.c";
  char* p_header      = "./output.h";
  
  while ((opt = getopt_long(argc, argv, "hc:d:o:H:", long_options, NULL)) != EOF)
    switch(opt) {
    case 'h':
      display_usage(argv[0]);
      exit(2);
    case 'c':
      p_config_file = optarg;
      break;
    case 'd':
      p_working_dir = optarg;
      break;
    case 'o':
      p_output = optarg;
      break;
    case 'H':
      p_header = optarg;
      break;
    default:
      exit(2);
  }
  
  if (chdir(p_working_dir) == -1) {
    fprintf(stderr, "%s: ", p_working_dir);
    perror("chdir(p_working_dir)");
    exit(1);
  }
  
  FILE* config_file = fopen(p_config_file, "r");
  if (!config_file) {
    fprintf(stderr, "%s: ", p_config_file);
    perror("fopen(p_config_file, \"r\")");
    exit(1);
  }
  
  FILE* output_file = fopen(p_output, "w");
  if (!output_file) {
    fprintf(stderr, "%s: ", p_output);
    perror("fopen(p_output, \"w\")");
    exit(1);
  }
  
  FILE* header_file = fopen(p_header, "w");
  if (!header_file) {
    fprintf(stderr, "%s: ", p_header);
    perror("fopen(p_header, \"w\")");
    exit(1);
  }
  
  fprintf(header_file, "#pragma once\n\n");
  
  char symname[64]; char fname[256];
  int n = 0;
  while ((n = fscanf(config_file, "%64s %256s\n", symname, fname), n != 0 && n != -1)) {
    fprintf(stderr, "%s <- %s: ", symname, fname);
    FILE* input_file = fopen(fname, "rb");
    if (!input_file) {
      fprintf(stderr, "%s: ", fname);
      perror("fopen(fname, \"rb\")");
      fclose(config_file);
      fclose(output_file);
      fclose(header_file);
      exit(1);      
    }
    
    if (fseek(input_file, 0, SEEK_END)) {
      fprintf(stderr, "%s: ", fname);
      perror("fseek(input_file, 0, SEEK_END)");
      exit(1);
    }
    off_t l = ftell(input_file);
    if (fseek(input_file, 0, SEEK_SET)) {
      fprintf(stderr, "%s: ", fname);
      perror("fseek(input_file, 0, SEEK_SET)");
      exit(1);
    }
    
    fprintf(header_file, "const char %s[%d];\n", symname, (int) l + 1);
    
    fprintf(output_file, "const char %s[] = {", symname);
    
    unsigned char buffer[32];
    while (!feof(input_file)) {
      size_t count = fread(buffer, 1, 32, input_file);
      for (int i = 0; i < count; ++i) {
        fprintf(output_file, "0x%02X,", buffer[i]);
      };
    };
    
    fprintf(output_file, "0x00};\n");
    
    fprintf(stderr, "OK\n");
    
    fclose(input_file);
  }
    
  fclose(config_file);
  fclose(output_file);
  fclose(header_file);
}
