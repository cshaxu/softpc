#include "media_snapshot.h"

#include <stdlib.h>
#include <string.h>

enum { MEDIA_SLOTS = 4, MEDIA_BLOCK_BYTES = 4096 };

typedef struct media_page {
    struct media_page *next;
    lib_u64 offset;
    lib_u32 count;
    lib_u8 bytes[MEDIA_BLOCK_BYTES];
} media_page;

typedef struct media_slot {
    lib_u32 present, mode, target_mode, cylinder;
    lib_u64 size, page_count;
    lib_u8 digest[32];
    char path[SOFTPC_MEDIA_ARCHIVE_PATH_MAX];
    media_page *pages;
} media_slot;

struct softpc_media_archive {
    media_slot slots[MEDIA_SLOTS];
    lib_bool prepared;
};

typedef struct media_hash {
    lib_u32 h[8];
    lib_u64 bytes;
    lib_u8 block[64];
    unsigned used;
} media_hash;

static lib_u32 rotate(lib_u32 n, unsigned bits)
{ return (n >> bits) | (n << (32u - bits)); }

static void hash_block(media_hash *hash)
{
    static const lib_u32 k[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };
    lib_u32 w[64], a,b,c,d,e,f,g,h;
    unsigned i;
    for (i = 0; i < 16; ++i) {
        const lib_u8 *p = hash->block + i * 4u;
        w[i] = ((lib_u32)p[0] << 24) | ((lib_u32)p[1] << 16) |
            ((lib_u32)p[2] << 8) | p[3];
    }
    for (; i < 64; ++i) {
        lib_u32 x = w[i-15], y = w[i-2];
        w[i] = w[i-16] + (rotate(x,7)^rotate(x,18)^(x>>3)) +
            w[i-7] + (rotate(y,17)^rotate(y,19)^(y>>10));
    }
    a=hash->h[0]; b=hash->h[1]; c=hash->h[2]; d=hash->h[3];
    e=hash->h[4]; f=hash->h[5]; g=hash->h[6]; h=hash->h[7];
    for (i = 0; i < 64; ++i) {
        lib_u32 t1 = h + (rotate(e,6)^rotate(e,11)^rotate(e,25)) +
            ((e&f)^((~e)&g)) + k[i] + w[i];
        lib_u32 t2 = (rotate(a,2)^rotate(a,13)^rotate(a,22)) +
            ((a&b)^(a&c)^(b&c));
        h=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2;
    }
    hash->h[0]+=a; hash->h[1]+=b; hash->h[2]+=c; hash->h[3]+=d;
    hash->h[4]+=e; hash->h[5]+=f; hash->h[6]+=g; hash->h[7]+=h;
}

static media_hash hash_begin(void)
{
    media_hash h = {{0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19},0,{0},0};
    return h;
}

static void hash_add(media_hash *h, const lib_u8 *bytes, lib_size count)
{
    h->bytes += count;
    while (count != 0) {
        lib_size n = 64u - h->used;
        if (n > count) n = count;
        memcpy(h->block + h->used, bytes, n);
        h->used += (unsigned)n; bytes += n; count -= n;
        if (h->used == 64) { hash_block(h); h->used = 0; }
    }
}

static void hash_end(media_hash *h, lib_u8 digest[32])
{
    lib_u64 bits = h->bytes * 8u;
    unsigned i;
    h->block[h->used++] = 0x80;
    if (h->used > 56) {
        memset(h->block + h->used, 0, 64u - h->used);
        hash_block(h); h->used = 0;
    }
    memset(h->block + h->used, 0, 56u - h->used);
    for (i=0; i<8; ++i) h->block[63u-i] = (lib_u8)(bits >> (i*8u));
    hash_block(h);
    for (i=0; i<32; ++i)
        digest[i] = (lib_u8)(h->h[i/4] >> (24u-(i%4)*8u));
}

static void media_view(unsigned index, softpc_media_view *view)
{
    if (index < 2) softpc_floppy_media_view(index, view);
    else softpc_hdd_media_view(index - 2, view);
}

