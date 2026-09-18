/* Test the private archive and real Storage without a CPU or media emulator. */
#include "../../src/compat/media_snapshot.c"
#include <assert.h>
#include <stdio.h>

static softpc_media_view views[4];

void softpc_floppy_media_view(unsigned slot, softpc_media_view *view)
{ *view = views[slot]; }
void softpc_hdd_media_view(unsigned slot, softpc_media_view *view)
{ *view = views[slot+2]; }

static lib_status replace(unsigned slot, lib_storage_medium **replacement)
{
    lib_storage_medium *retired = NULL;
    lib_status status;
    if (*replacement == NULL) return LIB_STATUS_OK;
    status = lib_storage_medium_replace(&views[slot].medium, *replacement, &retired);
    if (status != LIB_STATUS_OK) return status;
    *replacement = NULL;
    return lib_storage_medium_destroy(&retired);
}
lib_status softpc_floppy_media_restore(unsigned slot,
    lib_storage_medium **replacement, lib_u32 cylinder)
{
    views[slot].cylinder = cylinder;
    return replace(slot, replacement);
}
lib_status softpc_hdd_media_restore(unsigned slot, lib_storage_medium **replacement)
{ return replace(slot+2, replacement); }

typedef struct bytes_stream {
    lib_u8 bytes[32768];
    lib_size count, position;
} bytes_stream;

static lib_status write_bytes(void *context, const lib_u8 *bytes, lib_size count)
{
    bytes_stream *s = context;
    if (count > sizeof(s->bytes)-s->count) return LIB_STATUS_LIMIT_EXCEEDED;
    memcpy(s->bytes+s->count, bytes, count); s->count += count;
    return LIB_STATUS_OK;
}
static lib_status read_bytes(void *context, lib_u8 *bytes, lib_size count)
{
    bytes_stream *s = context;
    if (count > s->count-s->position) return LIB_STATUS_IO_ERROR;
    memcpy(bytes, s->bytes+s->position, count); s->position += count;
    return LIB_STATUS_OK;
}

static void attach(unsigned slot, const char *path, lib_storage_medium_mode mode)
{
    assert(lib_storage_medium_destroy(&views[slot].medium) == LIB_STATUS_OK);
    views[slot] = (softpc_media_view){NULL,path,mode,0};
    assert(lib_storage_medium_open(path, mode, &views[slot].medium) == LIB_STATUS_OK);
}

static void check_sha256(void)
{
    static const lib_u8 expected[32] = {
        0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
        0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad
    };
    static const lib_u8 long_expected[32] = {
        0xcd,0xc7,0x6e,0x5c,0x99,0x14,0xfb,0x92,0x81,0xa1,0xc7,0xe2,0x84,0xd7,0x3e,0x67,
        0xf1,0x80,0x9a,0x48,0xa4,0x97,0x20,0x0e,0x04,0x6d,0x39,0xcc,0xc7,0x11,0x2c,0xd0
    };
    media_hash h = hash_begin();
    lib_u8 digest[32], a[1000];
    hash_add(&h,(const lib_u8 *)"a",1);
    hash_add(&h,(const lib_u8 *)"bc",2);
    hash_end(&h,digest);
    assert(memcmp(digest,expected,32)==0);
    h=hash_begin(); memset(a,'a',sizeof(a));
    for (unsigned i=0;i<1000;++i) hash_add(&h,a,sizeof(a));
    hash_end(&h,digest);
    assert(memcmp(digest,long_expected,32)==0);
}

