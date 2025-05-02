#include "client_db.h"
#include <vppinfra/mem.h>
#include <string.h>
    
void client_db_init(app_main_t *nmp) {
  nmp->lookup_table_num_buckets = NUM_BUCKETS;
  nmp->lookup_table_memory_size = MEMORY_SIZE;
  
  clib_bihash_init_8_8(&nmp->lookup_table, "MAC-to-IP Lookup Table",
                       nmp->lookup_table_num_buckets,
                       nmp->lookup_table_memory_size);
}

int client_db_add_or_update(app_main_t *nmp, const client_info_t *info, client_t **pp_info) {
  clib_bihash_kv_8_8_t kv;
  clib_bihash_8_8_t *lut = &nmp->lookup_table;
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
    pool_get(nmp->clients, cs);
    clib_memset(cs, 0, sizeof(*cs));

    memcpy(cs->mac_address, info->mac_address, sizeof(cs->mac_address));
    cs->ip4_address = info->ip4_address;
    cs->srvr_address = info->srvr_address;
    cs->sw_if_index = info->clnt_if_index;
    cs->last_heard = time(NULL);

    kv.value = cs - nmp->clients;
    clib_bihash_add_del_8_8(lut, &kv, 1);

    if (pp_info) {
      *pp_info = cs;
    }
  } else {
    cs = pool_elt_at_index(nmp->clients, kv.value);
    
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

int client_db_delete(app_main_t *nmp, const u8 mac_address[6]) {
  clib_bihash_kv_8_8_t kv;
  clib_bihash_8_8_t *lut = &nmp->lookup_table;
  
  clib_memset(&kv, 0, sizeof(kv));
  memcpy(&kv.key, mac_address, 6);
  
  // Search to see if the entry exists
  if (clib_bihash_search_8_8(lut, &kv, &kv) < 0) {
    return -1; // Not found
  }

  // Get the client pointer from pool using stored index
  client_t *cs = pool_elt_at_index(nmp->clients, kv.value);

  // Optionally, you can zero the struct (not necessary if reusing index)
  clib_memset(cs, 0, sizeof(*cs));

  // Free the pool element
  pool_put(nmp->clients, cs);

  // Delete the hash table entry
  clib_bihash_add_del_8_8(lut, &kv, 0 /* is_add = 0 --> delete */);
  
  return 0; // Success
}

client_t *client_db_find(app_main_t *nmp, const u8 mac_address[6]) {
  clib_bihash_kv_8_8_t kv;
  clib_bihash_8_8_t *lut = &nmp->lookup_table;

  clib_memset(&kv, 0, sizeof(kv));
  memcpy(&kv.key, mac_address, 6);

  if (clib_bihash_search_8_8(lut, &kv, &kv) < 0) {
    return NULL;  // Not found
  }

  // Index to client pool is stored in value
  return pool_elt_at_index(nmp->clients, kv.value);
}
