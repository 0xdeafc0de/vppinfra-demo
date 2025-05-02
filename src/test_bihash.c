/*
 * Client MAC-to-IP Lookup table example with VPP Bihash Hash Table and memory pool utilities.
 *
 * An efficient hash table exampole using vppinfra bihash 8_8 (8 byte key, 8 byte value).
 * Keyed by the client's MAC address, the hash table designed for one writer many reader scenario
 * 
 * Example use case - Lightweight IP lease management
 */

#include "test_bihash.h"
#include <vppinfra/mem.h>
#include <string.h>

/**
 * @brief Initializes the hash table and memory pool.
 */
void client_db_init(app_main_t *am) {
  am->lookup_table_num_buckets = NUM_BUCKETS;
  am->lookup_table_memory_size = MEMORY_SIZE;
  
  clib_bihash_init_8_8(&am->lookup_table, "MAC-to-IP Lookup Table",
                       am->lookup_table_num_buckets,
                       am->lookup_table_memory_size);
}

int client_db_add_or_update(app_main_t *am, const client_info_t *info, client_t **pp_info) {
  clib_bihash_kv_8_8_t kv;
  clib_bihash_8_8_t *lut = &am->lookup_table;
  client_t *cs;
  int found = 1;

  clib_memset(&kv, 0, sizeof(kv));
  if (info == NULL) {
          return -1;
  } 
    
  memcpy(&kv.key, info->mac_address, sizeof(info->mac_address));

  if (clib_bihash_search_8_8(lut, &kv, &kv) < 0) {
    found = 0;
  } 

  if (!found) {
    pool_get(am->clients, cs);
    clib_memset(cs, 0, sizeof(*cs));

    // Fill client structure
    memcpy(cs->mac_address, info->mac_address, sizeof(cs->mac_address));
    cs->ip4_address = info->ip4_address;
    cs->srvr_address = info->srvr_address;
    cs->sw_if_index = info->clnt_if_index;
    cs->last_heard = time(NULL);

    kv.value = cs - am->clients;
    clib_bihash_add_del_8_8(lut, &kv, 1);

    if (pp_info) {
      *pp_info = cs;
    }
  } else {
    // Entry already exists, update fields
    cs = pool_elt_at_index(am->clients, kv.value);
    
    cs->ip4_address = info->ip4_address;
    cs->srvr_address = info->srvr_address;
    cs->sw_if_index = info->clnt_if_index;
    cs->last_heard = time(NULL);
    
    if (pp_info) {
      *pp_info = cs;
    }
  }
  
  return 0;
}

int client_db_delete(app_main_t *am, const u8 mac_address[6]) {
  clib_bihash_kv_8_8_t kv;
  clib_bihash_8_8_t *lut = &am->lookup_table;
  
  clib_memset(&kv, 0, sizeof(kv));
  memcpy(&kv.key, mac_address, 6);
  
  // Search to see if the entry exists
  if (clib_bihash_search_8_8(lut, &kv, &kv) < 0) {
    return -1; // Not found
  }

  // Get the client pointer from pool using stored index
  client_t *cs = pool_elt_at_index(am->clients, kv.value);

  clib_memset(cs, 0, sizeof(*cs));

  // Free the pool element
  pool_put(am->clients, cs);

  // Delete the hash table entry
  clib_bihash_add_del_8_8(lut, &kv, 0 /* is_add = 0 --> delete */);
  
  return 0; // Success
}

client_t *client_db_find(app_main_t *am, const u8 mac_address[6]) {
  clib_bihash_kv_8_8_t kv;
  clib_bihash_8_8_t *lut = &am->lookup_table;

  clib_memset(&kv, 0, sizeof(kv));
  memcpy(&kv.key, mac_address, 6);

  if (clib_bihash_search_8_8(lut, &kv, &kv) < 0) {
    return NULL;  // Not found
  }

  // Index to client pool is stored in value
  return pool_elt_at_index(am->clients, kv.value);
}
