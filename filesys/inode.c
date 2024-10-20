#include "filesys/inode.h"
#include <list.h>
#include <debug.h>
#include <round.h>
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/free-map.h"
#include "threads/malloc.h"

#include "filesys/fat.h"

/* Identifies an inode. */
#define INODE_MAGIC 0x494e4f44

/* On-disk inode.
 * Must be exactly DISK_SECTOR_SIZE bytes long. */
struct inode_disk {
	disk_sector_t start;                /* First data sector. */
	off_t length;                       /* File size in bytes. */
	unsigned magic;                     /* Magic number. */
	uint32_t unused[124];               /* Not used. */
	uint32_t is_dir;					/* 0: file, 1: directory  */
};

/* Returns the number of sectors to allocate for an inode SIZE
 * bytes long. */
static inline size_t
bytes_to_sectors (off_t size) {
	// /**/printf("\n------- bytes_to_sectors -------\n");
	// /**/printf("------- bytes_to_sectors end -------\n\n");
	return DIV_ROUND_UP (size, DISK_SECTOR_SIZE);
}

/** #Project 4: File System - Error 처리 */
static disk_sector_t byte_to_sector(const struct inode *inode, off_t pos);

/* In-memory inode. */
struct inode {
	struct list_elem elem;              /* Element in inode list. */
	disk_sector_t sector;               /* Sector number of disk location. */
	int open_cnt;                       /* Number of openers. */
	bool removed;                       /* True if deleted, false otherwise. */
	int deny_write_cnt;                 /* 0: writes ok, >0: deny writes. */
	struct inode_disk data;             /* Inode content. */
};

#ifndef EFILESYS
/* Returns the disk sector that contains byte offset POS within
 * INODE.
 * Returns -1 if INODE does not contain data for a byte at offset
 * POS. */
static disk_sector_t
byte_to_sector (const struct inode *inode, off_t pos) {
	// /**/printf("\n------- byte_to_sector -------\n");
	ASSERT (inode != NULL);
	if (pos < inode->data.length) {
		// /**/printf("------- byte_to_sector end (pos < inode->data.length) -------\n\n");
		return inode->data.start + pos / DISK_SECTOR_SIZE;
	}
	else {
		// /**/printf("------- byte_to_sector end else -------\n\n");
		return -1;
	}
}
#endif

/* List of open inodes, so that opening a single inode twice
 * returns the same `struct inode'. */
static struct list open_inodes;

/* Initializes the inode module. */
void
inode_init (void) {
	// /**/printf("\n------- inode_init -------\n");
	list_init (&open_inodes);
	// /**/printf("------- inode_init end -------\n\n");
}

#ifndef EFILESYS
/** #Project 4: Subdirectories - Initializes an inode with LENGTH bytes of data and writes
 * the new inode to sector SECTOR on the file system disk.
 * Returns true if successful.
 * Returns false if memory or disk allocation fails. */
bool inode_create(disk_sector_t sector, off_t length, int32_t is_dir) {    
    struct inode_disk *disk_inode = NULL;
    bool success = false;

    ASSERT(length >= 0);

    /* If this assertion fails, the inode structure is not exactly
     * one sector in size, and you should fix that. */
    ASSERT(sizeof *disk_inode == DISK_SECTOR_SIZE);

    disk_inode = calloc(1, sizeof *disk_inode);
    if (disk_inode != NULL) {
        size_t sectors = bytes_to_sectors(length);
        disk_inode->length = length;
        disk_inode->magic = INODE_MAGIC;
        disk_inode->is_dir = is_dir;

        if (free_map_allocate(sectors, &disk_inode->start)) {
            disk_write(filesys_disk, sector, disk_inode);
            if (sectors > 0) {
                static char zeros[DISK_SECTOR_SIZE];
                size_t i;

                for (i = 0; i < sectors; i++)
                    disk_write(filesys_disk, disk_inode->start + i, zeros);
            }
            success = true;
        }
        free(disk_inode);
    }
    return success;
}
#endif

/* Reads an inode from SECTOR
 * and returns a `struct inode' that contains it.
 * Returns a null pointer if memory allocation fails. */
struct inode *
inode_open (disk_sector_t sector) {
	// /**/printf("\n------- inode_open -------\n");
	struct list_elem *e;
	struct inode *inode;

	/* Check whether this inode is already open. */
	for (e = list_begin (&open_inodes); e != list_end (&open_inodes);
			e = list_next (e)) {
		inode = list_entry (e, struct inode, elem);
		if (inode->sector == sector) {
			inode_reopen (inode);
			// /**/printf("------- inode_open end (inode->sector == sector) -------\n\n");
			return inode; 
		}
	}

	/* Allocate memory. */
	inode = malloc (sizeof *inode);
	if (inode == NULL) {
		// /**/printf("------- inode_open end (inode == NULL) -------\n\n");
		return NULL;
	}

	/* Initialize. */
	list_push_front (&open_inodes, &inode->elem);
	inode->sector = sector;
	inode->open_cnt = 1;
	inode->deny_write_cnt = 0;
	inode->removed = false;
	disk_read (filesys_disk, inode->sector, &inode->data);
	// /**/printf("------- inode_open end -------\n\n");
	return inode;
}