static lib_status verify_base(const media_slot *slot, lib_storage_medium *base)
{
    media_hash hash = hash_begin();
    lib_u8 bytes[MEDIA_BLOCK_BYTES], digest[32];
    lib_size offset = 0;
    if (lib_storage_medium_byte_count(base) != slot->size)
        return LIB_STATUS_INVALID_ARGUMENT;
    while (offset < slot->size) {
        lib_size count = (lib_size)slot->size - offset;
        lib_status status;
        if (count > sizeof(bytes)) count = sizeof(bytes);
        status = lib_storage_medium_read_at(base, offset, bytes, count);
        if (status != LIB_STATUS_OK) return status;
        hash_add(&hash, bytes, count);
        offset += count;
    }
    hash_end(&hash, digest);
    return memcmp(digest, slot->digest, sizeof(digest)) == 0 ?
        LIB_STATUS_OK : LIB_STATUS_INVALID_ARGUMENT;
}

void softpc_media_archive_dispose(softpc_media_archive **archive)
{
    unsigned i;
    if (archive == NULL || *archive == NULL) return;
    for (i=0; i<MEDIA_SLOTS; ++i) {
        media_slot *slot = &(*archive)->slots[i];
        media_page *page = slot->pages;
        while (page != NULL) {
            media_page *next = page->next;
            free(page); page = next;
        }
    }
    free(*archive); *archive = NULL;
}

static lib_status capture_slot(media_slot *slot, const softpc_media_view *view)
{
    lib_storage_medium *base = NULL;
    lib_storage_medium *source = view->medium;
    media_page **tail = &slot->pages;
    media_hash hash = hash_begin();
    lib_u8 original[MEDIA_BLOCK_BYTES], effective[MEDIA_BLOCK_BYTES];
    lib_size offset, size;
    lib_status status = LIB_STATUS_OK;

    if (source == NULL) return LIB_STATUS_OK;
    if (view->path == NULL || strlen(view->path) >= sizeof(slot->path))
        return LIB_STATUS_INVALID_ARGUMENT;
    slot->present = 1; slot->mode = view->mode; slot->cylinder = view->cylinder;
    memcpy(slot->path, view->path, strlen(view->path) + 1u);
    size = lib_storage_medium_byte_count(source); slot->size = size;
    if (view->mode == LIB_STORAGE_MEDIUM_OVERLAY) {
        if (view->path == NULL) return LIB_STATUS_INVALID_ARGUMENT;
        status = lib_storage_medium_open(view->path, LIB_STORAGE_MEDIUM_READONLY, &base);
        if (status != LIB_STATUS_OK) return status;
        if (lib_storage_medium_byte_count(base) != size) {
            lib_storage_medium_destroy(&base);
            return LIB_STATUS_INVALID_ARGUMENT;
        }
    }
    for (offset=0; offset<size; ) {
        lib_size count = size-offset;
        if (count > MEDIA_BLOCK_BYTES) count = MEDIA_BLOCK_BYTES;
        status = lib_storage_medium_read_at(base != NULL ? base : source,
            offset, original, count);
        if (status != LIB_STATUS_OK) break;
        hash_add(&hash, original, count);
        if (base != NULL) {
            status = lib_storage_medium_read_at(source, offset, effective, count);
            if (status != LIB_STATUS_OK) break;
            if (memcmp(original, effective, count) != 0) {
                media_page *page = calloc(1, sizeof(*page));
                if (page == NULL) { status = LIB_STATUS_NO_MEMORY; break; }
                page->offset = offset; page->count = (lib_u32)count;
                memcpy(page->bytes, effective, count);
                *tail = page; tail = &page->next; ++slot->page_count;
            }
        }
        offset += count;
    }
    if (status == LIB_STATUS_OK) hash_end(&hash, slot->digest);
    {
        lib_status closed = lib_storage_medium_destroy(&base);
        if (status == LIB_STATUS_OK) status = closed;
    }
    return status;
}

