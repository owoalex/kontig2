 
# NSF Root Header

| Position  | Purpose           | Value                 |
|-----------|-------------------|-----------------------|
| 0-3       | Magic number      | 0x6e73662f            |
| 4         | Major revision    | unit8_t               |
| 5         | Minor revision    | uint8_t               |
| 6         | Bitwise flags A   | see table             |
| 7         | RESERVED          | undefined             |
| 8-15      | Stream count      | uint64_t (LE)         |

"Stream count" includes all streams, even empty streams.

## Bitwise flags (A)

| Position  | Purpose           |
|-----------|-------------------|
| 0         | Dirty marker      |
| 1-7       | RESERVED          |

The "Dirty" bit highlights that a file is broken in some fundamental way. An NSF file in this state must be rebuilt. Dirty NSF files should be considered "read only" as they will only become more broken with further writes to streams.

# NSF Stream Header

| Position  | Purpose                           | Value                 |
|-----------|-----------------------------------|-----------------------|
| 0-7       | Total stream length (in bytes)    | uint64_t (LE)         |
| 8-15      | Stream data position (in bytes)   | uint64_t (LE)         |
| 16        | Stream type                       | uint8_t               |
| 17        | Index entry size                  | uint8_t               |
| 32-39     | Stream index position (in bytes)  | uint64_t (LE)         |
| 40-47     | Index count                       | unit64_t (LE)         |

"Index count" refers to how many entries of size "Index entry size" are present. Note: this value MUST include deleted indexes 
"Stream data position" refers to where the actual data (excluding index) starts.
"Total stream length" refets to the length of the actual data (excluding index).
Note: Sometimes implementations of NSFs will intentionally leave gaps within various data structures in order to facilitate the efficient implementation of mutable arrays.



| Type #    | Description                           |
|-----------|---------------------------------------|
| 0         | Empty stream                          |
| 6         | String data (tags)                    |
| 7         | Raw quality data (Phred:uint8_t)      |
| 8         | Raw nucelotide data (bitwise ACTG)    |
| 32        | K-mer data (tree of linked lists)     |
| 33        | K-mer data (tree of arrays)           |
| 64        | Polymorphisms                         |
| 65        | Matt information                      |
| 255       | Fragment record                       |


When an NSF file is edited, gaps may be left when deleting or resizing streams. This can leave the file in a "sparse" state. When this occurs, a fragment record should be added as a stream. The fragment record should denote all unallocated space unusued in the heap of the NSF file. There should only ever be one fragment record per NSF file. This fragment record may be referred to by some streams to attempt to slot in data wherever possible. When a fragment stream becomes empty, the fragment stream may be removed entirely.

# NSF Stream Entry Index

| Position  | Purpose                           | Value                 |
|-----------|-----------------------------------|-----------------------|
| 0-7       | Data position (in bytes)          | uint64_t (LE)         |
| 8-15      | Data length (in bytes)            | uint64_t (LE)         |
| 16        | Bitwise flags B                   | see table             |
| 17-23     | RESERVED                          | undefined             |
| 24-31     | Reserved space (in bytes)         | uint64_t (LE)         |
| >=32      | Stream type specific              | unit64_t (LE)         |

NSF stream indexes have certain properties which are fixed, i.e. not specific to specific stream types. These fields are required to facilitate defragmentation. In order to preserve the ability for higher order functions to restructure the file without significant work, any embedded references to other data entries MUST refer to index indicies, and not use absolute file offsets.

Reserved space indicates how much free space after the data position and size has been reserved (i.e. not taken up by following entries) that can be freely written to without having to move the entry data. This may be used for mutable data types, where some slight growth is expected.

## Bitwise flags (B)

| Position  | Purpose           |
|-----------|-------------------|
| 0         | Not Deleted       |
| 1-7       | RESERVED          |

Deleted entries *may* be entirely zero, to enable gzip to compress them more effectively. As soon as the bitwise flag "Not deleted" is *not* present the rest of the data structure will be ignored. The underlying storage will *not* be preserved by the defragmenter.
