/*
  System Info
	Version: 0.1.0
	Author: Sir-Ignis
*/
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/*function credits: https://stackoverflow.com/a/8465083*/
char* concat(const char *s1, const char *s2)
{
    char *result = (char*)malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/*sends a request to ipinfo to gather data*/
int getIPData(const char * string) {
  CURL *curl = curl_easy_init();
  char* s = concat("https://ipinfo.io/", string);
  curl_easy_setopt(curl, CURLOPT_URL,
    s);
  free(s);
  return curl_easy_perform(curl);
}

/*returns a string depending on input integer i*/
const char * args(int i) {
  switch (i) {
    case 1:
      return "hostname\0";
    case 2:
      return "city\0";
    case 3:
      return "region\0";
    case 4:
      return "loc\0";
    case 5:
      return "postal\0";
    default:
      return "ip\0";
  }
}

/*calls functions to handler ip data*/
void ipDataHandler() {
  int ret;
  for(int i = 0; i < 6; i++) {
    ret = getIPData(args(i));
    if(ret != 0) {
      printf("ERROR! Refer to libcurl-errors\n");
    }
  }
}

int main()
{
  ipDataHandler();
 return 0;
}