lib_status softpc_media_archive_capture(softpc_media_archive **archive)
{
    softpc_media_archive *result;
    lib_status status = LIB_STATUS_OK;
    unsigned i;
    if (archive == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    result = calloc(1, sizeof(*result));
    if (result == NULL) return LIB_STATUS_NO_MEMORY;
    for (i=0; i<MEDIA_SLOTS && status == LIB_STATUS_OK; ++i) {
        softpc_media_view view;
        media_view(i, &view);
        status = capture_slot(&result->slots[i], &view);
    }
    if (status != LIB_STATUS_OK) { softpc_media_archive_dispose(&result); return status; }
    softpc_media_archive_dispose(archive); *archive = result;
    return LIB_STATUS_OK;
}

lib_status softpc_media_archive_write(const softpc_media_archive *archive,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_status status;
    unsigned i;
    if (archive == NULL || write == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    for (i=0, status=LIB_STATUS_OK; i<MEDIA_SLOTS && status == LIB_STATUS_OK; ++i) {
        const media_slot *slot = &archive->slots[i];
        const media_page *page;
        status = softpc_snapshot_stream_write_u32(write, context, slot->present);
        if (status != LIB_STATUS_OK || !slot->present) continue;
        status = softpc_snapshot_stream_write_u32(write, context, slot->mode);
        if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write, context, slot->cylinder);
        if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u64(write, context, slot->size);
        if (status == LIB_STATUS_OK) status = write(context, slot->digest, sizeof(slot->digest));
        if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
            context, (lib_u32)strlen(slot->path));
        if (status == LIB_STATUS_OK) status = write(context,
            (const lib_u8 *)slot->path, strlen(slot->path));
        if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u64(write, context, slot->page_count);
        for (page=slot->pages; page != NULL && status == LIB_STATUS_OK; page=page->next) {
            status = softpc_snapshot_stream_write_u64(write, context, page->offset);
            if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write, context, page->count);
            if (status == LIB_STATUS_OK) status = write(context, page->bytes, page->count);
        }
    }
    return status;
}

lib_status softpc_media_archive_read(softpc_media_archive **archive,
    softpc_snapshot_bytes_read read, void *context)
{
    softpc_media_archive *result;
    lib_status status;
    unsigned i;
    if (archive == NULL || read == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = LIB_STATUS_OK;
    result = calloc(1, sizeof(*result));
    if (result == NULL) return LIB_STATUS_NO_MEMORY;
    for (i=0; i<MEDIA_SLOTS && status == LIB_STATUS_OK; ++i) {
        media_slot *slot = &result->slots[i];
        media_page **tail = &slot->pages;
        lib_u64 p, previous = 0;
        lib_u32 path_length;
        status = softpc_snapshot_stream_read_u32(read, context, &slot->present);
        if (status != LIB_STATUS_OK) break;
        if (slot->present > 1) { status = LIB_STATUS_INVALID_ARGUMENT; break; }
        if (!slot->present) continue;
        status = softpc_snapshot_stream_read_u32(read, context, &slot->mode);
        if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &slot->cylinder);
        if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u64(read, context, &slot->size);
        if (status == LIB_STATUS_OK) status = read(context, slot->digest, sizeof(slot->digest));
        if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read,
            context, &path_length);
        if (status == LIB_STATUS_OK && (path_length == 0u ||
            path_length >= sizeof(slot->path)))
            status = LIB_STATUS_INVALID_ARGUMENT;
        if (status == LIB_STATUS_OK)
            status = read(context, (lib_u8 *)slot->path, path_length);
        if (status == LIB_STATUS_OK) slot->path[path_length] = '\0';
        if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u64(read, context, &slot->page_count);
        if (status != LIB_STATUS_OK) break;
        if (slot->mode > LIB_STORAGE_MEDIUM_OVERLAY || slot->size == 0 ||
            slot->size > (lib_u64)(lib_size)-1 || slot->size < 512 ||
            (i < 2 && slot->size % 512 != 0) ||
            slot->cylinder > (i < 2 ? 255u : 0u) ||
            slot->page_count > (slot->size-1)/MEDIA_BLOCK_BYTES+1 ||
            (slot->mode != LIB_STORAGE_MEDIUM_OVERLAY && slot->page_count != 0)) {
            status = LIB_STATUS_INVALID_ARGUMENT; break;
        }
        for (p=0; p<slot->page_count; ++p) {
            lib_u64 offset;
            lib_u32 bytes;
            media_page *page;
            status = softpc_snapshot_stream_read_u64(read, context, &offset);
            if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &bytes);
            if (status != LIB_STATUS_OK) break;
            if (offset >= slot->size || offset % MEDIA_BLOCK_BYTES != 0 ||
                (p != 0 && offset <= previous) || bytes == 0 ||
                bytes != (slot->size-offset < MEDIA_BLOCK_BYTES ? slot->size-offset : MEDIA_BLOCK_BYTES)) {
                status = LIB_STATUS_INVALID_ARGUMENT; break;
            }
            page = calloc(1, sizeof(*page));
            if (page == NULL) { status = LIB_STATUS_NO_MEMORY; break; }
            page->offset = offset; page->count = bytes;
            *tail = page; tail = &page->next; previous = offset;
            status = read(context, page->bytes, bytes);
            if (status != LIB_STATUS_OK) break;
        }
    }
    if (status != LIB_STATUS_OK) { softpc_media_archive_dispose(&result); return status; }
    softpc_media_archive_dispose(archive); *archive = result;
    return LIB_STATUS_OK;
}

