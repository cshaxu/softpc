#!/usr/bin/env node

/*
 * Build a reproducible FAT16 guest disk for standalone SoftPC testing.
 *
 * The supplied DOS 5 hard-disk image is a useful boot template but also
 * contains an old Windows installation. This tool retains only its three
 * DOS boot files, then installs the supplied Windows 3.1 distribution in
 * C:\EWIN31. An optional DOS companion image supplies guest-only boot
 * configuration such as HIMEM.SYS. It never changes either source image and
 * refuses to overwrite an existing output image.
 */

import crypto from 'node:crypto';
import fs from 'node:fs';
import path from 'node:path';

function fail(message) { throw new Error(`create_win31_guest_media: ${message}`); }
function usage() { fail('usage: node tools/create_win31_guest_media.mjs <dos-template.img> <new-output.img> <win31-source-dir> [dos-companion.img] [--guest-himem] [--guest-emm386]'); }

const args = process.argv.slice(2);
let guestHimem = false;
let guestEmm386 = false;
while (args.at(-1) === '--guest-himem' || args.at(-1) === '--guest-emm386') {
    const flag = args.pop();
    if (flag === '--guest-himem') guestHimem = true;
    else guestEmm386 = true;
}
if (guestEmm386) guestHimem = true;
const [templatePath, outputPath, sourcePath, companionPath] = args;
if (!templatePath || !outputPath || !sourcePath || args.length < 3 || args.length > 4) usage();
if (guestEmm386 && !companionPath)
    fail('--guest-emm386 requires a DOS companion supplying HIMEM.SYS');
if (fs.existsSync(outputPath)) fail(`refusing to overwrite ${outputPath}`);

const image = fs.readFileSync(templatePath);
const sourceEntries = fs.readdirSync(sourcePath, { withFileTypes: true })
    .filter((entry) => entry.isFile())
    .map((entry) => ({ name: entry.name.toUpperCase(), data: fs.readFileSync(path.join(sourcePath, entry.name)) }))
    .sort((left, right) => left.name.localeCompare(right.name));

