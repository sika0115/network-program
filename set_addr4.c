/* set_addr4.c -- set IPv4 address and port # */ 
#include "einet.h"
struct sockaddr_in *
set_address4 (char *hname, char *iname, char *sname,
              struct sockaddr_in *sap, char *protocol)
{     
      struct servent  *sp;
      struct hostent  *hp;
      char            *endptr;
      short           port;
      
      bzero(sap, sizeof(*sap));　//必ず全体を０でクリアする
      sap->sin_family = AF_INET; //アドレスファミリー(sin_family)にAF_INET(IPv4)を設定する
#ifndef CYGWIN
      sap->sin_len = sizeof(*sap);//サイズ(sin_len)に構造体のサイズを設定する
#endif

      if (hname == NULL)
            sap->sin_addr.s_addr = htonl(INADDR_ANY);
            /*
            実行例：% ./tcp4_echo_srv2 &
            解説：
            ・<host>の省略時、hnameはNULLが設定されている
            ・sap->sin_addrにhtonl(INADDR_ANY)を設定する
            ・INADDR_ANRは０に設定されている
            */
      else {
            if (!inet_pton(AF_INET, hname, &sap->sin_addr)) {
                  /*
                  実行例：% ./tcp4_echo_cli3 172.25.8.142
                  解説：
                  ・<host>にリテラル（数値）を指定した時、hnameはリテラル（数値）の文字列が設定されている。
                  ・inet_pton関数でIPv4アドレスに変換し、sap->sin_addrに直接設定する。
                  ・inet_ptonの戻り値で、1なら変換成功。0なら変換不成功。
                  */
                   hp = gethostbyname2(hname, AF_INET);
                  /*
                  実行例：% ./tcp4_echo_cli3 salem
                  解説：
                  ・<host>にホスト名を指定した時、hnameはホスト名の文字列が設定されている。
                  ・gethostbyname2関数でIPv4アドレスに変換し、sap->sin_addrにhp->h_addrを設定する。
                  */
            if (hp == NULL) {
                  /*戻り値は10行目のstruct hostent構造体へのポインタ。
                  NULLの時は、エラー、ホスト名を解決できなかった。*/
                  error(0, 0, "unknown IPv4 host: %s", hname);
                  return NULL;
                  }
            memcpy((char *)&sap->sin_addr, (char *)hp->h_addr, hp->h_length);
            }
      }

if (sname == NULL)
      sap->sin_port = htons(PORT_NO);
      /*
      実行例：% ./tcp4_echo_srv2 &
      解説：
      ・-p<port>の省略時、snameがNULLに設定されている。
      ・sap->sin_portにhtons(PORT_NO)を設定する。
      ・PORT_NOは(9Y00※ + 出席番号)に設定されている。
      ※ネットワーク演習:9500、情報ネットワーク工学:9700
      */
else {
      port = strtol(sname, &endptr, 0);
      if (*endptr == 0)
      　/* *endptr == 0 は文字列の最後まで読んだという意味。*/
            sap->sin_port = htons(port);
            /* "port=" ~ "(port);"
            実行例：% ./tcp6_echo_cli3 -p 7 winston
            ・-p<port>にリテラル（数値）を設定した時、snameはリテラル（数値）の文字列が設定されている。
            ・strol()でポート番号に変換し、sap->sin_portにhtons(port)を設定する。
            */
      else {
           sp = getservbyname(sname, protocol);
           /*
           実行例：% ./tcp6_echo_cli3 -p echo tarheel
           解説：
            ・-p<host>にサービス名を指定した時、snameにはサービス名の文字列が設定されている。
            ・gethostbyname()でポート番号に変換し、sap->sin_portにsp->s_portを設定する。      
           */
           if (sp == NULL) {
                 /*戻り値は9行目のstruct servent構造体へのポインタ。
                  NULLの時は、エラー、サービス名を解決できなかった。*/
                  error(0, 0, "unknown service: %s", sname);
                  return NULL;
            }
            sap->sin_port = sp->s_port;
      }
}
return sap;
}