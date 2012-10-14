/*
  md5digest.h
*/

#ifndef __MD5DIGEST_H__
#define __MD5DIGEST_H__

#if defined(__cplusplus)
extern "C"{
#endif

typedef unsigned char MD5DIGEST_WORK_t[96]; // octets alignment
typedef unsigned char MD5DIGEST_BUF_t[16];
typedef unsigned char MD5DIGEST_HEX_t[32 + 1];

extern int md5digest_work_size(void);
extern void md5digest_init(MD5DIGEST_WORK_t work);
extern void md5digest_loop(MD5DIGEST_WORK_t work, char *dat, size_t len);
extern void md5digest_pad(MD5DIGEST_WORK_t work);
extern void md5digest_result(MD5DIGEST_BUF_t md5hash, MD5DIGEST_WORK_t work);
extern char *md5digest_bin16_hex32(MD5DIGEST_HEX_t digest,
  MD5DIGEST_BUF_t md5hash, size_t len);
extern char *md5digest(MD5DIGEST_HEX_t digest, FILE *fp); // calc direct
extern char *md5crypt(char *passwd, char *salt);

#if defined(__cplusplus)
};
#endif

#endif // __MD5DIGEST_H__
