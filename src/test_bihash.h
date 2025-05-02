#ifndef TEST_BIHASH_H
#define TEST_BIHASH_H

#include <vppinfra/clib.h>
#include <vppinfra/bihash_8_8.h>
#include <vnet/ip/ip4_packet.h>
#include <time.h>

#define DIR_MAX         2
#define MAX_CLIENTS     (64 << 10) // 64K clients
#define NUM_BUCKETS     (MAX_CLIENTS / BIHASH_KVP_PER_PAGE)
#define MEMORY_SIZE     (6 << 20)  // 6MB memory

typedef struct {
  u32 sw_if_index;
  u8 mac_address[6];
  ip4_address_t ip4_address;
  ip4_address_t srvr_address;
  u32 flags;
  time_t last_heard;
  u32 num_pkt_per_dir[DIR_MAX];
} client_t;

typedef struct {
  clib_bihash_8_8_t lookup_table;
  u32 lookup_table_num_buckets;
  uword lookup_table_memory_size;

  client_t *clients;
} app_main_t;

typedef struct {
  u8 mac_address[6];
  ip4_address_t ip4_address;
  ip4_address_t srvr_address;
  u32 clnt_if_index;
} client_info_t;

int client_db_add_or_update(app_main_t *nmp, const client_info_t *info, client_t **pp_info);
int client_db_delete(app_main_t *nmp, const u8 mac_address[6]);
client_t *client_db_find(app_main_t *nmp, const u8 mac_address[6]);