/* Reopens and returns INODE. */
struct inode *
inode_reopen (struct inode *inode) {
	// /**/printf("\n------- inode_reopen -------\n");
	if (inode != NULL)
		inode->open_cnt++;
	// /**/printf("------- inode_reopen end -------\n\n");
	return inode;
}

/* Returns INODE's inode number. */
disk_sector_t
inode_get_inumber (const struct inode *inode) {
	// /**/printf("\n------- inode_get_inumber -------\n");
	// /**/printf("------- inode_get_inumber end -------\n\n");
	return inode->sector;
}

#ifndef EFILESYS
/* Closes INODE and writes it to disk.
 * If this was the last reference to INODE, frees its memory.
 * If INODE was also a removed inode, frees its blocks. */
void
inode_close (struct inode *inode) {
	// /**/printf("\n------- inode_close -------\n");
	/* Ignore null pointer. */
	if (inode == NULL) {
		// /**/printf("------- inode_close end (inode == NULL) -------\n\n");
		return;
	}

	/* Release resources if this was the last opener. */
	if (--inode->open_cnt == 0) {
		/* Remove from inode list and release lock. */
		list_remove (&inode->elem);

		/* Deallocate blocks if removed. */
		if (inode->removed) {
			free_map_release (inode->sector, 1);
			free_map_release (inode->data.start,
					bytes_to_sectors (inode->data.length)); 
		}

		free (inode); 
	}
	// /**/printf("------- inode_close end -------\n\n");
}
#endif

/* Marks INODE to be deleted when it is closed by the last caller who
 * has it open. */
void
inode_remove (struct inode *inode) {
	// /**/printf("\n------- inode_remove -------\n");
	ASSERT (inode != NULL);
	inode->removed = true;
	// /**/printf("------- inode_remove end -------\n\n");
}

/* Reads SIZE bytes from INODE into BUFFER, starting at position OFFSET.
 * Returns the number of bytes actually read, which may be less
 * than SIZE if an error occurs or end of Efile is reached. */
off_t
inode_read_at (struct inode *inode, void *buffer_, off_t size, off_t offset) {
	// /**/printf("\n------- inode_read_at -------\n");
	uint8_t *buffer = buffer_;
	off_t bytes_read = 0;
	uint8_t *bounce = NULL;

	while (size > 0) {
		/* Disk sector to read, starting byte offset within sector. */
		disk_sector_t sector_idx = byte_to_sector (inode, offset);
		int sector_ofs = offset % DISK_SECTOR_SIZE;

		/* Bytes left in inode, bytes left in sector, lesser of the two. */
		off_t inode_left = inode_length (inode) - offset;
		int sector_left = DISK_SECTOR_SIZE - sector_ofs;
		int min_left = inode_left < sector_left ? inode_left : sector_left;

		/* Number of bytes to actually copy out of this sector. */
		int chunk_size = size < min_left ? size : min_left;
		if (chunk_size <= 0)
			break;

		if (sector_ofs == 0 && chunk_size == DISK_SECTOR_SIZE) {
			/* Read full sector directly into caller's buffer. */
			disk_read (filesys_disk, sector_idx, buffer + bytes_read); 
		} else {
			/* Read sector into bounce buffer, then partially copy
			 * into caller's buffer. */
			if (bounce == NULL) {
				bounce = malloc (DISK_SECTOR_SIZE);
				if (bounce == NULL)
					break;
			}
			disk_read (filesys_disk, sector_idx, bounce);
			memcpy (buffer + bytes_read, bounce + sector_ofs, chunk_size);
		}

		/* Advance. */
		size -= chunk_size;
		offset += chunk_size;
		bytes_read += chunk_size;
	}
	free (bounce);

	// /**/printf("------- inode_read_at end -------\n\n");
	return bytes_read;
}

#ifndef EFILESYS
/* Writes SIZE bytes from BUFFER into INODE, starting at OFFSET.
 * Returns the number of bytes actually written, which may be
 * less than SIZE if end of Efile is reached or an error occurs.
 * (Normally a write at end of Efile would extend the inode, but
 * growth is not yet implemented.) */