int main(void)
{
    const char *path = "media-snapshot-base.img";
    const char *second_path = "media-snapshot-second-base.img";
    softpc_media_archive *saved=NULL, *decoded=NULL;
    bytes_stream stream={0};
    lib_u8 base[8704]={0}, data[8704];
    FILE *file;
    unsigned slot;
    check_sha256();
    file=fopen(path,"wb"); assert(file!=NULL);
    assert(fwrite(base,1,sizeof(base),file)==sizeof(base)); assert(fclose(file)==0);
    file=fopen(second_path,"wb"); assert(file!=NULL);
    assert(fwrite(base,1,sizeof(base),file)==sizeof(base)); assert(fclose(file)==0);
    attach(0,path,LIB_STORAGE_MEDIUM_OVERLAY);
    attach(2,path,LIB_STORAGE_MEDIUM_OVERLAY);
    views[0].cylinder=37;
    for(slot=0;slot<4;slot+=2) {
        assert(lib_storage_medium_fill_at(views[slot].medium,4090,12,0x41+slot)==LIB_STATUS_OK);
        assert(lib_storage_medium_fill_at(views[slot].medium,8192,512,0x51+slot)==LIB_STATUS_OK);
    }
    assert(softpc_media_archive_capture(&saved)==LIB_STATUS_OK);
    assert(saved->slots[0].page_count==3 && saved->slots[2].page_count==3);
    assert(saved->slots[0].pages->next->next->count==512);
    assert(softpc_media_archive_write(saved,write_bytes,&stream)==LIB_STATUS_OK);
    assert(softpc_media_archive_read(&decoded,read_bytes,&stream)==LIB_STATUS_OK);
    assert(stream.position==stream.count);
    assert(softpc_media_archive_prepare(decoded,path,LIB_STORAGE_MEDIUM_READONLY,
        path,LIB_STORAGE_MEDIUM_OVERLAY)!=LIB_STATUS_OK);
    assert(softpc_media_archive_prepare(decoded,NULL,LIB_STORAGE_MEDIUM_OVERLAY,
        path,LIB_STORAGE_MEDIUM_OVERLAY)!=LIB_STATUS_OK);
    assert(softpc_media_archive_prepare(decoded,path,LIB_STORAGE_MEDIUM_OVERLAY,
        path,LIB_STORAGE_MEDIUM_OVERLAY)==LIB_STATUS_OK);
    /* A later write must not survive replacement, including an old dirty page. */
    assert(lib_storage_medium_fill_at(views[0].medium,1024,512,0xee)==LIB_STATUS_OK);
    views[0].cylinder=0;
    assert(softpc_media_archive_restore(decoded)==LIB_STATUS_OK);
    assert(views[0].cylinder==37);
    for(slot=0;slot<4;slot+=2) {
        memset(base,0,sizeof(base)); memset(base+4090,0x41+slot,12);
        memset(base+8192,0x51+slot,512);
        assert(lib_storage_medium_read_at(views[slot].medium,0,data,sizeof(data))==LIB_STATUS_OK);
        assert(memcmp(base,data,sizeof(base))==0);
    }
    softpc_media_archive_dispose(&decoded);
    /* Every truncation in the media payload fails without exposing partial state. */
    {
        lib_size full=stream.count;
        for(lib_size n=0;n<full;++n) {
            stream.count=n; stream.position=0;
            assert(softpc_media_archive_read(&decoded,read_bytes,&stream)!=LIB_STATUS_OK);
            assert(decoded==NULL);
        }
        stream.count=full;
    }
    /* Slot 0: 4 present + 4 mode + 4 cylinder + 8 size + 32 digest +
       8 page count. Its first page offset starts at byte 60. */
    stream.bytes[60]=1; stream.position=0;
    assert(softpc_media_archive_read(&decoded,read_bytes,&stream)!=LIB_STATUS_OK);
    stream.bytes[60]=0;
    {
        lib_size second=60+12+4096;
        lib_u8 old=stream.bytes[second+1];
        stream.bytes[second+1]=0; stream.position=0;
        assert(softpc_media_archive_read(&decoded,read_bytes,&stream)!=LIB_STATUS_OK);
        stream.bytes[second+1]=old;
    }
    softpc_media_archive_dispose(&saved);
    for(slot=0;slot<4;slot+=2) assert(lib_storage_medium_destroy(&views[slot].medium)==LIB_STATUS_OK);
    file=fopen(path,"rb"); assert(file!=NULL);
    assert(fread(data,1,sizeof(data),file)==sizeof(data)); assert(fclose(file)==0);
    memset(base,0,sizeof(base)); assert(memcmp(base,data,sizeof(base))==0);

    /* Empty overlay, readonly, and exclusive direct sources use the same codec. */
    for(unsigned mode=0;mode<=LIB_STORAGE_MEDIUM_OVERLAY;++mode) {
        attach(0,path,(lib_storage_medium_mode)mode);
        assert(softpc_media_archive_capture(&saved)==LIB_STATUS_OK);
        assert(saved->slots[0].page_count==0);
        assert(softpc_media_archive_prepare(saved,path,(lib_storage_medium_mode)mode,
            NULL,LIB_STORAGE_MEDIUM_OVERLAY)==LIB_STATUS_OK);
        assert(softpc_media_archive_restore(saved)==LIB_STATUS_OK);
        assert(lib_storage_medium_destroy(&views[0].medium)==LIB_STATUS_OK);
        /* The saved external base must not silently change after capture. */
        file=fopen(path,"r+b"); assert(file!=NULL);
        assert(fputc(0x77,file)==0x77); assert(fclose(file)==0);
        assert(softpc_media_archive_prepare(saved,path,(lib_storage_medium_mode)mode,
            NULL,LIB_STORAGE_MEDIUM_OVERLAY)!=LIB_STATUS_OK);
        file=fopen(path,"r+b"); assert(file!=NULL);
        assert(fputc(0,file)==0); assert(fclose(file)==0);
        softpc_media_archive_dispose(&saved);
    }
    /* Each configured device has its own policy; archive preparation must not
       accidentally validate both present slots against one shared mode. */
    attach(0,path,LIB_STORAGE_MEDIUM_OVERLAY);
    attach(2,second_path,LIB_STORAGE_MEDIUM_DIRECT);
    assert(softpc_media_archive_capture(&saved)==LIB_STATUS_OK);
    assert(softpc_media_archive_prepare(saved,path,LIB_STORAGE_MEDIUM_OVERLAY,
        second_path,LIB_STORAGE_MEDIUM_DIRECT)==LIB_STATUS_OK);
    assert(softpc_media_archive_restore(saved)==LIB_STATUS_OK);
    assert(softpc_media_archive_prepare(saved,path,LIB_STORAGE_MEDIUM_DIRECT,
        second_path,LIB_STORAGE_MEDIUM_OVERLAY)!=LIB_STATUS_OK);
    softpc_media_archive_dispose(&saved);
    assert(lib_storage_medium_destroy(&views[0].medium)==LIB_STATUS_OK);
    assert(lib_storage_medium_destroy(&views[2].medium)==LIB_STATUS_OK);
    assert(remove(path)==0);
    assert(remove(second_path)==0);
    puts("media archive: overlay replacement, modes, SHA-256 and malformed streams passed");
    return 0;
}