function shortName(name) {
    if (name === '.') return Buffer.from('.          ', 'ascii');
    if (name === '..') return Buffer.from('..         ', 'ascii');
    if (!/^[A-Z0-9!#$%&'()@^_`{}~.-]+$/.test(name)) fail(`not an 8.3 DOS file name: ${name}`);
    const split = name.split('.');
    if (split.length > 2 || split[0].length === 0 || split[0].length > 8 ||
        (split.length === 2 && (split[1].length === 0 || split[1].length > 3))) fail(`not an 8.3 DOS file name: ${name}`);
    return Buffer.from(split[0].padEnd(8, ' ') + (split.length === 2 ? split[1] : '').padEnd(3, ' '), 'ascii');
}
for (const entry of sourceEntries) shortName(entry.name);

function expandSzdd(input) {
    const signature = Buffer.from([0x53, 0x5a, 0x44, 0x44, 0x88, 0xf0, 0x27, 0x33, 0x41]);
    if (!input.subarray(0, signature.length).equals(signature))
        fail('expected SZDD-compressed Windows 3.1 file');
    const length = input.readUInt32LE(10);
    const output = Buffer.alloc(length);
    const window = Buffer.alloc(4096, 0x20);
    let source = 14, target = 0, ring = 0xfee, flags = 0, remaining = 0;
    while (target < length) {
        if (remaining === 0) { flags = input[source++]; remaining = 8; }
        if ((flags & 1) !== 0) {
            const value = input[source++];
            output[target++] = value; window[ring] = value; ring = (ring + 1) & 0xfff;
        } else {
            let position = input[source++] | ((input[source++] & 0xf0) << 4);
            let count = (input[source - 1] & 0x0f) + 3;
            while (count-- !== 0 && target < length) {
                const value = window[position++ & 0xfff];
                output[target++] = value; window[ring] = value; ring = (ring + 1) & 0xfff;
            }
        }
        flags >>>= 1; --remaining;
    }
    return output;
}

const partitionLba = image.readUInt32LE(0x1be + 8);
const bootOffset = partitionLba * 512;
const bytesPerSector = image.readUInt16LE(bootOffset + 11);
const sectorsPerCluster = image[bootOffset + 13];
const reservedSectors = image.readUInt16LE(bootOffset + 14);
const fatCount = image[bootOffset + 16];
const rootEntryCount = image.readUInt16LE(bootOffset + 17);
const totalSectors = image.readUInt16LE(bootOffset + 19) || image.readUInt32LE(bootOffset + 32);
const sectorsPerFat = image.readUInt16LE(bootOffset + 22);
if (bytesPerSector !== 512 || sectorsPerCluster === 0 || fatCount < 1 || rootEntryCount === 0 || totalSectors === 0 || sectorsPerFat === 0) fail('template is not the expected FAT16 hard-disk layout');
const rootSectors = Math.ceil(rootEntryCount * 32 / bytesPerSector);
const fatOffset = bootOffset + reservedSectors * bytesPerSector;
const rootOffset = fatOffset + fatCount * sectorsPerFat * bytesPerSector;
const dataOffset = rootOffset + rootSectors * bytesPerSector;
const clusterBytes = bytesPerSector * sectorsPerCluster;
const clusterCount = Math.floor((totalSectors - reservedSectors - fatCount * sectorsPerFat - rootSectors) / sectorsPerCluster);
const maxCluster = clusterCount + 1;

function fatValue(cluster) { return image.readUInt16LE(fatOffset + cluster * 2); }
function clusterOffset(cluster) {
    if (cluster < 2 || cluster > maxCluster) fail(`invalid cluster ${cluster}`);
    return dataOffset + (cluster - 2) * clusterBytes;
}
function originalRootEntry(name) {
    const wanted = shortName(name);
    for (let index = 0; index < rootEntryCount; ++index) {
        const offset = rootOffset + index * 32;
        if (image[offset] === 0) break;
        if (image[offset] === 0xe5 || image[offset + 11] === 0x0f) continue;
        if (image.subarray(offset, offset + 11).equals(wanted)) return { attr: image[offset + 11], firstCluster: image.readUInt16LE(offset + 26), size: image.readUInt32LE(offset + 28) };
    }
    fail(`DOS boot file ${name} is absent from the template`);
}
function readOriginalFile(entry) {
    const chunks = [];
    let cluster = entry.firstCluster;
    let remaining = entry.size;
    let guard = 0;
    while (remaining > 0) {
        if (cluster < 2 || cluster > maxCluster || guard++ > clusterCount) fail('invalid FAT chain in DOS template');
        const amount = Math.min(remaining, clusterBytes);
        chunks.push(Buffer.from(image.subarray(clusterOffset(cluster), clusterOffset(cluster) + amount)));
        remaining -= amount;
        cluster = fatValue(cluster);
    }
    return Buffer.concat(chunks);
}

function readFatRootFile(imagePath, name) {
    const companion = fs.readFileSync(imagePath);
    const companionBootOffset = companion.readUInt16LE(11) === 512 ? 0 :
        companion.readUInt32LE(0x1be + 8) * 512;
    const companionBytesPerSector = companion.readUInt16LE(companionBootOffset + 11);
    const companionSectorsPerCluster = companion[companionBootOffset + 13];
    const companionReserved = companion.readUInt16LE(companionBootOffset + 14);
    const companionFats = companion[companionBootOffset + 16];
    const companionRootCount = companion.readUInt16LE(companionBootOffset + 17);
    const companionSectorsPerFat = companion.readUInt16LE(companionBootOffset + 22);
    if (companionBytesPerSector !== 512 || companionSectorsPerCluster === 0 ||
        companionFats < 1 || companionRootCount === 0 || companionSectorsPerFat === 0)
        fail(`DOS companion is not the expected FAT image: ${imagePath}`);
    const companionFatOffset = companionBootOffset + companionReserved * companionBytesPerSector;
    const companionRootOffset = companionFatOffset + companionFats * companionSectorsPerFat * companionBytesPerSector;
    const companionDataOffset = companionRootOffset + Math.ceil(companionRootCount * 32 / companionBytesPerSector) * companionBytesPerSector;
    const companionClusterBytes = companionBytesPerSector * companionSectorsPerCluster;
    const companionTotalSectors = companion.readUInt16LE(companionBootOffset + 19) ||
        companion.readUInt32LE(companionBootOffset + 32);
    const companionRootSectors = Math.ceil(companionRootCount * 32 / companionBytesPerSector);
    const companionClusterCount = Math.floor((companionTotalSectors - companionReserved -
        companionFats * companionSectorsPerFat - companionRootSectors) / companionSectorsPerCluster);
    const companionFat12 = companionClusterCount < 4085;
    function companionFatValue(cluster) {
        if (!companionFat12) return companion.readUInt16LE(companionFatOffset + cluster * 2);
        const offset = companionFatOffset + Math.floor(cluster * 3 / 2);
        const value = companion.readUInt16LE(offset);
        return (cluster & 1) === 0 ? value & 0x0fff : value >>> 4;
    }
    const wanted = shortName(name);
    let entry = null;
    for (let index = 0; index < companionRootCount; ++index) {
        const offset = companionRootOffset + index * 32;
        if (companion[offset] === 0) break;
        if (companion[offset] === 0xe5 || companion[offset + 11] === 0x0f) continue;
        if (companion.subarray(offset, offset + 11).equals(wanted)) {
            entry = { attr: companion[offset + 11], firstCluster: companion.readUInt16LE(offset + 26), size: companion.readUInt32LE(offset + 28) };
            break;
        }
    }
    if (entry === null) fail(`DOS companion file ${name} is absent`);
    const chunks = [];
    let cluster = entry.firstCluster;
    let remaining = entry.size;
    let guard = 0;
    while (remaining > 0) {
        if (cluster < 2 || guard++ > 65535) fail(`invalid FAT chain for DOS companion ${name}`);
        const offset = companionDataOffset + (cluster - 2) * companionClusterBytes;
        const amount = Math.min(remaining, companionClusterBytes);
        chunks.push(Buffer.from(companion.subarray(offset, offset + amount)));
        remaining -= amount;
        cluster = companionFatValue(cluster);
    }
    return { name, attr: entry.attr, data: Buffer.concat(chunks) };
}
const bootFiles = ['IO.SYS', 'MSDOS.SYS', 'COMMAND.COM'].map((name) => {
    const entry = originalRootEntry(name);
    return { name, attr: entry.attr, data: readOriginalFile(entry) };
});

/* Rebuild the file allocation layer while retaining the template's MBR,
 * partition table and DOS boot sector. */
for (let copy = 0; copy < fatCount; ++copy) image.fill(0, fatOffset + copy * sectorsPerFat * bytesPerSector, fatOffset + (copy + 1) * sectorsPerFat * bytesPerSector);
image.fill(0, rootOffset, rootOffset + rootEntryCount * 32);
function setFat(cluster, value) {
    for (let copy = 0; copy < fatCount; ++copy) image.writeUInt16LE(value, fatOffset + copy * sectorsPerFat * bytesPerSector + cluster * 2);
}
setFat(0, 0xfff8);
setFat(1, 0xffff);
let nextCluster = 2;
function allocate(count) {
    if (count === 0) return 0;
    if (nextCluster + count - 1 > maxCluster) fail('guest source does not fit in the FAT16 template');
    const first = nextCluster;
    for (let index = 0; index < count; ++index) setFat(nextCluster + index, index + 1 < count ? nextCluster + index + 1 : 0xffff);
    nextCluster += count;
    return first;
}
function writeClusters(firstCluster, data) {
    if (data.length === 0) return;
    let cluster = firstCluster;
    let inputOffset = 0;
    while (inputOffset < data.length) {
        const destination = clusterOffset(cluster);
        image.fill(0, destination, destination + clusterBytes);
        const amount = Math.min(clusterBytes, data.length - inputOffset);
        data.copy(image, destination, inputOffset, inputOffset + amount);
        inputOffset += amount;
        cluster = image.readUInt16LE(fatOffset + cluster * 2);
    }
}
function writeDirectoryEntry(destination, name, attr, firstCluster, size) {
    destination.fill(0);
    shortName(name).copy(destination, 0);
    destination[11] = attr;
    destination.writeUInt16LE(firstCluster, 26);
    destination.writeUInt32LE(size, 28);
}

const files = [];
for (const bootFile of bootFiles) {
    const firstCluster = allocate(Math.ceil(bootFile.data.length / clusterBytes));
    writeClusters(firstCluster, bootFile.data);
    files.push({ ...bootFile, firstCluster });
}
if (guestHimem && !companionPath) {
    /* HIMEM remains a guest DOS driver.  This merely stages the original
       Windows 3.1 distribution file and a guest CONFIG.SYS; it neither links
       nor implements XMS in the standalone VM host. */
    const packed = fs.readFileSync(path.join(sourcePath, 'HIMEM.SY_'));
    const himem = { name: 'HIMEM.SYS', attr: 0x20, data: expandSzdd(packed) };
    const config = { name: 'CONFIG.SYS', attr: 0x20,
        data: Buffer.from('DEVICE=HIMEM.SYS\r\nFILES=30\r\n', 'ascii') };
    const autoexec = { name: 'AUTOEXEC.BAT', attr: 0x20,
        data: Buffer.from('', 'ascii') };
    for (const file of [himem, config, autoexec]) {
        const firstCluster = allocate(Math.ceil(file.data.length / clusterBytes));
        writeClusters(firstCluster, file.data);
        files.push({ ...file, firstCluster });
    }
}
if (companionPath) {
    for (const name of ['HIMEM.SYS', 'CONFIG.SYS', 'AUTOEXEC.BAT']) {
        const companionFile = readFatRootFile(companionPath, name);
        if (guestEmm386 && name === 'CONFIG.SYS') {
            companionFile.data = Buffer.from(
                'DEVICE=HIMEM.SYS\r\nDEVICE=EMM386.EXE RAM\r\nDOS=HIGH,UMB\r\nFILES=30\r\n',
                'ascii');
        }
        const firstCluster = allocate(Math.ceil(companionFile.data.length / clusterBytes));
        writeClusters(firstCluster, companionFile.data);
        files.push({ ...companionFile, firstCluster });
    }
}
if (guestEmm386) {
    const packed = fs.readFileSync(path.join(sourcePath, 'EMM386.EX_'));
    const data = expandSzdd(packed);
    const firstCluster = allocate(Math.ceil(data.length / clusterBytes));
    writeClusters(firstCluster, data);
    files.push({ name: 'EMM386.EXE', attr: 0x20, data, firstCluster });
}
const directoryEntries = 2 + sourceEntries.length;
const sourceDirectoryCluster = allocate(Math.ceil(directoryEntries * 32 / clusterBytes));
const directoryData = Buffer.alloc(Math.ceil(directoryEntries * 32 / clusterBytes) * clusterBytes);
writeDirectoryEntry(directoryData.subarray(0, 32), '.', 0x10, sourceDirectoryCluster, 0);
writeDirectoryEntry(directoryData.subarray(32, 64), '..', 0x10, 0, 0);
for (let index = 0; index < sourceEntries.length; ++index) {
    const source = sourceEntries[index];
    const firstCluster = allocate(Math.ceil(source.data.length / clusterBytes));
    writeClusters(firstCluster, source.data);
    writeDirectoryEntry(directoryData.subarray((index + 2) * 32, (index + 3) * 32), source.name, 0x20, firstCluster, source.data.length);
}
writeClusters(sourceDirectoryCluster, directoryData);
for (let index = 0; index < files.length; ++index) {
    const file = files[index];
    writeDirectoryEntry(image.subarray(rootOffset + index * 32, rootOffset + (index + 1) * 32), file.name, file.attr, file.firstCluster, file.data.length);
}
writeDirectoryEntry(image.subarray(rootOffset + files.length * 32, rootOffset + (files.length + 1) * 32), 'EWIN31', 0x10, sourceDirectoryCluster, 0);

fs.writeFileSync(outputPath, image, { flag: 'wx' });
console.log(`created ${outputPath}`);
console.log(`files=${sourceEntries.length} allocated_clusters=${nextCluster - 2}/${clusterCount} sha256=${crypto.createHash('sha256').update(image).digest('hex')}`);
