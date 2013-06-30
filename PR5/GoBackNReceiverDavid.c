/***************************************************************************
 *
 * This file is part of the ProtSim framework developed by TKN for a
 * practical course on basics of simulation and Internet protocol functions
 *
 * Copyright:   (C)2004-2007 Telecommunication Networks Group (TKN) at
 *              Technische Universitaet Berlin, Germany.
 *
 * Authors:     Lars Westerhoff, Guenter Schaefer
 *
 **************************************************************************/

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <inttypes.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <time.h>
#include "timeval_macros.h"
#include "GoBackNMessageStruct.h"
#include "SocketConnection.h"

#define DEBUG
#ifdef DEBUG
#define DEBUGOUT(x, ...) fprintf(stderr, x, __VA_ARGS__)
#else
#define DEBUGOUT(x, ...)
#endif

#define DEFAULT_LOCAL_PORT 12105

#define DEFAULT_REMOTE_PORT 4343
#define DEFAULT_PAYLOAD_SIZE 1024

unsigned       localPort, remotePort;
char*          remoteName;
char*          fileName;

long           lastReceivedSeqNo;
size_t         goodBytes, totalBytes;

void help(int exitCode) {
    fprintf(stderr,"GoBackNReceiver [--local|-l port] [--remote|-r port] hostname file\n");

    exit(exitCode);
}

void initialize(int argc, char** argv) {
    localPort = DEFAULT_LOCAL_PORT;
    remotePort = DEFAULT_REMOTE_PORT;

    while (1) {
        static struct option long_options[] = {
            {"local", 1, NULL, 'l'},
            {"remote", 1, NULL, 'r'},
            {"help", 0, NULL, 'h'},
            {0, 0, 0, 0}
        };

        int c = getopt_long (argc, argv, "l:r:h", long_options, NULL);
        if (c == -1) break;

        int retval = 0;
        switch (c) {
            case 'l':
                retval = sscanf(optarg, "%u", &localPort);
                if (retval < 1) help(1);
                break;

            case 'r':
                retval = sscanf(optarg, "%u", &remotePort);
                if (retval < 1) help(1);
                break;

            case 'h':
                help(0);
                break;

            case '?':
                help(1);
                break;

            default:
                printf ("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (argc < optind+2) help(1);

    remoteName = argv[optind];
    fileName = argv[optind+1];

    lastReceivedSeqNo = -1;
    goodBytes = totalBytes = 0;
}

int openConnection() {
    int s = connectToServer(localPort, remoteName, remotePort);
    int x = 1;
    if (setsockopt(s, SOL_TCP, TCP_NODELAY, &x, sizeof(int)) < 0) {
        perror("setsockopt(TCP_NODELAY)");
        exit(1);
    }
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        exit(1);
    }
    x = sizeof(GoBackNMessageStruct);
    if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, &x, sizeof(int)) < 0) {
        perror("setsockopt(SO_SNDBUF)");
        exit(1);
    }

    return s;
}

void writeBuffer(FILE* file, GoBackNMessageStruct* packet) {
    size_t count = packet->size - sizeof(*packet);
    size_t retval;
    if ((retval = fwrite(packet->data, 1, count, file)) < count) {
        if (ferror(file)) {
            perror("fread");
            exit(1);
        }
	else {
	    fprintf(stderr,"WARNING: Could not write complete packet to file!\n");
	}
    }
    DEBUGOUT("FILE: %u bytes written\n", retval);
}

void sendAck(int s, GoBackNMessageStruct* packet, long expected) {
    GoBackNMessageStruct* ack = allocateGoBackNMessageStruct(0);
    ack->destNode = packet->srcNode;
    ack->destAppl = packet->srcAppl;
    ack->seqNo = -1;
    ack->seqNoExpected = expected;
    ack->size = sizeof(*ack);
    ack->hasErrors = false;

    int retval;
    if ((retval = send(s, ack, ack->size, 0)) < 0) {
	perror("send");
	exit(1);
    }
    DEBUGOUT("SOCKET: %d bytes sent\n", retval);
    freeGoBackNMessageStruct(ack);
}

int main(int argc, char** argv) {
    initialize(argc, argv);

    // open file
    FILE* output = fopen(fileName, "wb");
    if (output == NULL) {
        perror("fopen");
        exit(1);
    }

    // open connection
    int s = openConnection();

    ssize_t bytesRead = 0;
    while (1) {
	GoBackNMessageStruct* data = allocateGoBackNMessageStruct(DEFAULT_PAYLOAD_SIZE);
        if (recv(s, data, sizeof(*data), MSG_PEEK) < 0) {
            perror("recv(MSG_PEEK)");
            exit(1);
        }
        bytesRead = data->size;
	bytesRead = recv(s, data, data->size, 0);
        if (bytesRead < 0) {
	    perror("recv");
	    exit(1);
	}
        if (bytesRead < data->size) {
            fprintf(stderr,"WARNING: Truncated read\n");
        }
        DEBUGOUT("SOCKET: %d bytes received from %u.%u\n", bytesRead, data->srcNode, data->srcAppl);
        DEBUGOUT("%d.%d -> %d.%d, #%d/%d, size: %u, errors: %d\n",
                 data->srcNode, data->srcAppl, data->destNode, data->destAppl, data->seqNo,
                 data->seqNoExpected, data->size, (int)data->hasErrors);

        if (bytesRead == 0) {
            freeGoBackNMessageStruct(data);
            break;
        }
        
	data->size = bytesRead;
	totalBytes += bytesRead-sizeof(*data);

        /* YOUR TASK:*/
		
		// Check packet for errors
		if(data->hasError)
			 {
				DEBUGOUT("recievd data package has error\n");
			 }else
			 {
				
				if(data->seqNo<lastReceivedSeqNo+1) // # an older one
				{
					DEBUGOUT("recievd data package seqNo to low-> old package\n");
					sendAck(s, &data, lastReceivedSeqNo+1);
				}
				else if(data->seqNo>lastReceivedSeqNo+1) // # too new (we missed at least one packet)
				{
					DEBUGOUT("recievd data package seqNo to high-> too new\n");
					sendAck(s, &data, lastReceivedSeqNo+1);
				}
				else // # is the one we are expecting
				{
					// Acknowledge the packet if appropriate
					sendAck(s, &data, data->seqNo+1);
					
					// Write packet to file if appropriate
					writeBuffer(&output, &data);
					
					// Update sequence number counter
					lastReceivedSeqNo=data->seqNo;
					// Update goodBytes
					goodBytes+=bytesRead;
					// If this was the last packet of the transmission
					if(sizeof(*data)==data->size))
					{
						//  close the file with fclose and
						fclose(output);

						//   output totalBytes and goodBytes
						DEBUGOUT("file recieved. total bytes: %d, good bytes: %d errounous Bytes\n", totalBytes, goodBytes, (totalBytes-goodBytes) );
						freeGoBackNMessageStruct(data);
					}
				}
			 }
		



	freeGoBackNMessageStruct(data);
    }
}
