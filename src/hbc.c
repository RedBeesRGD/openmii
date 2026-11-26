#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <gctypes.h>
#include <string.h>
#include <malloc.h>

#include "hbc.h"
#include "utils.h"

#include "ohbc/hbc_content0.h"
#include "ohbc/hbc_content1.h"
#include "ohbc/hbc_certs.h"
#include "ohbc/hbc_tik.h"
#include "ohbc/hbc_tmd.h"

s8 install_hbc(void) {
	s32 ret, cfd;
	s32 BLK_SIZE = 1024, offset = 0, size;
	u8 *contentBuffer = (u8 *)memalign(32, BLK_SIZE);
	
	ret = ES_AddTicket((signed_blob *)hbc_tik, hbc_tik_size, (signed_blob *)hbc_certs, hbc_certs_size, NULL, 0);
	printf("ES_AddTicket returned: %d\n", ret);
	
	ret = ES_AddTitleStart((signed_blob *)hbc_tmd, hbc_tmd_size, (signed_blob *)hbc_certs, hbc_certs_size, NULL, 0);
	printf("ES_AddTitleStart returned: %d\n", ret);
	
	cfd = ES_AddContentStart(0x000100014F484243, 0);
	printf("ES_AddContentStart(0) returned: %d\n", cfd);
	
	while (offset < hbc_content0_size) {
		size = (offset + BLK_SIZE <= hbc_content0_size) ? BLK_SIZE : hbc_content0_size - offset;
		memcpy(contentBuffer, (u8 *)(hbc_content0+offset), size);
		ret = ES_AddContentData(cfd, contentBuffer, size);
		offset += BLK_SIZE;
	}
	
	printf("ES_AddContentData(0) returned: %d\n", ret);
	ret = ES_AddContentFinish(cfd);

	printf("ES_AddContentFinish(0) returned: %d\n", ret);
	
	cfd = ES_AddContentStart(0x000100014F484243, 1);
	printf("ES_AddContentStart(1) returned: %d\n", cfd);
	
	offset = 0;
	
	while (offset < hbc_content1_size) {
		size = (offset + BLK_SIZE <= hbc_content1_size) ? BLK_SIZE : hbc_content1_size - offset;
		memcpy(contentBuffer, (u8 *)(hbc_content1+offset), size);
		ret = ES_AddContentData(cfd, contentBuffer, size);
		offset += BLK_SIZE;
	}
	
	printf("ES_AddContentData(1) returned: %d\n", ret);
	ret = ES_AddContentFinish(cfd);

	printf("ES_AddContentFinish(1) returned: %d\n", ret);
	ret = ES_AddTitleFinish();

	printf("ES_AddTitleFinish returned: %d\n", ret);

	if (contentBuffer != NULL) {
		free(contentBuffer);
	}

	return ret;
}

