
/*************************************************************************
   LAB 1

	Edit this file ONLY!

*************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include "dns.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define TableSize 3571
#define DNS List**
#define STEP 83

typedef struct Node {
	IPADDRESS IP;
	char* DomainName;
	struct Node* next;
} Node;

typedef struct List
{
	struct Node* first;
	struct Node* last;
} List;

unsigned int HashCode(const char* S)
{
	unsigned int hash = 0;
	for (int i = 0; S[i] != '\0'; i++) {
		hash *= STEP;
		hash += (S[i] - '-');
		hash %= TableSize;
	}
	return hash;
}

Node* InitNode(IPADDRESS ip, const char* dn) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node != NULL) {
		node->IP = ip;
		node->DomainName = strdup(dn);
		node->next = NULL;
	}
	return node;
}

void AddNodeToHashTable(DNSHandle hDNS, IPADDRESS ip, const char* dn) {
	unsigned int hash = HashCode(dn);
	DNS hashTable = (DNS)hDNS;
	if (hashTable[(unsigned int)hash]->first == NULL) {
		hashTable[hash]->first = InitNode(ip, dn);
		hashTable[hash]->last = hashTable[hash]->first;
	}
	else {
		hashTable[hash]->last->next = InitNode(ip, dn);
		hashTable[hash]->last = hashTable[hash]->last->next;
	}
}

DNSHandle InitDNS()
{
	DNS hashTable = (DNS)calloc(TableSize, sizeof(List*));
	if (hashTable != NULL) {
		for (int i = 0; i < TableSize; i++)
		{
			hashTable[i] = (List*)malloc(sizeof(List));
			if (hashTable[i] != NULL) {
				hashTable[i]->first = NULL;
				hashTable[i]->last = NULL;
			}
		}
		return (DNSHandle)hashTable;
	}
	return (DNSHandle)INVALID_DNS_HANDLE;
}

void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath)
{
	FILE* fin = fopen(hostsFilePath, "r");
	char* domain = (char*)calloc(256, sizeof(char));
	int ipbytes[4];

	while (!feof(fin)) {
		fscanf(fin, "%d.%d.%d.%d %s", &ipbytes[0], &ipbytes[1], &ipbytes[2], &ipbytes[3], domain);
		IPADDRESS ip = ipbytes[3] | ipbytes[2] << 8 | ipbytes[1] << 16 | ipbytes[0] << 24;
		AddNodeToHashTable(hDNS, ip, domain);
	}
	fclose(fin);
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName)
{
	unsigned int i = HashCode(hostName);
	List* list = ((DNS)hDNS)[i];
	Node* node = list->first;
	while (node != NULL && strcmp(node->DomainName, hostName) != 0) {
		node = node->next;
	}
	if (node != NULL)
		return node->IP;
	return INVALID_IP_ADDRESS;
}

void ShutdownDNS(DNSHandle hDNS)
{
	if (hDNS != NULL) {
		DNS hashTable = (DNS)hDNS;
		for (int i = 0; i < TableSize; i++)
		{
			if (hashTable[i] != NULL) {
				Node* node = hashTable[i]->first;
				while (node != NULL) {
					Node* temp = node;
					node = node->next;
					free(temp->DomainName);
					free(temp);
				}
				free(hashTable[i]);
			}
		}
		free(hashTable);
	}
}
