/* -*-mode:c; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
  p0f - SSL fingerprinting
  -------------------------

  Copyright (C) 2012 by Marek Majkowski <marek@popcount.org>

  Distributed under the terms and conditions of GNU LGPL.

 */

#ifndef _HAVE_FP_SSL_H
#define _HAVE_FP_SSL_H

#include "types.h"


/* Constants */

#define MATCH_MAYBE 0x10000000  /* '?' - indicats a single optional match */
#define MATCH_ANY   0x20000000  /* '*' - match zero or more elements */
#define END_MARKER  0x40000000  /* internal marker */

#define SSL_MAX_CIPHERS 128     /* max number of ciphers in a signature */
#define SSL_MAX_TIME_DIFF 10    /* remote clock scew limit, in seconds */


/* Flags */

#define SSL_FLAG_V2    0x0001  /* SSLv2 handshake. */
#define SSL_FLAG_VER   0x0002  /* Record version different than ClientHello. */
#define SSL_FLAG_RTIME 0x0004  /* weird SSL time, (delta > 5 years), most likely random*/
#define SSL_FLAG_STIME 0x0008  /* small SSL time, (absolute value < 1 year)
                                  most likely time since reboot for old ff */
#define SSL_FLAG_COMPR 0x0010  /* Deflate compression support. */

/* SSLv3 */

#define SSL3_REC_HANDSHAKE 0x16    /* 22 */
#define SSL3_MSG_CLIENT_HELLO 0x01

struct ssl3_record_hdr {

  u8 content_type;
  u8 ver_maj;
  u8 ver_min;
  u16 length;

} __attribute__((packed));


struct ssl3_message_hdr {

  u8 message_type;
  u8 length[3];

} __attribute__((packed));



/* Internal data structures */

struct ssl_sig_record;

struct ssl_sig {

  u16 request_version;   /* Requested SSL version (maj << 8) | min */

  u32 remote_time;       /* ClientHello message gmt_unix_time field */
  u32 recv_time;         /* Actual receive time */

  u32* cipher_suites;    /* List of SSL ciphers, END_MARKER terminated */

  u32* extensions;       /* List of SSL extensions, END_MARKER terminated */

  u32 flags;             /* SSL flags */

  struct ssl_sig_record* matched; /* NULL = no match */
};


u8 process_ssl(u8 to_srv, struct packet_flow* f);

#endif /* _HAVE_FP_SSL_H */