off_t
inode_write_at (struct inode *inode, const void *buffer_, off_t size,
		off_t offset) {
	// /**/printf("\n------- inode_write_at -------\n");
	const uint8_t *buffer = buffer_;
	off_t bytes_written = 0;
	uint8_t *bounce = NULL;
    off_t origin_offset = offset;

	if (inode->deny_write_cnt) {
		// /**/printf("------- inode_write_at end (inode->deny_write_cnt) -------\n\n");
		return 0;
	}

	while (size > 0) {
		/* Sector to write, starting byte offset within sector. */
		disk_sector_t sector_idx = byte_to_sector (inode, offset);
		int sector_ofs = offset % DISK_SECTOR_SIZE;

		/* Bytes left in inode, bytes left in sector, lesser of the two. */
		off_t inode_left = inode_length (inode) - offset;
		int sector_left = DISK_SECTOR_SIZE - sector_ofs;
		int min_left = inode_left < sector_left ? inode_left : sector_left;

		/* Number of bytes to actually write into this sector. */
		int chunk_size = size < min_left ? size : min_left;
		if (chunk_size <= 0)
			break;

		if (sector_ofs == 0 && chunk_size == DISK_SECTOR_SIZE) {
			/* Write full sector directly to disk. */
			disk_write (filesys_disk, sector_idx, buffer + bytes_written); 
		} else {
			/* We need a bounce buffer. */
			if (bounce == NULL) {
				bounce = malloc (DISK_SECTOR_SIZE);
				if (bounce == NULL)
					break;
			}

			/* If the sector contains data before or after the chunk
			   we're writing, then we need to read in the sector
			   first.  Otherwise we start with a sector of all zeros. */
			if (sector_ofs > 0 || chunk_size < sector_left) 
				disk_read (filesys_disk, sector_idx, bounce);
			else
				memset (bounce, 0, DISK_SECTOR_SIZE);
			memcpy (bounce + sector_ofs, buffer + bytes_written, chunk_size);
			disk_write (filesys_disk, sector_idx, bounce); 
		}

		/* Advance. */
		size -= chunk_size;
		offset += chunk_size;
		bytes_written += chunk_size;
	}
	free (bounce);

	// /**/printf("------- inode_write_at end -------\n\n");
	return bytes_written;
}
#endif

/* Disables writes to INODE.
   May be called at most once per inode opener. */
	void
inode_deny_write (struct inode *inode) 
{
	// /**/printf("\n------- inode_deny_write -------\n");
	inode->deny_write_cnt++;
	ASSERT (inode->deny_write_cnt <= inode->open_cnt);
	// /**/printf("------- inode_deny_write end -------\n\n");
}

/* Re-enables writes to INODE.
 * Must be called once by each inode opener who has called
 * inode_deny_write() on the inode, before closing the inode. */
void
inode_allow_write (struct inode *inode) {
	// /**/printf("\n------- inode_allow_write -------\n");
	ASSERT (inode->deny_write_cnt > 0);
	ASSERT (inode->deny_write_cnt <= inode->open_cnt);
	inode->deny_write_cnt--;
	// /**/printf("------- inode_allow_write end -------\n\n");
}

/* Returns the length, in bytes, of INODE's data. */
off_t
inode_length (const struct inode *inode) {
	// /**/printf("\n------- inode_length -------\n");
	// /**/printf("------- inode_length end -------\n\n");
	return inode->data.length;
}

#ifdef EFILESYS
static disk_sector_t byte_to_sector(const struct inode *inode, off_t pos) {
    ASSERT(inode != NULL);

    cluster_t target = sector_to_cluster(inode->data.start);

    /* file length와 관계없이 pos에 크기에 따라 계속 진행
       file length보다 pos가 크면 새로운 cluster를 할당해가면서 진행 */
    while (pos >= DISK_SECTOR_SIZE) {
        if (fat_get(target) == EOChain)
            fat_create_chain(target);

        target = fat_get(target);
        pos -= DISK_SECTOR_SIZE;
    }

    disk_sector_t sector = cluster_to_sector(target);
    return sector;
}

/** #Project 4: File System - Initializes an inode with LENGTH bytes of data and writes
 * the new inode to sector SECTOR on the file system disk.
 * Returns true if successful.
 * Returns false if memory or disk allocation fails. */
