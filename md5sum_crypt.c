/*
  md5sum_crypt.c

  > cl md5sum_crypt.c md5digest_mt.lib -EHsc -I. -link -LIBPATH:.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5digest.h"

char *hexdigest(char *plain)
{
  MD5DIGEST_HEX_t digest;
  MD5DIGEST_BUF_t md5hash;
  MD5DIGEST_WORK_t work;
  int worksize = sizeof(work) / sizeof(work[0]); // ? == md5digest_work_size()
  int n, p, s = strlen(plain);
  md5digest_init(work);
  memset(md5hash, 0, sizeof(md5hash) / sizeof(md5hash[0]));
  for(n = 0, p = 0; p < s; p += n)
    md5digest_loop(work, plain + p, n = worksize < s - p ? worksize : s - p);
  md5digest_pad(work);
  md5digest_result(md5hash, work);
  return md5digest_bin16_hex32(
    digest, md5hash, sizeof(md5hash) / sizeof(md5hash[0]));
}

int main(int ac, char *av[])
{
  if(ac < 2){
    fprintf(stderr, "Usage 1: %s key clearpw\n", av[0]);
    fprintf(stderr, "Usage 2: %s file_to_calc_md5hash\n", av[0]);
    assert(0 == strcmp("d41d8cd98f00b204e9800998ecf8427e", hexdigest("")));
    assert(0 == strcmp("0cc175b9c0f1b6a831c399e269772661", hexdigest("a")));
    assert(0 == strcmp("900150983cd24fb0d6963f7d28e17f72", hexdigest("abc")));
    assert(0 == strcmp("f96b697d7cb7938d525a2f31aaf161d0", hexdigest(
      "message digest")));
    assert(0 == strcmp("c3fcd3d76192e4007dfb496cca67e13b", hexdigest(
      "abcdefghijklmnopqrstuvwxyz")));
    assert(0 == strcmp("d174ab98d277d9f5a5611c2c9f419d9f", hexdigest(
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")));
    assert(0 == strcmp("57edf4a22be3c955ac49da2e2107b67a", hexdigest(
      "1234567890123456789012345678901234567890" \
      "1234567890123456789012345678901234567890")));
    assert(0 == strcmp("49cb3608e2b33fad6b65df8cb8f49668", hexdigest(
      "1234567890123456789012345678901234567890" \
      "1234567890123456789012345678901234567890" \
      "12345678901234567890")));
    return 1;
  }
  if(ac >= 3){
    fprintf(stdout, "key = [%s]\n", av[1]);
    fprintf(stdout, "clearpw = [%s]\n", av[2]);
    fprintf(stdout, "cryptpw = [%s]\n", md5crypt(av[2], av[1]));
  }else{
    MD5DIGEST_HEX_t digest;
    FILE *fp;
    fprintf(stdout, "file = [%s]\n", av[1]);
    if(!(fp = fopen(av[1], "rb"))){
      fprintf(stderr, "error: file can not read: %s\n", av[1]);
      return 255;
    }
    fprintf(stdout, "\ndigest = [%s]\n", md5digest(digest, fp));
    fclose(fp);

    if(1){ // test for incremental calc
      MD5DIGEST_HEX_t digest;
      MD5DIGEST_BUF_t md5hash;
      MD5DIGEST_WORK_t work;
      int worksize = sizeof(work) / sizeof(work[0]);
      unsigned char buf[8192];
      FILE *fp;
      fprintf(stdout, "file = [%s]\n", av[1]);
      if(!(fp = fopen(av[1], "rb"))){
        fprintf(stderr, "error: file can not read: %s\n", av[1]);
        return 255;
      }
      fprintf(stdout, "work size (get) = [%d]\n", md5digest_work_size());
      fprintf(stdout, "work size (alloc) = [%d]\n", sizeof(work));
      md5digest_init(work);
      memset(md5hash, 0, sizeof(md5hash) / sizeof(md5hash[0]));
      while(!feof(fp)) md5digest_loop(work, buf, fread(buf, 1, worksize, fp));
      md5digest_pad(work);
      md5digest_result(md5hash, work);
      fprintf(stdout, "\ndigest = [%s]\n", md5digest_bin16_hex32(
        digest, md5hash, sizeof(md5hash) / sizeof(md5hash[0])));
      fclose(fp);
    }
  }
  return 0;
}