static lib_status media_target_mode(const media_slot *slot, unsigned index,
    lib_storage_medium_mode hard_disk_mode, lib_storage_medium_mode *out_mode)
{
    if (slot == NULL || out_mode == NULL || hard_disk_mode > LIB_STORAGE_MEDIUM_OVERLAY)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (index < 2u) {
        *out_mode = (lib_storage_medium_mode)slot->mode;
        return LIB_STATUS_OK;
    }
    if (slot->mode == LIB_STORAGE_MEDIUM_OVERLAY &&
        hard_disk_mode == LIB_STORAGE_MEDIUM_READONLY)
        return LIB_STATUS_INVALID_ARGUMENT;
    *out_mode = hard_disk_mode == LIB_STORAGE_MEDIUM_OVERLAY ?
        LIB_STORAGE_MEDIUM_OVERLAY : hard_disk_mode;
    return LIB_STATUS_OK;
}

static lib_bool media_same_path(const media_slot *slot,
    const softpc_media_view *view)
{
    return view->medium != NULL && view->path != NULL &&
        strcmp(slot->path, view->path) == 0;
}

static lib_status media_apply_pages(const media_slot *slot,
    lib_storage_medium *medium)
{
    const media_page *page;
    lib_status status = LIB_STATUS_OK;
    for (page = slot->pages; page != NULL && status == LIB_STATUS_OK;
        page = page->next)
        status = lib_storage_medium_write_at(medium, (lib_size)page->offset,
            page->bytes, page->count);
    return status;
}

static lib_status media_prepare_slot(media_slot *slot, unsigned index,
    lib_storage_medium_mode target_mode)
{
    softpc_media_view view;
    lib_storage_medium *base_opened = NULL;
    lib_storage_medium *base;
    lib_status status;

    slot->target_mode = target_mode;
    if (!slot->present) return LIB_STATUS_OK;
    media_view(index, &view);
    if (media_same_path(slot, &view) &&
        view.mode != LIB_STORAGE_MEDIUM_OVERLAY)
        base = view.medium;
    else {
        status = lib_storage_medium_open(slot->path, LIB_STORAGE_MEDIUM_READONLY,
            &base_opened);
        if (status != LIB_STATUS_OK) return status;
        base = base_opened;
    }
    status = verify_base(slot, base);
    {
        lib_status closed = lib_storage_medium_destroy(&base_opened);
        if (status == LIB_STATUS_OK) status = closed;
    }
    return status;
}