bool inode_create(disk_sector_t sector, off_t length, int32_t is_dir) {
    struct inode_disk *disk_inode = NULL;
    bool success = false;

    ASSERT(length >= 0);
    ASSERT(sizeof *disk_inode == DISK_SECTOR_SIZE);

    /* create disk_node and initialize*/
    disk_inode = calloc(1, sizeof *disk_inode);
    if (disk_inode != NULL) {
        cluster_t start_clst = fat_create_chain(0);
        size_t sectors = bytes_to_sectors(length);

        disk_inode->length = length;
        disk_inode->magic = INODE_MAGIC;
        disk_inode->is_dir = is_dir;  // File or directory
        disk_inode->start = cluster_to_sector(start_clst);

        /* write disk_inode on disk */
        disk_write(filesys_disk, sector, disk_inode);

        if (sectors > 0) {
            static char zeros[DISK_SECTOR_SIZE];
            cluster_t target = start_clst;
            disk_sector_t w_sector;

            /* make cluster chain based length and initialize zero*/
            while (sectors > 0) {
                w_sector = cluster_to_sector(target);
                disk_write(filesys_disk, w_sector, zeros);

                target = fat_create_chain(target);
                sectors--;
            }
        }
        success = true;
    }
    free(disk_inode);
    return success;
}

/** #Project 4: File System - Closes INODE and writes it to disk.
 * If this was the last reference to INODE, frees its memory.
 * If INODE was also a removed inode, frees its blocks. */
void inode_close(struct inode *inode) {
    /* Ignore null pointer. */
    if (inode == NULL)
        return;

    /* Release resources if this was the last opener. */
    if (--inode->open_cnt == 0) {
        /* Remove from inode list and release lock. */
        list_remove(&inode->elem);

        /* Deallocate blocks if removed. */
        if (inode->removed) {
            fat_remove_chain(inode->sector, 0);
            // cluster_t clst = sector_to_cluster(inode->sector);  // disk inode 삭제
            // fat_remove_chain(clst, 0);

            // clst = sector_to_cluster(inode->data.start);  // file data 삭제
            // fat_remove_chain(clst, 0);
        }

        // disk_write(filesys_disk, inode->sector, &inode->data);  // file close 시 변경사항 저장

        free(inode);
    }
}

/** #Project 4: File System - Writes SIZE bytes from BUFFER into INODE, starting at OFFSET.
 * Returns the number of bytes actually written, which may be less than SIZE if end of file is reached or an error occurs.
 * (Normally a write at end of file would extend the inode, but growth is not yet implemented.) */
off_t inode_write_at(struct inode *inode, const void *buffer_, off_t size, off_t offset) {
    const uint8_t *buffer = buffer_;
    off_t bytes_written = 0;
    uint8_t *bounce = NULL;
    off_t ori_offset = offset; // backup

    if (inode->deny_write_cnt)
        return 0;

    while (size > 0) {
        /* Sector to write, starting byte offset within sector. */
        disk_sector_t sector_idx = byte_to_sector(inode, offset);
        int sector_ofs = offset % DISK_SECTOR_SIZE;

        /* Bytes left in inode, bytes left in sector, lesser of the two. */
        off_t inode_left = inode_length(inode) - offset;
        int sector_left = DISK_SECTOR_SIZE - sector_ofs;
        int min_left = inode_left < sector_left ? inode_left : sector_left;

        /* Number of bytes to actually write into this sector. */
        int chunk_size = size < min_left ? size : min_left;
        if (chunk_size <= 0)
            break;

        if (sector_ofs == 0 && chunk_size == DISK_SECTOR_SIZE) {
            /* Write full sector directly to disk. */
            disk_write(filesys_disk, sector_idx, buffer + bytes_written);
        } else {
            /* We need a bounce buffer. */
            if (bounce == NULL) {
                bounce = malloc(DISK_SECTOR_SIZE);
                if (bounce == NULL)
                    break;
            }

            /* If the sector contains data before or after the chunk
               we're writing, then we need to read in the sector
               first.  Otherwise we start with a sector of all zeros. */
            if (sector_ofs > 0 || chunk_size < sector_left)
                disk_read(filesys_disk, sector_idx, bounce);
            else
                memset(bounce, 0, DISK_SECTOR_SIZE);
            memcpy(bounce + sector_ofs, buffer + bytes_written, chunk_size);
            disk_write(filesys_disk, sector_idx, bounce);
        }

        /* Advance. */
        size -= chunk_size;
        offset += chunk_size;
        bytes_written += chunk_size;
    }
    free(bounce);

    // if (inode_length(inode) < ori_offset + bytes_written) // inode length 갱신
    //     inode->data.length = ori_offset + bytes_written;

    return bytes_written;
}
#endif

/** #Project 4: Subdirectories - Returns the is_dir, in bool, of INODE's data. */
bool inode_is_dir(const struct inode *inode) {
    return inode->data.is_dir;
}

/** #Project 4: Subdirectories - Returns the removed, in bool, of INODE */
bool inode_is_removed(const struct inode *inode){
    return inode->removed;
}

/** #Project 4: Subdirectories - Returns the sector, in bytes, of INODE */
disk_sector_t inode_sector(struct inode *inode){
	return inode->sector;
}