 
# NSF Root Header

|-----------|-------------------|-----------------------|
| Position  | Purpose           | Value                 |
|-----------|-------------------|-----------------------|
| 0-3       | Magic number      | 0x6e73662f            |
| 4         | Major revision    | unit8_t               |
| 5         | Minor revision    | uint8_t               |
| 6         | Bitwise flags A   | see table             |
| 7         | RESERVED          | undefined             |
| 8-15      | Stream count      | uint64_t (LE)         |
|-----------|-------------------|-----------------------|

"Stream count" includes all streams, even empty streams.

## Bitwise flags (A)

|-----------|-------------------|
| Position  | Purpose           |
|-----------|-------------------|
| 0         | Dirty marker      |
| 1-7       | RESERVED          |
|-----------|-------------------|

The "Dirty" bit highlights that a file is fragmented but a fragment stream could not be written. An NSF file in this state must be rebuilt. Dirty NSF files should be considered "read only" as they will only become more fragmented with further writes to streams.

# NSF Stream Header

|-----------|-----------------------------------|-----------------------|
| Position  | Purpose                           | Value                 |
|-----------|-----------------------------------|-----------------------|
| 0-7       | Total stream length (in bytes)    | uint64_t (LE)         |
| 8-15      | Stream data offset (in bytes)     | uint64_t (LE)         |
| 16        | Stream type                       | uint8_t               |
| 17        | Index entry size                  | uint8_t               |
| 32-39     | Stream index offset               | uint64_t (LE)         |
| 40-47     | Index count                       | unit64_t (LE)         |
|-----------|-----------------------------------|-----------------------|

"Index count" refers to how many entries of size "Index entry size" are present.
"Stream data offset" refers to where the actual data (excluding index) starts.
"Total stream length" refets to the length of the actual data (excluding index).

|-----------|---------------------------------------|-------------------------------|
| Type #    | Description                           | Stream Size Means             |
|-----------|---------------------------------------|-------------------------------|
| 0         | Empty stream                          | undefined                     |
| 6         | String data (tags)                    | Number of strings             |
| 7         | Raw quality data (Phred:uint8_t)      | Number of reads               |
| 8         | Raw nucelotide data (bitwise ACTG)    | Number of reads               |
| 32        | K-mer data (tree of linked lists)     | Total tree size (in bytes)    |
| 33        | K-mer data (tree of arrays)           | Total tree size (in bytes)    |
| 64        | Polymorphisms                         | Number of contigs             |
| 65        | Matt information                      | Number of matts               |
| 255       | Fragment record                       | Number of fragments           |
|-----------|---------------------------------------|-------------------------------|

When an NSF file is edited, gaps may be left when deleting or resizing streams. This can leave the file in a "sparse" state. When this occurs, a fragment record should be added as a stream. The fragment record should denote all unallocated space unusued in the heap of the NSF file. There should only ever be one fragment record per NSF file. This fragment record may be referred to by some streams to attempt to slot in data wherever possible. When a fragment stream becomes empty, the fragment stream may be removed entirely.