lib_status softpc_media_archive_prepare(softpc_media_archive *archive,
    lib_storage_medium_mode hard_disk_mode)
{
    lib_status status = LIB_STATUS_OK;
    unsigned i;

    if (archive == NULL || hard_disk_mode > LIB_STORAGE_MEDIUM_OVERLAY)
        return LIB_STATUS_INVALID_ARGUMENT;
    archive->prepared = LIB_FALSE;
    /* This product exposes only floppy A: and fixed disk C:.  Rejecting a
       populated unsupported slot avoids silently claiming to restore it. */
    if (archive->slots[1].present || archive->slots[3].present)
        return LIB_STATUS_INVALID_ARGUMENT;
    for (i = 0; i < MEDIA_SLOTS && status == LIB_STATUS_OK; ++i) {
        media_slot *slot = &archive->slots[i];
        lib_storage_medium_mode target_mode;
        status = media_target_mode(slot, i, hard_disk_mode, &target_mode);
        if (status == LIB_STATUS_OK)
            status = media_prepare_slot(slot, i, target_mode);
    }
    if (status != LIB_STATUS_OK) return status;
    archive->prepared = LIB_TRUE;
    return LIB_STATUS_OK;
}

lib_status softpc_media_archive_attachment(const softpc_media_archive *archive,
    unsigned slot, const char **path, lib_storage_medium_mode *mode)
{
    const media_slot *source;
    if (archive == NULL || path == NULL || mode == NULL || !archive->prepared ||
        slot >= MEDIA_SLOTS)
        return LIB_STATUS_INVALID_ARGUMENT;
    source = &archive->slots[slot];
    *path = source->present ? source->path : NULL;
    *mode = (lib_storage_medium_mode)source->target_mode;
    return LIB_STATUS_OK;
}

lib_status softpc_media_archive_restore(softpc_media_archive *archive)
{
    unsigned i;
    lib_status status;

    if (archive == NULL || !archive->prepared) return LIB_STATUS_INVALID_STATE;
    /* Restore is the sole mutation phase.  A failure after detaching a live
       attachment deliberately leaves that slot detached: preparation is the
       no-side-effect validation boundary, not a rollback transaction. */
    archive->prepared = LIB_FALSE;
    for (i = 0; i < MEDIA_SLOTS; ++i) {
        media_slot *slot = &archive->slots[i];
        softpc_media_view view;
        lib_storage_medium *replacement = NULL;
        lib_storage_medium_mode target = (lib_storage_medium_mode)slot->target_mode;
        lib_bool retain;
        media_view(i, &view);
        if (!slot->present) {
            if (i < 2u)
                status = softpc_floppy_media_restore(i, NULL, target,
                    &replacement, 0u);
            else
                status = softpc_hdd_media_restore(i - 2u, NULL, target,
                    &replacement);
            if (status != LIB_STATUS_OK) return status;
            continue;
        }
        /* An overlay always carries mutable pages beyond its verified base.
           It therefore never qualifies as a retained attachment, including
           when the saved source itself was readonly or direct. */
        retain = target != LIB_STORAGE_MEDIUM_OVERLAY &&
            slot->mode != LIB_STORAGE_MEDIUM_OVERLAY &&
            media_same_path(slot, &view) && view.mode == target;
        if (retain) {
            status = verify_base(slot, view.medium);
        } else {
            if (i < 2u)
                status = softpc_floppy_media_restore(i, NULL, target,
                    &replacement, 0u);
            else
                status = softpc_hdd_media_restore(i - 2u, NULL, target,
                    &replacement);
            if (status != LIB_STATUS_OK) return status;
            status = lib_storage_medium_open(slot->path, target, &replacement);
            if (status == LIB_STATUS_OK) status = verify_base(slot, replacement);
            if (status == LIB_STATUS_OK && slot->mode == LIB_STORAGE_MEDIUM_OVERLAY)
                status = media_apply_pages(slot, replacement);
            if (status != LIB_STATUS_OK) {
                (void)lib_storage_medium_destroy(&replacement);
                return status;
            }
        }
        if (i < 2u)
            status = softpc_floppy_media_restore(i, slot->path, target,
                &replacement, slot->cylinder);
        else
            status = softpc_hdd_media_restore(i - 2u, slot->path, target,
                &replacement);
        if (status != LIB_STATUS_OK) {
            (void)lib_storage_medium_destroy(&replacement);
            return status;
        }
    }
    return LIB_STATUS_OK;
}
