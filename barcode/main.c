#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stropts.h>
#include <curl/curl.h>

// custom libraries
#include "barcodeParser.h"

#define POLL_TIME_MS 1000  
#define MAX_BUFFER_SIZE 1024

int continuePolling = 1;

// check if file exists
int fileExists(char* filepath){
	if( access( filepath, F_OK ) != -1 ) {
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

// The signal handler just clears the flag and re-enables itself
void catchSignal(int signal){
	continuePolling = 0;
}

int main(int argc, char** argv){
	CURL *curl;
	//CURLcode res;
	
	//curl_global_init(CURL_GLOBAL_ALL);

	//curl = curl_easy_init();

	// check if enough arguments are passed
	if (argc < 2){
		printf("Not enough arguments\n");
		exit(EXIT_FAILURE);
	}
	
	// check if valid file path
	if (fileExists(argv[1]) == EXIT_FAILURE){
		printf("Not valid file path %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	// Establish a handler for signals
	signal(SIGINT | SIGKILL, catchSignal);
	
	// The structure for two events
	struct pollfd fds[1] = {{0}};
	 
	int fd = open(argv[1], O_RDONLY | O_NONBLOCK);
	
	if (fd < 0 ){
		printf("Error opening file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	printf("Polling device %s\n", argv[1]);
	
	// Monitor file for input
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	barcodeContext context = initializeBarcodeContext();
	barcodeOutput output = {0};
	
	while (continuePolling){
		// Poll
		int ret = poll(fds, 1, POLL_TIME_MS);
		//printf("ret:%d  \n", ret );	
		if (ret > 0 && (fds[0].revents & POLLIN)){
			// If we detect the event, 
			// zero it out so we can reuse the structure
			int readBytes = 0;
			char buffer[MAX_BUFFER_SIZE] = {0};
			//printf("INPUT CAPTURED\n");
				
			if ((readBytes = read(fds[0].fd, buffer, sizeof(buffer))) > 0){
				if (validString(buffer, readBytes) == SUCCESS){
					barcodeInput inputLine = convertStringToLineInput(buffer, readBytes);
					addInputLineToContext(&context, inputLine);
					bzero(buffer, sizeof(buffer));
				}
			}
			//printf("numner of readbytes:%d  \n", readBytes );
			//printf("inputline: %s \n", context);				
			if (parseBarcodeContext(context, &output) == SUCCESS){
				
				char *datasent = output.line;
				if (curl){
				
				CURLcode res;
	
				curl_global_init(CURL_GLOBAL_ALL);

				curl = curl_easy_init();
					
				curl_easy_setopt(curl, CURLOPT_URL, "http://169.254.164.6:8000/licence/");  //169.254.164.6
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 20L);
				curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, datasent);
				//printf("BARCODE SCAN SUCCESS \n");
				printf("BARCODE RESULT : %s\n", output.line);
				//bzero(&output,sizeof(output)) ;
				res = curl_easy_perform(curl);
				if(res != CURLE_OK){
				 fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				}
				//curl_easy_cleanup(curl);

				//printf("BARCODE SCAN SUCCESS \n");
				//printf("BARCODE RESULT : %s\n", output.line);
				context = initializeBarcodeContext();
				bzero(&output,sizeof(output)) ;
				curl_easy_cleanup(curl);
				}
		}
		}
		// zero it out so we can reuse the structure
		fds[0].revents = 0;
		
	}	
	curl_global_cleanup();
	exit(EXIT_SUCCESS);
}
